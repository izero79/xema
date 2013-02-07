#include "locationmodel.h"
#include "settings.h"

#include <QDebug>

LocationModel::LocationModel(QObject *parent) :
    QAbstractListModel(parent)
{
    roles[FilterRole] = "filter";
    roles[IndexRole] = "realindex";
    roles[TownRole] = "town";
    roles[PlaceRole] = "place";
    roles[CoordinateRole] = "coordinate";
    roles[EngTownRole] = "engtown";
    roles[EngPlaceRole] = "engplace";
    roles[SweTownRole] = "swetown";
    roles[SwePlaceRole] = "sweplace";
    roles[EngTownOnlyRole] = "engonlytown";
    roles[EngPlaceOnlyRole] = "engonlyplace";
    roles[SweTownOnlyRole] = "sweonlytown";
    roles[SwePlaceOnlyRole] = "sweonlyplace";
    roles[IsCustomRole] = "custom";
    roles[FullStringRole] = "fullstring";
    roles[LocalizedTownRole] = "localizedTown";
    roles[LocalizedPlaceRole] = "localizedPlace";
    roles[CountryRole] = "country";
    roles[SweCountryRole] = "swecountry";
    roles[EngCountryRole] = "engcountry";
    roles[LocalizedCountryRole] = "localizedCountry";
    roles[EngCountryOnlyRole] = "engonlycountry";
    roles[SweCountryOnlyRole] = "sweonlycountry";
    setRoleNames(roles);
}

int LocationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return items.count();
}

void LocationModel::clear()
{
    QAbstractItemModel::beginRemoveRows(QModelIndex(), 0, items.count());
    items.clear();
    QAbstractItemModel::endRemoveRows();
}

QVariant LocationModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > items.count())
    {
        return QVariant();
    }

    Location item = items[index.row()];

    if (role == FilterRole)
    {
        return QString( item.localizedCountry() + ", " + item.localizedPlace() + ", " +
                        item.localizedTown() + ", " + item.wgsCoordinateForFilter());
    }
    else if (role == IndexRole)
    {
        return index.row();
    }
    else if (role == LocalizedTownRole)
    {
        return item.localizedTown();
    }
    else if (role == LocalizedPlaceRole)
    {
        return item.localizedPlace();
    }
    else if (role == LocalizedCountryRole)
    {
        return item.localizedCountry();
    }
    else if (role == TownRole)
    {
        return item.town();
    }
    else if (role == PlaceRole)
    {
        return item.place();
    }
    else if (role == CountryRole)
    {
        return item.finCountry();
    }
    else if (role == SweCountryRole)
    {
        return item.sweCountry();
    }
    else if (role == EngCountryRole)
    {
        return item.engCountry();
    }
    else if (role == EngTownRole)
    {
        return item.engTown();
    }
    else if (role == EngPlaceRole)
    {
        return item.engPlace();
    }
    else if (role == SweTownRole)
    {
        return item.sweTown();
    }
    else if (role == SwePlaceRole)
    {
        return item.swePlace();
    }
    else if (role == EngTownOnlyRole)
    {
        return item.engTown(true);
    }
    else if (role == EngPlaceOnlyRole)
    {
        return item.engPlace(true);
    }
    else if (role == SweTownOnlyRole)
    {
        return item.sweTown(true);
    }
    else if (role == SwePlaceOnlyRole)
    {
        return item.swePlace(true);
    }
    else if (role == CoordinateRole)
    {
        return item.coordinate();
    }
    else if (role == YkjCoordinateRole)
    {
        return item.ykjCoordinate();
    }
    else if (role == WgsCoordinateRole)
    {
        return item.wgsCoordinate();
    }
    else if (role == IsCustomRole)
    {
        return item.custom();
    }
    else if (role == FullStringRole)
    {
        return item.town() + ", " + item.place();
    }
    else if (role == EngCountryOnlyRole)
    {
        return item.engCountry(true);
    }
    else if (role == SweCountryOnlyRole)
    {
        return item.sweCountry(true);
    }
    else if (role == SortingNameRole)
    {
        QString lang = Settings::lang();
        if (lang == "en") {
            return item.engCountry(false) + ", " + item.engTown(false) + ", " + item.engPlace(false);
        }
        else if (lang == "sv") {
            return item.sweCountry(false) + ", " + item.sweTown(false) + ", " + item.swePlace(false);
        }
        else {
            return item.finCountry() + ", " + item.town() + ", " + item.place();
        }
    }
    return QVariant();
}

bool LocationModel::removeRow (int row, const QModelIndex & parent)
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

bool LocationModel::removeRows (int row, int count, const QModelIndex & parent)
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


void LocationModel::addItem(const Location &item)
{
    int index = items.size();
    QAbstractItemModel::beginInsertRows(QModelIndex(), index, index);
    items.append(item);
    QAbstractItemModel::endInsertRows();
}

Location LocationModel::getItem(int row) const
{
    if (row < 0 || row >= items.count())
    {
        return Location();
    }
    return items.at(row);
}

QList<Location> LocationModel::content() const
{
    return items;
}

void LocationModel::setContent(const QList<Location> &newItems)
{
    if (newItems.size() > 0)
    {
        QAbstractItemModel::beginInsertRows(QModelIndex(), 0, newItems.size() - 1);
        items = newItems;
        QAbstractItemModel::endInsertRows();
    }
}

bool LocationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    Location tmp;
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
    case TownRole:
        tmp.setTown(value.toString());
        break;
    case PlaceRole:
        tmp.setPlace(value.toString());
        break;
    case CountryRole:
        tmp.setFinCountry(value.toString());
        break;
    case YkjCoordinateRole:
        tmp.setYKJCoordinate(value.toString());
        break;
    case WgsCoordinateRole:
        tmp.setWGSCoordinate(value.toString());
        break;
    case SweTownRole:
    case SweTownOnlyRole:
        tmp.setSweTown(value.toString());
        break;
    case SwePlaceRole:
    case SwePlaceOnlyRole:
        tmp.setSwePlace(value.toString());
        break;
    case EngTownRole:
    case EngTownOnlyRole:
        tmp.setEngTown(value.toString());
        break;
    case EngPlaceRole:
    case EngPlaceOnlyRole:
        tmp.setEngPlace(value.toString());
        break;
    case SweCountryRole:
    case SweCountryOnlyRole:
        tmp.setSweCountry(value.toString());
        break;
    case EngCountryRole:
    case EngCountryOnlyRole:
        tmp.setEngCountry(value.toString());
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

void LocationModel::replaceItem(int row, const Location &item)
{
    items.replace(row, item);
    QModelIndex idx = index(row, 0);
    QAbstractItemModel::dataChanged(idx, idx);
}
