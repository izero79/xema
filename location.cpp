#include "location.h"
#include "settings.h"
#include <QDebug>

Location::Location() :
    mCustom(false)
{
}

Location::Location(const QString &town, const QString &place, const QString &wgsCoordinate, const QString &ykjCoordinate) :
    mTown(town),
    mPlace(place),
    mFinCountry(),
    mEngCountry(),
    mSweCountry(),
    mYKJCoordinate(ykjCoordinate),
    mWGSCoordinate(wgsCoordinate),
    mCustom(false)
{

}

void Location::setTown(const QString &name)
{
    mTown = name;
}

void Location::setPlace(const QString &name)
{
    mPlace = name;
}

void Location::setFinCountry(const QString &country)
{
    mFinCountry = country;
}

void Location::setEngTown(const QString &name)
{
    mEngTown = name;
}

void Location::setEngPlace(const QString &name)
{
    mEngPlace = name;
}

void Location::setEngCountry(const QString &country)
{
    mEngCountry = country;
}

void Location::setSweTown(const QString &name)
{
    mSweTown = name;
}

void Location::setSwePlace(const QString &name)
{
    mSwePlace = name;
}

void Location::setSweCountry(const QString &country)
{
    mSweCountry = country;
}

void Location::setYKJCoordinate(const QString &ykj)
{
    mYKJCoordinate = ykj;
}

void Location::setWGSCoordinate(const QString &wgs)
{
    mWGSCoordinate = wgs;
}

void Location::setCustom(bool custom)
{
    mCustom = custom;
}

QString Location::localizedTown() const
{
    QString lang = Settings::lang();
    if (lang == "sv") {
        return sweTown();
    } else if (lang == "en") {
        return engTown();
    }
    else
        return town();

}

QString Location::localizedPlace() const
{
    QString lang = Settings::lang();
    if (lang == "sv") {
        return swePlace();
    } else if (lang == "en") {
        return engPlace();
    }
    else
        return place();
}

QString Location::localizedCountry() const
{
    QString lang = Settings::lang();
    if (lang == "sv") {
        return sweCountry();
    } else if (lang == "en") {
        return engCountry();
    }
    else
        return finCountry();
}

QString Location::town() const
{
    return mTown;
}

QString Location::place() const
{
    return mPlace;
}

QString Location::finCountry() const
{
    return mFinCountry;
}

QString Location::engTown(bool allowEmpty) const
{
    if (mEngTown.isEmpty() == false) {
        return mEngTown;
    }
    else if (allowEmpty) {
        return mEngTown;
    }
    return mTown;
}

QString Location::engPlace(bool allowEmpty) const
{
    if (mEngPlace.isEmpty() == false) {
        return mEngPlace;
    }
    else if (allowEmpty) {
        return mEngPlace;
    }
    return mPlace;
}

QString Location::engCountry(bool allowEmpty) const
{
    if (mEngCountry.isEmpty() == false) {
        return mEngCountry;
    }
    else if (allowEmpty) {
        return mEngCountry;
    }
    return mFinCountry;
}

QString Location::sweTown(bool allowEmpty) const
{
    if (mSweTown.isEmpty() == false) {
        return mSweTown;
    }
    else if (allowEmpty) {
        return mSweTown;
    }
    return mTown;
}

QString Location::swePlace(bool allowEmpty) const
{
    if (mSwePlace.isEmpty() == false) {
        return mSwePlace;
    }
    else if (allowEmpty) {
        return mSwePlace;
    }
    return mPlace;
}

QString Location::sweCountry(bool allowEmpty) const
{
    if (mSweCountry.isEmpty() == false) {
        return mSweCountry;
    }
    else if (allowEmpty) {
        return mSweCountry;
    }
    return mFinCountry;
}

QString Location::ykjCoordinate() const
{
    return mYKJCoordinate;
}

QString Location::wgsCoordinate() const
{
    return mWGSCoordinate;
}

QString Location::wgsCoordinateForFilter() const
{
    if(mWGSCoordinate.isEmpty() == false)
    {
        QString latitude = mWGSCoordinate.section(':',0, 0);
        QString longitude = mWGSCoordinate.section(':',1, 1);
        latitude = latitude.left(4); // TODO korjaa tama
        longitude = longitude.left(4);
        return QString(latitude+":"+longitude);
    }
    return mWGSCoordinate;
}

QString Location::coordinate() const
{
    if (mYKJCoordinate.isEmpty() == true)
    {
        if (mWGSCoordinate.isEmpty() == false)
        {
            return mWGSCoordinate;
        }
        else
        {
            return QString();
        }
    }
    return mYKJCoordinate;
}

bool Location::custom() const
{
    return mCustom;
}
