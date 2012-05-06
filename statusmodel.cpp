#include <QDebug>

#include "statusmodel.h"

StatusModel::StatusModel(QObject *parent) :
    QAbstractListModel(parent)
{
    roles[FilterRole] = "filter";
    roles[IndexRole] = "realindex";
    roles[FinNameRole] = "finname";
    roles[SweNameRole] = "swename";
    roles[EngNameRole] = "engname";
    roles[AbbrevRole] = "abbrev";
    roles[SelectedRole] = "selected";
    setRoleNames(roles);
}

int StatusModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return items.count();
}

void StatusModel::clear()
{
    QAbstractItemModel::beginRemoveRows(QModelIndex(), 0, items.count());
    items.clear();
    QAbstractItemModel::endRemoveRows();
}

QVariant StatusModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > items.count())
    {
        return QVariant();
    }

    Status item = items[index.row()];

    if (role == FilterRole)
    {
        return QString(item.name() + ", " + item.abbreviation());
    }
    else if (role == IndexRole)
    {
        return index.row();
    }
    else if (role == FinNameRole)
    {
        return item.name();
    }
    else if (role == SweNameRole)
    {
        return item.sweName();
    }
    else if (role == EngNameRole)
    {
        return item.engName();
    }
    else if (role == AbbrevRole)
    {
        return item.abbreviation();
    }
    else if (role == SelectedRole)
    {
        return item.selected();
    }
    return QVariant();
}

bool StatusModel::removeRow (int row, const QModelIndex & parent)
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

bool StatusModel::removeRows (int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent)
    if (row < 0 || row >= items.count() || count + row > items.count())
    {
        return false;
    }
    QAbstractItemModel::beginRemoveRows(QModelIndex(), row, row + count);
    for(int i = row; i < row + count; i++)
    {
        items.removeAt(row);
    }
    QAbstractItemModel::endRemoveRows();
    return true;
}


void StatusModel::addItem(const Status &item)
{
    int index = items.size();
    QAbstractItemModel::beginInsertRows(QModelIndex(), index, index);
    items.append(item);
    QAbstractItemModel::endInsertRows();
}

Status StatusModel::getItem(int row)
{
    if (row < 0 || row >= items.count())
    {
        return Status();
    }
    return items.at(row);
}

QList<Status> StatusModel::content() const
{
    return items;
}

void StatusModel::setContent(const QList<Status> &newItems)
{
    if (newItems.size() > 0)
    {
        QAbstractItemModel::beginInsertRows(QModelIndex(), 0, newItems.size() - 1);
        items = newItems;
        QAbstractItemModel::endInsertRows();
    }
}

bool StatusModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != SelectedRole)
    {
        return false;
    }
    Status tmp = items.at(index.row());
    tmp.setSelected(value.toBool());
    items.replace(index.row(), tmp);
    QAbstractItemModel::dataChanged(index, index);
    return true;
}
