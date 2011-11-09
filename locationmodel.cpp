#include "locationmodel.h"

LocationModel::LocationModel(QObject *parent) :
    QAbstractListModel(parent)
{
    roles[TownRole] = "town";
    roles[PlaceRole] = "place";
    roles[CoordinateRole] = "coordinate";
    setRoleNames(roles);
}

int LocationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent )
    return items.count();
}

void LocationModel::clear()
{
    QAbstractItemModel::beginRemoveRows( QModelIndex(), 0, items.count() );
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

    if( role == TownRole )
    {
        return item.town();
    }
    else if( role == PlaceRole )
    {
        return item.place();
    }
    else if( role == CoordinateRole )
    {
        return item.coordinate();
    }
    return QVariant();
}

bool LocationModel::removeRow ( int row, const QModelIndex & parent)
{
    Q_UNUSED( parent )
    if ( row < 0 || row >= items.count() )
    {
        return false;
    }
    QAbstractItemModel::beginRemoveRows( QModelIndex(), row, row );
    items.removeAt( row );
    QAbstractItemModel::endRemoveRows();
    return true;
}

bool LocationModel::removeRows ( int row, int count, const QModelIndex & parent)
{
    Q_UNUSED( parent )
    if ( row < 0 || row >= items.count() || count + row > items.count() )
    {
        return false;
    }
    QAbstractItemModel::beginRemoveRows( QModelIndex(), row, row + count );
    for( int i = row; i < row + count; i++ )
    {
        items.removeAt( row );
    }
    QAbstractItemModel::endRemoveRows();
    return true;
}


void LocationModel::addItem(const Location &item)
{
    int index = items.size();
    QAbstractItemModel::beginInsertRows( QModelIndex(), index, index);
    items.append( item );
    QAbstractItemModel::endInsertRows();
}

Location LocationModel::getItem( int row )
{
    if( row < 0 || row >= items.count() )
    {
        return Location();
    }
    return items.at( row );
}

QList<Location> LocationModel::content() const
{
    return items;
}

void LocationModel::setContent( const QList<Location> &newItems )
{
    if( newItems.size() > 0 )
    {
        QAbstractItemModel::beginInsertRows( QModelIndex(), 0, newItems.size() - 1 );
        items = newItems;
        QAbstractItemModel::endInsertRows();
    }
}
