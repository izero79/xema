#include "bird.h"

Bird::Bird() :
    mId( 0 ),
    mFinGroup(),
    mSweGroup(),
    mLatinGroup(),
    mFinName(),
    mSweName(),
    mLatinName(),
    mAbbreviation(),
    mCategory()
{
}

Bird::Bird( int id,  const QString &finGroup, const QString &sweGroup, const QString &latinGroup, const QString &finName, const QString &sweName, const QString &latin, const QString &abbr, const QString &category )  :
    mId( id ),
    mFinGroup( finGroup ),
    mSweGroup( sweGroup ),
    mLatinGroup( latinGroup ),
    mFinName( finName ),
    mSweName( sweName ),
    mLatinName( latin ),
    mAbbreviation( abbr ),
    mCategory( category )
{

}

void Bird::setId( int id )
{
    mId = id;
}

void Bird::setFinGroup( const QString &group )
{
    mFinGroup = group;
}

void Bird::setSweGroup( const QString &group )
{
    mSweGroup = group;
}

void Bird::setLatinGroup( const QString &group )
{
    mLatinGroup = group;
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

QString Bird::finGroup() const
{
    return mFinGroup;
}

QString Bird::sweGroup() const
{
    return mSweGroup;
}

QString Bird::latinGroup() const
{
    return mLatinGroup;
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
