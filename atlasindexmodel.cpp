#include "atlasindexmodel.h"
#include <QDebug>

AtlasIndexModel::AtlasIndexModel(QObject *parent) :
    QAbstractListModel(parent)
{
    roles[FilterRole] = "filter";
    roles[IndexRole] = "realindex";
    roles[ValueRole] = "name";
    setRoleNames(roles);
}

int AtlasIndexModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return items.count();
}

void AtlasIndexModel::clear()
{
    QAbstractItemModel::beginRemoveRows(QModelIndex(), 0, items.count());
    items.clear();
    QAbstractItemModel::endRemoveRows();
}

QVariant AtlasIndexModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > items.count())
    {
        return QVariant();
    }

    AtlasIndex item = items[index.row()];

    if (role == FilterRole)
    {
        return QString(item.index());
    }
    else if (role == IndexRole)
    {
        return index.row();
    }
    else if (role == ValueRole)
    {
        if (item.index() == 0)
        {
            return "";
        }
        return item.index();
    }
    return QVariant();
}

bool AtlasIndexModel::removeRow (int row, const QModelIndex & parent)
{
    Q_UNUSED(parent)
    if (row < 0 || row >= items.count())
    {
        return false;
    }
    QAbstractItemModel::beginRemoveRows(QModelIndex(), row, row);
    items.removeAt(row);
    QAbstractItemModel::endRemoveRows();
    return true;
}

bool AtlasIndexModel::removeRows (int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent)
    if (row < 0 || row >= items.count() || (count - 1) + row > items.count())
    {
        return false;
    }
    QAbstractItemModel::beginRemoveRows(QModelIndex(), row, row + (count - 1));
    for(int i = row; i < row + count; i++)
    {
        items.removeAt(row);
    }
    QAbstractItemModel::endRemoveRows();
    return true;
}


void AtlasIndexModel::addItem(const AtlasIndex &item)
{
    int index = items.size();
    QAbstractItemModel::beginInsertRows(QModelIndex(), index, index);
    items.append(item);
    QAbstractItemModel::endInsertRows();
}

AtlasIndex AtlasIndexModel::getItem(int row)
{
    if (row < 0 || row >= items.count())
    {
        return AtlasIndex();
    }
    return items.at(row);
}

QList<AtlasIndex> AtlasIndexModel::content() const
{
    return items;
}

void AtlasIndexModel::setContent(const QList<AtlasIndex> &newItems)
{
    if (newItems.size() > 0)
    {
        QAbstractItemModel::beginInsertRows(QModelIndex(), 0, newItems.size() - 1);
        items = newItems;
        QAbstractItemModel::endInsertRows();
    }
}

bool AtlasIndexModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    AtlasIndex tmp;
    if (index.row() >= items.count() || index.row() < 0)
    {
        row = items.count();
    }
    else
    {
        tmp = items.at(row);
    }
    switch(role)
    {
    case ValueRole:
        tmp.setIndex(value.toInt());
        break;
    default:
        break;
    }

    if (row >= items.count())
    {
        QAbstractItemModel::beginInsertRows(QModelIndex(), items.count(), items.count());
        items.append(tmp);
        QAbstractItemModel::endInsertRows();
    }
    else
    {
        items.replace(index.row(), tmp);
    }
    QAbstractItemModel::dataChanged(index, index);
    return true;
}
