#include "status.h"

Status::Status() :
    mName(),
    mAbbrev(),
    mSelected( false )
{
}

Status::Status( const QString &name, const QString &abbrev ) :
    mName( name ),
    mAbbrev( abbrev ),
    mSelected( false )
{

}

void Status::setName( const QString &name )
{
    mName = name;
}

void Status::setAbbrev( const QString &abbr )
{
    mAbbrev = abbr;
}

void Status::setSelected( bool selected )
{
    mSelected = selected;
}


QString Status::name() const
{
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
