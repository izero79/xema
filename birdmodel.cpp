#include "birdmodel.h"

BirdModel::BirdModel(QObject *parent) :
    QAbstractListModel(parent)
{
    roles[IdRole] = "itemid";
    roles[FinNameRole] = "finname";
    roles[SweNameRole] = "swename";
    roles[AbbrevRole] = "abbrev";
    roles[LatinNameRole] = "latinname";
    roles[CategoryRole] = "category";
    setRoleNames(roles);
}

int BirdModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent )
    return items.count();
}

void BirdModel::clear()
{
    QAbstractItemModel::beginRemoveRows( QModelIndex(), 0, items.count() );
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

    if( role == IdRole )
    {
        return item.id();
    }
    else if( role == FinNameRole )
    {
        return item.finName();
    }
    else if( role == SweNameRole )
    {
        return item.sweName();
    }
    else if( role == AbbrevRole )
    {
        return item.abbreviation();
    }
    else if( role == LatinNameRole )
    {
        return item.latinName();
    }
    else if( role == CategoryRole )
    {
        return item.category();
    }
    return QVariant();
}

bool BirdModel::removeRow ( int row, const QModelIndex & parent)
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

bool BirdModel::removeRows ( int row, int count, const QModelIndex & parent)
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


void BirdModel::addItem(const Bird &item)
{
    int index = items.size();
    QAbstractItemModel::beginInsertRows( QModelIndex(), index, index);
    items.append( item );
    QAbstractItemModel::endInsertRows();
}

Bird BirdModel::getItem( int row )
{
    if( row < 0 || row >= items.count() )
    {
        return Bird();
    }
    return items.at( row );
}

QList<Bird> BirdModel::content() const
{
    return items;
}

void BirdModel::setContent( const QList<Bird> &newItems )
{
    if( newItems.size() > 0 )
    {
        QAbstractItemModel::beginInsertRows( QModelIndex(), 0, newItems.size() - 1 );
        items = newItems;
        QAbstractItemModel::endInsertRows();
    }
}
