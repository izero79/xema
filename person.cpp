#include "person.h"

Person::Person() :
    mFirstName(),
    mSurName(),
    mRegistered( false )
{
}

Person::Person( const QString &firstName, const QString &surName, bool registered ) :
    mFirstName( firstName),
    mSurName( surName),
    mRegistered( registered )
{

}


void Person::setFirstName( const QString &name )
{
    mFirstName = name;
}

void Person::setSurName( const QString &name )
{
    mSurName = name;
}

void Person::setRegistered( bool registered )
{
    mRegistered = registered;
}


QString Person::firstName() const
{
    return mFirstName;
}

QString Person::surName() const
{
    return mSurName;
}

QString Person::name() const
{
    return QString( mFirstName + " " + mSurName );
}

bool Person::registered() const
{
    return mRegistered;
}

