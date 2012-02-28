#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include <QString>
#include <QStringList>
#include <QMap>

class HistoryItem
{
public:
    HistoryItem();
    HistoryItem( qlonglong id, const QString &place, const QString &date );

    void setPlace( const QString &name );
    void setDate( const QString &date );
    void setSpecie( const QString &name );
    void addSpecie( const QString &name );
    void increaseDateCount();
    void increasePlaceCount();
    void addSpecieCount(const QString &name, int count);

    int id() const;
    QString place() const;
    QString date() const;
    QString species() const;
    int dateCount() const;
    int placeCount() const;
    int specieCount(const QString &name = QString()) const;

private:
    qlonglong mId;
    QString mPlace;
    QString mDate;
    QMap<QString,int> mSpecies;
    int mDateCount;
    int mPlaceCount;
};

#endif // HISTORYITEM_H
