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
    void setSpeciesAbbr(const QString &abbrev);
    void addSpecies(const QString &name);
    void addSpeciesAbbr(const QString &abbrev);
    void increaseDateCount();
    void increasePlaceCount();
    void addSpeciesCount(const QString &name, int count);
    void setTime(const QString &time);
    void setTiiraExported(bool exported);

    int id() const;
    QString place() const;
    QString date() const;
    QString species() const;
    QString speciesAbbr() const;
    int dateCount() const;
    int placeCount() const;
    int specieCount(const QString &name = QString()) const;
    QDate sortDate() const;
    QTime sortTime() const;
    QString sectionString() const;
    bool tiiraExported() const;

private:
    qlonglong mId;
    QString mPlace;
    QString mDate;
    QMap<QString,int> mSpecies;
    QString mSpeciesAbbr;
    int mDateCount;
    int mPlaceCount;
    QString mTime;
    bool mTiiraExported;
};

#endif // HISTORYITEM_H
