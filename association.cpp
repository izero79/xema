#include "association.h"
#include "settings.h"

Association::Association() :
    mSelected(false)
{

}

Association::Association(const QString &abbrev, const QString &name) :
    mAbbrev(abbrev),
    mName(name),
    mSelected(false)
{

}

void Association::setName(const QString &name)
{
    mName = name;
}

void Association::setAbbrev(const QString &abbrev)
{
    mAbbrev = abbrev;
}

void Association::setSelected(bool selected)
{
    mSelected = selected;
}

QString Association::name() const
{
    return mName;
}

QString Association::abbrev() const
{
    return mAbbrev;
}

bool Association::selected() const
{
    return mSelected;
}
