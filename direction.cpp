#include "direction.h"
#include "settings.h"

Direction::Direction() {
}

Direction::Direction(const QString &value, const QString &finname, const QString &swename, const QString &engname) :
    mValue(value),
    mFinName(finname),
    mSweName(swename),
    mEngName(engname)
{

}

void Direction::setFinName(const QString &name)
{
    mFinName = name;
}

void Direction::setSweName(const QString &name)
{
    mSweName = name;
}

void Direction::setEngName(const QString &name)
{
    mEngName = name;
}

void Direction::setValue(const QString &value)
{
    mValue = value;
}

QString Direction::localizedName() const
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

QString Direction::finName() const
{
    return mFinName;
}

QString Direction::sweName() const
{
    if (mSweName.isEmpty()) {
        return mFinName;
    }
    return mSweName;
}

QString Direction::engName() const
{
    if (mEngName.isEmpty()) {
        return mFinName;
    }
    return mEngName;
}

QString Direction::value() const
{
    return mValue;
}
