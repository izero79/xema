#ifndef LOCATION_H
#define LOCATION_H

#include <QString>

class Location
{
public:
    Location();

    Location(const QString &town, const QString &place, const QString &wgsCoordinate = QString(), const QString &ykjCoordinate = QString());

    void setTown(const QString &name);
    void setPlace(const QString &name);
    void setSweTown(const QString &name);
    void setSwePlace(const QString &name);
    void setEngTown(const QString &name);
    void setEngPlace(const QString &name);
    void setYKJCoordinate(const QString &ykj);
    void setWGSCoordinate(const QString &wgs);
    void setCustom(bool custom);

    QString town() const;
    QString place() const;
    QString engTown(bool allowEmpty = false) const;
    QString engPlace(bool allowEmpty = false) const;
    QString sweTown(bool allowEmpty = false) const;
    QString swePlace(bool allowEmpty = false) const;
    QString ykjCoordinate() const;
    QString wgsCoordinate() const;
    QString wgsCoordinateForFilter() const;
    QString coordinate() const;
    bool custom() const;

private:
    QString mTown;
    QString mPlace;
    QString mEngTown;
    QString mEngPlace;
    QString mSweTown;
    QString mSwePlace;
    QString mYKJCoordinate;
    QString mWGSCoordinate;
    bool mCustom;
};

#endif // LOCATION_H
