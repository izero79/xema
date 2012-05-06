#ifndef ATLASINDEXMODEL_H
#define ATLASINDEXMODEL_H

#include <QAbstractListModel>

#include "atlasindex.h"

class AtlasIndexModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AtlasIndexModel(QObject *parent = 0);
    
    enum Roles
    {
        FilterRole = Qt::UserRole + 1,
        IndexRole,
        ValueRole, // 35
        FinNameRole,
        SweNameRole,
        EngNameRole
    };

signals:
    
public slots:
    Q_INVOKABLE     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE     int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE     void clear();
    Q_INVOKABLE     void addItem(const AtlasIndex &item);
    Q_INVOKABLE     bool removeRow (int row, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     AtlasIndex getItem(int index) const;
    Q_INVOKABLE     bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

                    void setContent(const QList<AtlasIndex> &newItems);
                    QList<AtlasIndex> content() const;

private:
    QList<AtlasIndex> items;
    QHash<int, QByteArray> roles;

};

#endif // ATLASINDEXMODEL_H
