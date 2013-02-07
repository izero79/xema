#ifndef COORDINATECONVERTER_H
#define COORDINATECONVERTER_H

#include <QObject>
#include <QPair>

class CoordinateConverter : public QObject
{
    Q_OBJECT
public:
    explicit CoordinateConverter(QObject *parent = 0);
    QPair<double,double> wgsToykj(const double &xcoord, const double &ycoord);
    QPair<double,double> ykjTowgs(double xcoord, double ycoord);
    Q_INVOKABLE QString wgsToYkjString(const QString &wgsString);
    Q_INVOKABLE QString ykjToWgsString(const QString &ykjString);
    Q_INVOKABLE QString countCoordinates( const QString &wgs, int distanceInM, const QString &direction);
    Q_INVOKABLE QPair<double, double> directionValues(const QString &direction);

signals:
    
public slots:

private:
    void init();
    bool validDirection(const QString &direction);

private:
    bool mGkInited;
    
};

#endif // COORDINATECONVERTER_H
