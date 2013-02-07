#include "directionmodel.h"
#include <QDebug>

DirectionModel::DirectionModel(QObject *parent) :
    QAbstractListModel(parent)
{
    roles[FilterRole] = "filter";
    roles[IndexRole] = "realindex";
    roles[ValueRole] = "value";
    roles[FinNameRole] = "finname";
    roles[SweNameRole] = "swename";
    roles[EngNameRole] = "engname";
    setRoleNames(roles);
}

int DirectionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return items.count();
}

void DirectionModel::clear()
{
    QAbstractItemModel::beginRemoveRows(QModelIndex(), 0, items.count());
    items.clear();
    QAbstractItemModel::endRemoveRows();
}

QVariant DirectionModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > items.count())
    {
        return QVariant();
    }

    Direction item = items[index.row()];

    if (role == FilterRole)
    {
        return QString(item.value() + ", " + item.localizedName());
    }
    else if (role == IndexRole)
    {
        return index.row();
    }
    else if (role == FinNameRole)
    {
        return item.finName();
    }
    else if (role == SweNameRole)
    {
        return item.sweName();
    }
    else if (role == EngNameRole)
    {
        return item.engName();
    }
    else if (role == ValueRole)
    {
        return item.value();
    }
    return QVariant();
}

bool DirectionModel::removeRow (int row, const QModelIndex & parent)
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

bool DirectionModel::removeRows (int row, int count, const QModelIndex & parent)
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


void DirectionModel::addItem(const Direction &item)
{
    int index = items.size();
    QAbstractItemModel::beginInsertRows(QModelIndex(), index, index);
    items.append(item);
    QAbstractItemModel::endInsertRows();
}

Direction DirectionModel::getItem(int row) const
{
    if (row < 0 || row >= items.count())
    {
        return Direction();
    }
    return items.at(row);
}
