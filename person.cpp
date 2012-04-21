#include "person.h"

Person::Person() :
    mFirstName(),
    mSurName(),
    mRegistered(false),
    mDefault(false),
    mSelected(false)
{
}

Person::Person(const QString &firstName, const QString &surName, bool registered, bool defaultName) :
    mFirstName(firstName),
    mSurName(surName),
    mRegistered(registered),
    mDefault(defaultName),
    mSelected(false)
{

}


void Person::setFirstName(const QString &name)
{
    mFirstName = name;
}

void Person::setSurName(const QString &name)
{
    mSurName = name;
}

void Person::setRegistered(bool registered)
{
    mRegistered = registered;
}

void Person::setDefault(bool defaultName)
{
    mDefault = defaultName;
}

void Person::setSelected(bool selected)
{
    mSelected = selected;
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
    return QString(mFirstName + " " + mSurName);
}

bool Person::registered() const
{
    return mRegistered;
}

bool Person::defaultName() const
{
    return mDefault;
}

bool Person::selected() const
{
    return mSelected;
}
