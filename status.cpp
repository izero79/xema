#include "status.h"
#include "settings.h"

Status::Status() :
    mName(),
    mSweName(),
    mEngName(),
    mAbbrev(),
    mSelected(false),
    mCustom(false)
{
}

Status::Status(const QString &name, const QString &abbrev, const QString &swename, const QString &engname) :
    mName(name),
    mSweName(swename),
    mEngName(engname),
    mAbbrev(abbrev),
    mSelected(false),
    mCustom(false)
{

}

void Status::setName(const QString &name)
{
    mName = name;
}

void Status::setSweName(const QString &name)
{
    mSweName = name;
}

void Status::setEngName(const QString &name)
{
    mEngName = name;
}

void Status::setAbbrev(const QString &abbr)
{
    mAbbrev = abbr;
}

void Status::setSelected(bool selected)
{
    mSelected = selected;
}

void Status::setCustom(bool custom)
{
    mCustom = custom;
}

QString Status::localizedName() const
{
    QString lang = Settings::lang();
    if (lang == "sv") {
        return sweName();
    } else if (lang == "en") {
        return engName();
    }
    else
        return name();

}

QString Status::name() const
{
    return mName;
}

QString Status::sweName(bool allowEmpty) const
{
    if (mSweName.isEmpty() == false) {
        return mSweName;
    }
    else if (allowEmpty) {
        return mSweName;
    }
    return mName;
}

QString Status::engName(bool allowEmpty) const
{
    if (mEngName.isEmpty() == false) {
        return mEngName;
    }
    else if (allowEmpty) {
        return mEngName;
    }
    return mName;
}

QString Status::abbreviation() const
{
    return mAbbrev;
}

bool Status::selected() const
{
    return mSelected;
}

bool Status::custom() const
{
    return mCustom;
}
