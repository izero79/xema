#ifndef LOCATIONMODEL_H
#define LOCATIONMODEL_H

#include <QAbstractListModel>

#include "location.h"

class LocationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit LocationModel(QObject *parent = 0);

    enum Roles
    {
        FilterRole = Qt::UserRole + 1,
        IndexRole,
        TownRole, // 35
        PlaceRole,
        CoordinateRole,
        YkjCoordinateRole,
        WgsCoordinateRole, // 39
        SweTownRole, // 40
        SwePlaceRole,
        EngTownRole,
        EngPlaceRole, // 43
        SweTownOnlyRole,
        SwePlaceOnlyRole, // 45
        EngTownOnlyRole,
        EngPlaceOnlyRole, // 47
        IsCustomRole,
        FullStringRole,
        LocalizedTownRole, // 50
        LocalizedPlaceRole,
        CountryRole,
        SweCountryRole,
        EngCountryRole,
        LocalizedCountryRole, // 55
        SweCountryOnlyRole,
        EngCountryOnlyRole,
        OrganizationRole, // 58
        OrgAbbrevRole, // 59
        SortingNameRole = Qt::UserRole + 100
    };

signals:

public slots:
    Q_INVOKABLE     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE     int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE     void clear();
    Q_INVOKABLE     void addItem(const Location &item);
    Q_INVOKABLE     bool removeRow (int row, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     Location getItem(int index) const;
    Q_INVOKABLE     bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Q_INVOKABLE     void replaceItem(int row, const Location &item);


                    void setContent(const QList<Location> &newItems);
                    QList<Location> content() const;

private:
    QList<Location> items;
    QHash<int, QByteArray> roles;

};

#endif // LOCATIONMODEL_H
