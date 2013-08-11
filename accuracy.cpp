#include "accuracy.h"
#include "settings.h"

Accuracy::Accuracy() {
}

Accuracy::Accuracy(const QString &value, const QString &finname, const QString &swename, const QString &engname) :
    mValue(value),
    mFinName(finname),
    mSweName(swename),
    mEngName(engname)
{

}

void Accuracy::setFinName(const QString &name)
{
    mFinName = name;
}

void Accuracy::setSweName(const QString &name)
{
    mSweName = name;
}

void Accuracy::setEngName(const QString &name)
{
    mEngName = name;
}

void Accuracy::setValue(const QString &value)
{
    mValue = value;
}

QString Accuracy::localizedName() const
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

QString Accuracy::finName() const
{
    return mFinName;
}

QString Accuracy::sweName() const
{
    if (mSweName.isEmpty()) {
        return mFinName;
    }
    return mSweName;
}

QString Accuracy::engName() const
{
    if (mEngName.isEmpty()) {
        return mFinName;
    }
    return mEngName;
}

QString Accuracy::value() const
{
    return mValue;
}
