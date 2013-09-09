#include "associationmodel.h"
#include <QDebug>

AssociationModel::AssociationModel(QObject *parent) :
    QAbstractListModel(parent)
{
    roles[FilterRole] = "filter";
    roles[IndexRole] = "realindex";
    roles[AbbrevRole] = "abbrev";
    roles[NameRole] = "name";
    roles[SelectedRole] = "selected";
    setRoleNames(roles);
}

int AssociationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return items.count();
}

void AssociationModel::clear()
{
    QAbstractItemModel::beginRemoveRows(QModelIndex(), 0, items.count());
    items.clear();
    QAbstractItemModel::endRemoveRows();
}

QVariant AssociationModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > items.count())
    {
        return QVariant();
    }

    Association item = items[index.row()];

    if (role == FilterRole)
    {
        return QString(item.abbrev() + ", " + item.name());
    }
    else if (role == IndexRole)
    {
        return index.row();
    }
    else if (role == NameRole)
    {
        return item.name();
    }
    else if (role == AbbrevRole)
    {
        return item.abbrev();
    }
    else if (role == SelectedRole)
    {
        return item.selected();
    }
    return QVariant();
}

bool AssociationModel::removeRow (int row, const QModelIndex & parent)
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

bool AssociationModel::removeRows (int row, int count, const QModelIndex & parent)
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


void AssociationModel::addItem(const Association &item)
{
    int index = items.size();
    QAbstractItemModel::beginInsertRows(QModelIndex(), index, index);
    items.append(item);
    QAbstractItemModel::endInsertRows();
}

Association AssociationModel::getItem(int row) const
{
    if (row < 0 || row >= items.count())
    {
        return Association();
    }
    return items.at(row);
}


bool AssociationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    Association tmp;
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
    case NameRole:
        tmp.setName(value.toString());
        break;
    case AbbrevRole:
        tmp.setAbbrev(value.toString());
        break;
    case SelectedRole:
        tmp.setSelected(value.toBool());
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
