#include "dress.h"
#include "settings.h"

#include <QDebug>

Dress::Dress() {
}

Dress::Dress(const QString &value, const QString &finname, const QString &swename, const QString &engname) :
    mValue(value),
    mFinName(finname),
    mSweName(swename),
    mEngName(engname)
{

}

void Dress::setFinName(const QString &name)
{
    mFinName = name;
}

void Dress::setSweName(const QString &name)
{
    mSweName = name;
}

void Dress::setEngName(const QString &name)
{
    mEngName = name;
}

void Dress::setValue(const QString &value)
{
    mValue = value;
}

QString Dress::localizedName() const
{
    QString lang = Settings::lang();
    if (lang == "sv") {
        return sweName();
    } else if (lang == "en") {
        return engName();
    }
    else
        return finName();

}


QString Dress::finName() const
{
    return mFinName;
}

QString Dress::sweName() const
{
    if (mSweName.isEmpty()) {
        return mFinName;
    }
    return mSweName;
}

QString Dress::engName() const
{
    if (mEngName.isEmpty()) {
        return mFinName;
    }
    return mEngName;
}

QString Dress::value() const
{
    return mValue;
}
