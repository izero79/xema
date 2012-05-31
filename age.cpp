#include "age.h"

Age::Age() {
}

Age::Age(const QString &value, const QString &finname, const QString &swename, const QString &engname) :
    mValue(value),
    mFinName(finname),
    mSweName(swename),
    mEngName(engname)
{

}

void Age::setFinName(const QString &name)
{
    mFinName = name;
}

void Age::setSweName(const QString &name)
{
    mSweName = name;
}

void Age::setEngName(const QString &name)
{
    mEngName = name;
}

void Age::setValue(const QString &value)
{
    mValue = value;
}

QString Age::finName() const
{
    return mFinName;
}

QString Age::sweName() const
{
    if (mSweName.isEmpty()) {
        return mFinName;
    }
    return mSweName;
}

QString Age::engName() const
{
    if (mEngName.isEmpty()) {
        return mFinName;
    }
    return mEngName;
}

QString Age::value() const
{
    return mValue;
}
