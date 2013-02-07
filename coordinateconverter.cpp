#include "coordinateconverter.h"
#include "ykjetrs.h"
#include "gausskrueger.h"
#include <QDebug>
#include <QRegExp>
#include <QStringList>

/*
 * Gauss-Krüger-projektiomuunnosobjektit
 */

static struct gk_args kkj_gk;
static struct gk_args etrs_gk;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795029
#endif

#define DEG       (M_PI/180.0) /* yksi aste radiaaneina */
#define Hayford_a 6378388.0    /* Hayfordin ellipsoidin isoakselin puolikas */
#define Hayford_f (1.0/297.0)  /* ja litistyneisyys */
#define GRS80_a   6378137.0    /* Samat GRS80-ellipsoidille */
#define GRS80_f   (1.0/298.257222101)

static double degreesPerM_E = 0.000021320512;
static double degreesPerM_N = 0.000008979482;

CoordinateConverter::CoordinateConverter(QObject *parent) :
    QObject(parent),
    mGkInited(false)
{
    init();
}

void CoordinateConverter::init()
{

    if(mGkInited) {
        return;
    }
    gk_init(&etrs_gk, GRS80_a,   GRS80_f,   0.9996,  500000.0, 27.0*DEG);
    gk_init(&kkj_gk,  Hayford_a, Hayford_f, 1.0,    3500000.0, 27.0*DEG);

    mGkInited = true;
}

QPair<double,double> CoordinateConverter::wgsToykj(const double &xcoord, const double &ycoord)
{
    QPair<double,double> result;
    init();
    double xDEG = xcoord*DEG;
    double yDEG = ycoord*DEG;
    double newX = 0;
    double newY = 0;
    gk_sphericaltorectangular(&etrs_gk, xDEG, yDEG, &newX, &newY);
    int rc = 0;
    rc=etrstm35fin_ykj(newX, newY, &newX, &newY);

    if (newX < 0 || newY < 0) {
        result.first = 0;
        result.second = 0;
    } else {
        result.first = newX;
        result.second = newY;
    }
    return result;
}

QPair<double,double> CoordinateConverter::ykjTowgs(double xcoord, double ycoord)
{
    QPair<double,double> result;
    init();
    double newX = 0;
    double newY = 0;
    int rc = 0;
    rc=ykj_etrstm35fin(xcoord, ycoord, &newX, &newY);
    gk_rectangulartospherical(&etrs_gk, newX, newY, &newX, &newY);
    newX/=DEG, newY/=DEG;

    if (newX < 0 || newY < 0) {
        result.first = 0;
        result.second = 0;
    } else {
        result.first = newX;
        result.second = newY;
    }
    return result;
}

QString CoordinateConverter::wgsToYkjString(const QString &wgsString) {
    if (wgsString.contains(":") == false || wgsString.length() < 3) {
        return QString();
    }

    qDebug() << Q_FUNC_INFO << wgsString;
    QString tmpString = wgsString;
    tmpString.remove(":");
    tmpString = tmpString.remove(".");
    QRegExp re("\\-{0,1}\\d*\\-{0,1}\\d*");  // a digit (\d), zero or more times (*)
    if (re.exactMatch(tmpString) == false) {
        return QString();
    }

    QString x = wgsString.section(":", 0, 0);
    QString y = wgsString.section(":", 1, 1);
    double dx = x.toDouble();
    double dy = y.toDouble();
    if (dx < -90 || dx > 90 || dy < -180 || dy > 180) {
        return QString();
    }
    double ykjx = 0;
    double ykjy = 0;
    QPair<double,double> newCoord;
    newCoord = wgsToykj(dx, dy);
    ykjx = newCoord.first;
    ykjy = newCoord.second;
    if (ykjx == 0 || ykjy == 0) {
        return QString();
    }
    long newX = ykjx;
    long newY = ykjy;
    QString ykjX;
    ykjX.setNum(newX);
    QString ykjY;
    ykjY.setNum(newY);
    QString ykj = ykjX;
    ykj.append(":");
    ykj.append(ykjY);
    return ykj;

}

