#include "accuracymodel.h"
#include <QDebug>

AccuracyModel::AccuracyModel(QObject *parent) :
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

int AccuracyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return items.count();
}

void AccuracyModel::clear()
{
    QAbstractItemModel::beginRemoveRows(QModelIndex(), 0, items.count());
    items.clear();
    QAbstractItemModel::endRemoveRows();
}

QVariant AccuracyModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > items.count())
    {
        return QVariant();
    }

    Accuracy item = items[index.row()];

    if (role == FilterRole)
    {
        return QString(item.localizedName());
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

bool AccuracyModel::removeRow (int row, const QModelIndex & parent)
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

bool AccuracyModel::removeRows (int row, int count, const QModelIndex & parent)
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


void AccuracyModel::addItem(const Accuracy &item)
{
    int index = items.size();
    QAbstractItemModel::beginInsertRows(QModelIndex(), index, index);
    items.append(item);
    QAbstractItemModel::endInsertRows();
}

Accuracy AccuracyModel::getItem(int row) const
{
    if (row < 0 || row >= items.count())
    {
        return Accuracy();
    }
    return items.at(row);
}
