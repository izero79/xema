#include "status.h"

Status::Status() :
    mName(),
    mSweName(),
    mEngName(),
    mAbbrev(),
    mSelected(false)
{
}

Status::Status(const QString &name, const QString &abbrev, const QString &swename, const QString &engname) :
    mName(name),
    mSweName(swename),
    mEngName(engname),
    mAbbrev(abbrev),
    mSelected(false)
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


QString Status::name() const
{
    return mName;
}

QString Status::sweName() const
{
    if (mSweName.isEmpty()) {
        return name();
    }
    return mSweName;
}

QString Status::engName() const
{
    if (mEngName.isEmpty()) {
        return name();
    }
    return mEngName;
}

QString Status::abbreviation() const
{
    return mAbbrev;
}

bool Status::selected() const
{
    return mSelected;
}
