#ifndef ASSOCIATIONMODEL_H
#define ASSOCIATIONMODEL_H

#include <QAbstractListModel>

#include "association.h"

class AssociationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AssociationModel(QObject *parent = 0);

    enum Roles
    {
        FilterRole = Qt::UserRole + 1,
        IndexRole,
        AbbrevRole, // 35
        NameRole,
        SelectedRole = Qt::EditRole
    };

signals:

public slots:
    Q_INVOKABLE     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE     int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE     void clear();
    Q_INVOKABLE     void addItem(const Association &item);
    Q_INVOKABLE     bool removeRow (int row, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     Association getItem(int index) const;
    Q_INVOKABLE     bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

private:
    QList<Association> items;
    QHash<int, QByteArray> roles;

};

#endif // ASSOCIATIONMODEL_H
