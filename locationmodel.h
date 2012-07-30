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
        SweTownRole,
        SwePlaceRole,
        EngTownRole,
        EngPlaceRole, // 43
        SweTownOnlyRole,
        SwePlaceOnlyRole, // 45
        EngTownOnlyRole,
        EngPlaceOnlyRole, // 47
        IsCustomRole,
        FullStringRole
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
