#include "location.h"

Location::Location() :
    mTown(),
    mPlace(),
    mYKJCoordinate(),
    mWGSCoordinate()
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
