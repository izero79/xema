#include "locationmodel.h"
#include <QDebug>

LocationModel::LocationModel(QObject *parent) :
    QAbstractListModel(parent)
{
    roles[FilterRole] = "filter";
    roles[IndexRole] = "realindex";
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

    if( role == FilterRole )
    {
        return QString( item.town() + ", " +
                        item.place() + ", " );
    }
    else if( role == IndexRole )
    {
        return index.row();
    }
    else if( role == TownRole )
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
    else if( role == YkjCoordinateRole )
    {
        return item.ykjCoordinate();
    }
    else if( role == WgsCoordinateRole )
    {
        return item.wgsCoordinate();
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
    if ( row < 0 || row >= items.count() || (count - 1) + row > items.count() )
    {
        return false;
    }
    QAbstractItemModel::beginRemoveRows( QModelIndex(), row, row + ( count - 1 ) );
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

bool LocationModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    qDebug() << "joo" << index.row() << items.count();
    int row = index.row();
    Location tmp;
    if( index.row() >= items.count() || index.row() < 0 )
    {
        row = items.count();
        qDebug() << "trying to create new";
    }
    else
    {
        tmp = items.at( row );
    }
    qDebug() << "setData"  << value;
    switch( role )
    {
    case TownRole:
        tmp.setTown( value.toString() );
        break;
    case PlaceRole:
        tmp.setPlace( value.toString() );
        break;
    case YkjCoordinateRole:
        tmp.setYKJCoordinate( value.toString() );
        break;
    case WgsCoordinateRole:
        tmp.setWGSCoordinate( value.toString() );
        break;
    default:
        break;
    }

    if( row >= items.count() )
    {
        QAbstractItemModel::beginInsertRows( QModelIndex(), items.count(), items.count() );
        items.append( tmp );
        QAbstractItemModel::endInsertRows();
    }
    else
    {
        items.replace( index.row(), tmp );
    }
    QAbstractItemModel::dataChanged( index, index );
    return true;
}
