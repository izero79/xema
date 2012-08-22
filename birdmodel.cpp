#include "birdmodel.h"
#include "settings.h"

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
    roles[EngNameRole] = "engname";
    roles[EngGroupRole] = "enggroup";
    roles[SweNameOnlyRole] = "sweonlyname";
    roles[SweGroupOnlyRole] = "sweonlygroup";
    roles[EngNameOnlyRole] = "engonlyname";
    roles[EngGroupOnlyRole] = "engonlygroup";
    roles[SectionRole] = "section";
    roles[LocalizedNameRole] = "localizedName";
    roles[LocalizedGroupRole] = "localizedGroup";
    roles[EmptyRole1] = "value";
    roles[EmptyRole2] = "name";
    roles[EmptyRole3] = "selected";
    roles[IsCustomRole] = "custom";
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
        return QString(item.localizedName() + ", " +
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
    else if (role == LocalizedNameRole)
    {
        return item.localizedName();
    }
    else if (role == LocalizedGroupRole)
    {
        return item.localizedGroup();
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
    else if (role == SweNameOnlyRole)
    {
        return item.sweName(true);
    }
    else if (role == EngNameOnlyRole)
    {
        return item.engName(true);
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
    else if (role == EngGroupRole)
    {
        return item.engGroup();
    }
    else if (role == SweGroupOnlyRole)
    {
        return item.sweGroup(true);
    }
    else if (role == EngGroupOnlyRole)
    {
        return item.engGroup(true);
    }
    else if (role == LatinGroupRole)
    {
        return item.latinGroup();
    }
    else if (role == CategoryRole)
    {
        return item.category();
    }
    else if (role == SectionRole)
    {
        return item.finGroup();
    }
    else if (role == IsCustomRole)
    {
        return item.custom();
    }
    else if (role == SortingNameRole)
    {
        QString lang = Settings::lang();
        if (lang == "en") {
            return item.engName(false);
        }
        else if (lang == "sv") {
            return item.sweName(false);
        }
        else {
            return item.finName();
        }
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
    case SweNameOnlyRole:
        tmp.setSweName(value.toString());
        break;
    case SweGroupRole:
    case SweGroupOnlyRole:
        tmp.setSweGroup(value.toString());
        break;
    case EngNameRole:
    case EngNameOnlyRole:
        tmp.setEngName(value.toString());
        break;
    case EngGroupRole:
    case EngGroupOnlyRole:
        tmp.setEngGroup(value.toString());
        break;
    case LatinNameRole:
        tmp.setLatinName(value.toString());
        break;
    case FinGroupRole:
        tmp.setFinGroup(value.toString());
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
    case IsCustomRole:
        tmp.setCustom(value.toBool());
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
