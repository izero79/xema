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
    void setFinCountry(const QString &country);
    void setSweCountry(const QString &country);
    void setEngCountry(const QString &country);
    void setYKJCoordinate(const QString &ykj);
    void setWGSCoordinate(const QString &wgs);
    void setCustom(bool custom);
    void setOrganization(const QString &org);
    void setOrgAbbrev(const QString &org);

    QString localizedTown() const;
    QString localizedPlace() const;
    QString localizedCountry() const;
    QString town() const;
    QString place() const;
    QString finCountry() const;
    QString sweCountry(bool allowEmpty = false) const;
    QString engCountry(bool allowEmpty = false) const;
    QString engTown(bool allowEmpty = false) const;
    QString engPlace(bool allowEmpty = false) const;
    QString sweTown(bool allowEmpty = false) const;
    QString swePlace(bool allowEmpty = false) const;
    QString ykjCoordinate() const;
    QString wgsCoordinate() const;
    QString wgsCoordinateForFilter() const;
    QString coordinate() const;
    bool custom() const;
    QString organization() const;
    QString orgAbbrev() const;

private:
    QString mTown;
    QString mPlace;
    QString mFinCountry;
    QString mEngTown;
    QString mEngPlace;
    QString mEngCountry;
    QString mSweTown;
    QString mSwePlace;
    QString mSweCountry;
    QString mYKJCoordinate;
    QString mWGSCoordinate;
    bool mCustom;
    QString mOrganization;
    QString mOrgAbbrev;
};

#endif // LOCATION_H
