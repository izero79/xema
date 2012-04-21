#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QDateTime>

class HistoryItem
{
public:
    HistoryItem();
    HistoryItem(qlonglong id, const QString &place, const QString &date);

    void setPlace(const QString &name);
    void setDate(const QString &date);
    void setSpecies(const QString &name);
    void addSpecies(const QString &name);
    void increaseDateCount();
    void increasePlaceCount();
    void addSpeciesCount(const QString &name, int count);
    void setTime(const QString &time);

    int id() const;
    QString place() const;
    QString date() const;
    QString species() const;
    int dateCount() const;
    int placeCount() const;
    int specieCount(const QString &name = QString()) const;
    QDate sortDate() const;
    QTime sortTime() const;

private:
    qlonglong mId;
    QString mPlace;
    QString mDate;
    QMap<QString,int> mSpecies;
    int mDateCount;
    int mPlaceCount;
    QString mTime;
};

#endif // HISTORYITEM_H
