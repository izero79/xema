#include <QDebug>

#include "historyitem.h"

HistoryItem::HistoryItem() :
    mId( 0 ),
    mPlace(),
    mDate(),
    mSpecies(),
    mDateCount( 1 ),
    mPlaceCount( 1 )
{
}

HistoryItem::HistoryItem( qlonglong id, const QString &place, const QString &date ) :
    mId( id ),
    mPlace( place ),
    mDate( date ),
    mSpecies(),
    mDateCount( 1 ),
    mPlaceCount( 1 )
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

void HistoryItem::setSpecies( const QString &name )
{
    mSpecies.clear();
    mSpecies.insert( name, 0);
}

void HistoryItem::addSpecies( const QString &name )
{
    if( mSpecies.contains( name ) )
    {
        int oldCount = mSpecies.value(name);
//        oldCount++;
        mSpecies.insert(name, oldCount);
        return;
    }
    mSpecies.insert( name, 0 );
}

void HistoryItem::increaseDateCount()
{
    mDateCount++;
}

void HistoryItem::increasePlaceCount()
{
    mPlaceCount++;
    qDebug() << "increasePlaceCount, now" << mPlaceCount;
}

void HistoryItem::addSpeciesCount(const QString &name, int count)
{
    if(name.isEmpty() == false)
    {
        int oldCount = mSpecies.value(name);
        int newCount = oldCount + count;
        mSpecies.insert(name, newCount);
    }
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

QString HistoryItem::species() const
{
    QString species;

    QMap<QString,int> omalista = mSpecies;
    QMap<QString,int>::iterator it;
    for(it = omalista.begin(); it != omalista.end(); it++) {
        QString key = it.key();
        int value = it.value();
        if( it.value() > 1 )
        {
            QString num;
            num.setNum( value );
            species.append( key + " (" + num + ")" );
        }
        else
        {
            species.append( key );
        }
        species.append( ", ");
    }
    if( species.endsWith( ", "));
    {
        species.remove( species.length() - 2, 2 );
    }
    return species;
}

int HistoryItem::dateCount() const
{
    return mDateCount;
}

int HistoryItem::placeCount() const
{
    return mPlaceCount;
}

int HistoryItem::specieCount(const QString &name) const
{
    int count = 0;
    if(name.isEmpty()==true)
    {
        QMap<QString,int> omalista = mSpecies;
        QMap<QString,int>::iterator it;
        for(it = omalista.begin(); it != omalista.end(); it++) {
            QString key = it.key();
            int value = it.value();
            count += value;
        }
    }
    return count;
}
