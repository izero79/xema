#ifndef STATUSMODEL_H
#define STATUSMODEL_H

#include <QAbstractListModel>

#include "status.h"

class StatusModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit StatusModel(QObject *parent = 0);

    enum Roles {
        FilterRole = Qt::UserRole + 1,
        IndexRole,
        NameRole,
        AbbrevRole,
        SelectedRole = Qt::EditRole
    };

signals:

public slots:
    Q_INVOKABLE     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE     int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE     void clear();
    Q_INVOKABLE     void addItem(const Status &item);
    Q_INVOKABLE     bool removeRow (int row, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     Status getItem(int index);
    Q_INVOKABLE     bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

                    void setContent(const QList<Status> &newItems);
                    QList<Status> content() const;

private:
    QList<Status> items;
    QHash<int, QByteArray> roles;
};

#endif // STATUSMODEL_H