QString CoordinateConverter::ykjToWgsString(const QString &ykjString) {
    if (ykjString.contains(":") == false || ykjString.length() < 3) {
        return QString();
    }
    qDebug() << Q_FUNC_INFO << ykjString;
    QString tmpString = ykjString;
    tmpString.remove(":");
    QRegExp re("\\d*");  // a digit (\d), zero or more times (*)
    if (re.exactMatch(tmpString) == false) {
       return QString();
    }

    QString x = ykjString.section(":", 0, 0);
    QString y = ykjString.section(":", 1, 1);
    double dx = x.toDouble();
    double dy = y.toDouble();
    if (dx < 0 || dy < 0) {
        return QString();
    }
    double wgsx = 0;
    double wgsy = 0;
    QPair<double,double> newCoord;
    newCoord = ykjTowgs(dx, dy);
    wgsx = newCoord.first;
    wgsy = newCoord.second;
    if (wgsx == 0 || wgsy == 0) {
        return QString();
    }
    QString wgsX;
    wgsX.setNum(wgsx,'g',6);
    QString wgsY;
    wgsY.setNum(wgsy,'g',6);
    QString wgs = wgsX;
    wgs.append(":");
    wgs.append(wgsY);
    return wgs;
}


QString CoordinateConverter::countCoordinates( const QString &wgs, int distanceInM, const QString &direction) {
    if (distanceInM < 0) {
        return "";
    }
    if (wgs.isEmpty() || wgs.contains(":") == false || wgs.length() < 3) {
        return "";
    }

    if (validDirection(direction) == false ) {
        return "";
    }

    if (distanceInM == 0) {
        return wgs;
    }

    QPair<double,double> directionXY = directionValues(direction);

    QString wgsX = wgs.section(":", 0, 0);
    QString wgsY = wgs.section(":", 1, 1);
    double x = wgsX.toDouble();
    double y = wgsY.toDouble();


    double newX = x + degreesPerM_N * directionXY.first * distanceInM;
    double newY = y + degreesPerM_E * directionXY.second * distanceInM;

    QString newWgsX;
    newWgsX.setNum(newX,'g',6);
    QString newWgsY;
    newWgsY.setNum(newY,'g',6);
    QString newCoords = newWgsX;
    newCoords.append(":");
    newCoords.append(newWgsY);

    return newCoords;
}

bool CoordinateConverter::validDirection(const QString &direction) {
    bool valid = false;
    QStringList valids;
    valids << "N" << "E" << "S" << "W";
    valids << "NE" << "SE" << "SW" << "NW";
    valids << "NNE" << "ENE" << "ESE" << "SSE";
    valids << "SSW" << "WSW" << "WNW" << "NNW";
    if (valids.contains(direction)) {
        valid = true;
    }
    return valid;
}

QPair<double, double> CoordinateConverter::directionValues(const QString &direction) {
    QPair<double, double> values;
    double x = 0;
    double y = 0;
    if (direction == "N") {
        x = 1;
        y = 0;
    }
    if (direction == "NNE") {
        x = 0.923878;
        y = 0.382683;
    }
    if (direction == "NE") {
        x = 0.707107;
        y = 0.707107;
    }
    if (direction == "ENE") {
        x = 0.382683;
        y = 0.923878;
    }
    if (direction == "E") {
        x = 0;
        y = 1;
    }
    if (direction == "ESE") {
        x = -0.382683;
        y = 0.923878;
    }
    if (direction == "SE") {
        x = -0.707107;
        y = 0.707107;
    }
    if (direction == "SSE") {
        x = -0.923878;
        y = 0.382683;
    }

    if (direction == "S") {
        x = -1;
        y = 0;
    }
    if (direction == "SSW") {
        x = -0.923878;
        y = -0.382683;
    }
    if (direction == "SW") {
        x = -0.707107;
        y = -0.707107;
    }
    if (direction == "WSW") {
        x = -0.382683;
        y = -0.923878;
    }
    if (direction == "W") {
        x = 0;
        y = -1;
    }
    if (direction == "WNW") {
        x = 0.382683;
        y = -0.923878;
    }
    if (direction == "NW") {
        x = 0.707107;
        y = -0.707107;
    }
    if (direction == "NNW") {
        x = 0.923878;
        y = -0.382683;
    }

    values.first = x;
    values.second = y;

    return values;
}
