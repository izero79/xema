#include "birdmodel.h"
#include <QDebug>

BirdModel::BirdModel(QObject *parent) :
    QAbstractListModel(parent)
{
    roles[FilterRole] = "filter";
    roles[IndexRole] = "realindex";
    roles[IdRole] = "itemid";
    roles[FinNameRole] = "finname";
    roles[SweNameRole] = "swename";
    roles[AbbrevRole] = "abbrev";
    roles[LatinNameRole] = "latinname";
    roles[FinGroupRole] = "fingroup";
    roles[SweGroupRole] = "swegroup";
    roles[LatinGroupRole] = "latingroup";
    roles[CategoryRole] = "category";
    setRoleNames(roles);
}

int BirdModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return items.count();
}

void BirdModel::clear()
{
    QAbstractItemModel::beginRemoveRows(QModelIndex(), 0, items.count());
    items.clear();
    QAbstractItemModel::endRemoveRows();
}

QVariant BirdModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > items.count())
    {
        return QVariant();
    }

    Bird item = items[index.row()];

    if (role == FilterRole)
    {
        return QString(item.finName() + ", " +
                        item.sweName() + ", " +
                        item.latinName() + ", " +
                        item.abbreviation() + ", ");
    }
    else if (role == IndexRole)
    {
        return index.row();
    }
    else if (role == IdRole)
    {
        return item.id();
    }
    else if (role == FinNameRole)
    {
        return item.finName();
    }
    else if (role == SweNameRole)
    {
        return item.sweName();
    }
    else if (role == AbbrevRole)
    {
        return item.abbreviation();
    }
    else if (role == LatinNameRole)
    {
        return item.latinName();
    }
    else if (role == FinGroupRole)
    {
        return item.finGroup();
    }
    else if (role == SweGroupRole)
    {
        return item.sweGroup();
    }
    else if (role == LatinGroupRole)
    {
        return item.latinGroup();
    }
    else if (role == CategoryRole)
    {
        return item.category();
    }
    return QVariant();
}

bool BirdModel::removeRow (int row, const QModelIndex & parent)
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

bool BirdModel::removeRows (int row, int count, const QModelIndex & parent)
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


void BirdModel::addItem(const Bird &item)
{
    int index = items.size();
    QAbstractItemModel::beginInsertRows(QModelIndex(), index, index);
    items.append(item);
    QAbstractItemModel::endInsertRows();
}

Bird BirdModel::getItem(int row) const
{
    if (row < 0 || row >= items.count())
    {
        return Bird();
    }
    return items.at(row);
}

QList<Bird> BirdModel::content() const
{
    return items;
}

void BirdModel::setContent(const QList<Bird> &newItems)
{
    if (newItems.size() > 0)
    {
        QAbstractItemModel::beginInsertRows(QModelIndex(), 0, newItems.size() - 1);
        items = newItems;
        QAbstractItemModel::endInsertRows();
    }
}

bool BirdModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    Bird tmp;
    if (index.row() >= items.count() || index.row() < 0)
    {
        row = items.count();
        tmp.setId(row + 1);
    }
    else
    {
        tmp = items.at(row);
    }
    switch(role)
    {
    case FinNameRole:
        tmp.setFinName(value.toString());
        break;
    case SweNameRole:
        tmp.setSweName(value.toString());
        break;
    case LatinNameRole:
        tmp.setLatinName(value.toString());
        break;
    case FinGroupRole:
        tmp.setFinGroup(value.toString());
        break;
    case SweGroupRole:
        tmp.setSweGroup(value.toString());
        break;
    case LatinGroupRole:
        tmp.setLatinGroup(value.toString());
        break;
    case CategoryRole:
        tmp.setCategory(value.toString());
        break;
    case AbbrevRole:
        tmp.setAbbreviation(value.toString());
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

void BirdModel::replaceItem(int row, const Bird &item)
{
    items.replace(row, item);
    QModelIndex idx = index(row, 0);
    QAbstractItemModel::dataChanged(idx, idx);
}
