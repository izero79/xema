#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include <QString>

class HistoryItem
{
public:
    HistoryItem();
    HistoryItem( qlonglong id, const QString &place, const QString &date );

    void setPlace( const QString &name );
    void setDate( const QString &date );

    int id() const;
    QString place() const;
    QString date() const;

private:
    qlonglong mId;
    QString mPlace;
    QString mDate;
};

#endif // HISTORYITEM_H
