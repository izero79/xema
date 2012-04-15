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

signals:
    
public slots:

private:
    void init();

private:
    bool mGkInited;
    
};

#endif // COORDINATECONVERTER_H
