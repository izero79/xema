#ifndef SEXMODEL_H
#define SEXMODEL_H

#include <QAbstractListModel>

#include "sex.h"

class SexModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SexModel(QObject *parent = 0);

    enum Roles
    {
        FilterRole = Qt::UserRole + 1,
        IndexRole,
        FinNameRole, // 35
        SweNameRole,
        EngNameRole, // 37
        ValueRole
    };

signals:

public slots:
    Q_INVOKABLE     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE     int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE     void clear();
    Q_INVOKABLE     void addItem(const Sex &item);
    Q_INVOKABLE     bool removeRow (int row, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     Sex getItem(int index) const;

private:
    QList<Sex> items;
    QHash<int, QByteArray> roles;

};

#endif // SEXMODEL_H
