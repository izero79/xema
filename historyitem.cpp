#include "historyitem.h"

HistoryItem::HistoryItem() :
    mId( 0 ),
    mPlace(),
    mDate()
{
}

HistoryItem::HistoryItem( qlonglong id, const QString &place, const QString &date ) :
    mId( id ),
    mPlace( place ),
    mDate( date )
{

}

void HistoryItem::setPlace( const QString &name )
{
    mPlace = name;
}

void HistoryItem::setDate( const QString &date )
{
    mDate = date;
}


int HistoryItem::id() const
{
    return mId;
}

QString HistoryItem::place() const
{
    return mPlace;
}

QString HistoryItem::date() const
{
    return mDate;
}
