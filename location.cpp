#include "location.h"
#include <QDebug>

Location::Location()
{
}

Location::Location(const QString &town, const QString &place, const QString &wgsCoordinate, const QString &ykjCoordinate) :
    mTown(town),
    mPlace(place),
    mYKJCoordinate(ykjCoordinate),
    mWGSCoordinate(wgsCoordinate)
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

void Location::setEngTown(const QString &name)
{
    mEngTown = name;
}

void Location::setEngPlace(const QString &name)
{
    mEngPlace = name;
}

void Location::setSweTown(const QString &name)
{
    mSweTown = name;
}

void Location::setSwePlace(const QString &name)
{
    mSwePlace = name;
}

void Location::setYKJCoordinate(const QString &ykj)
{
    mYKJCoordinate = ykj;
}

void Location::setWGSCoordinate(const QString &wgs)
{
    mWGSCoordinate = wgs;
}

QString Location::town() const
{
    return mTown;
}

QString Location::place() const
{
    return mPlace;
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
        latitude = latitude.left(4);
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
