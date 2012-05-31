#ifndef AGEMODEL_H
#define AGEMODEL_H

#include <QAbstractListModel>

#include "age.h"

class AgeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AgeModel(QObject *parent = 0);

    enum Roles
    {
        FilterRole = Qt::UserRole + 1,
        IndexRole,
        ValueRole, // 35
        FinNameRole,
        SweNameRole,
        EngNameRole // 38
    };

signals:

public slots:
    Q_INVOKABLE     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE     int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE     void clear();
    Q_INVOKABLE     void addItem(const Age &item);
    Q_INVOKABLE     bool removeRow (int row, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     Age getItem(int index) const;

private:
    QList<Age> items;
    QHash<int, QByteArray> roles;

};

#endif // AGEMODEL_H
