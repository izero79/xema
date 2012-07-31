#include <QDebug>

#include "personmodel.h"

PersonModel::PersonModel(QObject *parent) :
    QAbstractListModel(parent)
{
    roles[FilterRole] = "filter";
    roles[IndexRole] = "realindex";
    roles[NameRole] = "name";
    roles[RegisteredRole] = "registered";
    roles[DefaultRole] = "default";
    roles[FirstNameRole] = "firstname";
    roles[SurNameRole] = "surname";
    roles[SelectedRole] = "selected";
    setRoleNames(roles);
}

int PersonModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return items.count();
}

void PersonModel::clear()
{
    QAbstractItemModel::beginRemoveRows(QModelIndex(), 0, items.count());
    items.clear();
    QAbstractItemModel::endRemoveRows();
}

QVariant PersonModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > items.count())
    {
        return QVariant();
    }

    Person item = items[index.row()];

    if (role == FilterRole)
    {
        return item.name();
    }
    else if (role == IndexRole)
    {
        return index.row();
    }
    else if (role == NameRole)
    {
        return item.name();
    }
    else if (role == RegisteredRole)
    {
        return item.registered();
    }
    else if (role == DefaultRole)
    {
        return item.defaultName();
    }
    else if (role == FirstNameRole)
    {
        return item.firstName();
    }
    else if (role == SurNameRole)
    {
        return item.surName();
    }
    else if (role == SelectedRole)
    {
        return item.selected();
    }
    else if (role == SortingNameRole)
    {
        return item.surName() + ", " + item.firstName();
    }
    return QVariant();
}

bool PersonModel::removeRow (int row, const QModelIndex & parent)
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

bool PersonModel::removeRows (int row, int count, const QModelIndex & parent)
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


void PersonModel::addItem(const Person &item)
{
    int index = items.size();
    QAbstractItemModel::beginInsertRows(QModelIndex(), index, index);
    items.append(item);
    QAbstractItemModel::endInsertRows();
}

Person PersonModel::getItem(int row) const
{
    if (row < 0 || row >= items.count())
    {
        return Person();
    }
    return items.at(row);
}

QList<Person> PersonModel::content() const
{
    return items;
}

void PersonModel::setContent(const QList<Person> &newItems)
{
    if (newItems.size() > 0)
    {
        QAbstractItemModel::beginInsertRows(QModelIndex(), 0, newItems.size() - 1);
        items = newItems;
        QAbstractItemModel::endInsertRows();
    }
}

bool PersonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    Person tmp;
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
    case RegisteredRole:
        tmp.setRegistered(value.toBool());
        break;
    case DefaultRole:
        tmp.setDefault(value.toBool());
        break;
    case FirstNameRole:
        tmp.setFirstName(value.toString());
        break;
    case SurNameRole:
        tmp.setSurName(value.toString());
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

void PersonModel::replaceItem(int row, const Person &item)
{
    items.replace(row, item);
    QModelIndex idx = index(row, 0);
    QAbstractItemModel::dataChanged(idx, idx);
}
