#include "coordinateconverter.h"
#include "ykjetrs.h"
#include "gausskrueger.h"
#include <QDebug>

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

    result.first = newX;
    result.second = newY;
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

    result.first = newX;
    result.second = newY;
    return result;
}
