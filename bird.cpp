#include "bird.h"

Bird::Bird() :
    mId( 0 ),
    mFinName(),
    mSweName(),
    mAbbreviation(),
    mLatinName(),
    mCategory()
{
}

Bird::Bird( int id, const QString &finName, const QString &sweName, const QString &abbr, const QString &latin, const QString &category )  :
    mId( id ),
    mFinName( finName ),
    mSweName( sweName ),
    mAbbreviation( abbr ),
    mLatinName( latin ),
    mCategory( category )
{

}

void Bird::setId( int id )
{
    mId = id;
}

void Bird::setFinName( const QString &name )
{
    mFinName = name;
}

void Bird::setSweName( const QString &name )
{
    mSweName = name;
}

void Bird::setAbbreviation( const QString &abbr )
{
    mAbbreviation = abbr;
}

void Bird::setLatinName( const QString &name )
{
    mLatinName = name;
}

void Bird::setCategory( const QString &category )
{
    mCategory = category;
}

int Bird::id() const
{
    return mId;
}

QString Bird::finName() const
{
    return mFinName;
}

QString Bird::sweName() const
{
    return mSweName;
}

QString Bird::abbreviation() const
{
    return mAbbreviation;
}

QString Bird::latinName() const
{
    return mLatinName;
}

QString Bird::category() const
{
    return mCategory;
}
