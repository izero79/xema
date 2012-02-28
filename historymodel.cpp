#include "historymodel.h"

HistoryModel::HistoryModel(QObject *parent) :
    QAbstractListModel(parent)
{
    roles[FilterRole] = "filter";
    roles[IdRole] = "itemid";
    roles[PlaceRole] = "place";
    roles[DateRole] = "date";
    roles[SpecieRole] = "specie";
    roles[DateCountRole] = "dateCount";
    roles[PlaceCountRole] = "placeCount";
    roles[SpecieCountRole] = "specieCount";
    setRoleNames(roles);
}

int HistoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent )
    return items.count();
}

void HistoryModel::clear()
{
    QAbstractItemModel::beginRemoveRows( QModelIndex(), 0, items.count() );
    items.clear();
    QAbstractItemModel::endRemoveRows();
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > items.count())
    {
        return QVariant();
    }

    HistoryItem item = items[index.row()];

    if( role == FilterRole )
    {
        return QString( item.place() + ", " +
                        item.date() + ", " +
                        item.species() );
    }
    else if( role == IdRole )
    {
        return item.id();
    }
    else if( role == PlaceRole )
    {
        return item.place();
    }
    else if( role == DateRole )
    {
        return item.date();
    }
    else if( role == SpecieRole )
    {
        return item.species();
    }
    else if( role == DateCountRole )
    {
        return item.dateCount();
    }
    else if( role == PlaceCountRole )
    {
        return item.placeCount();
    }
    else if( role == SpecieCountRole )
    {
        return item.specieCount();
    }
    return QVariant();
}

bool HistoryModel::removeRow ( int row, const QModelIndex & parent)
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

bool HistoryModel::removeRows ( int row, int count, const QModelIndex & parent)
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


void HistoryModel::addItem(const HistoryItem &item)
{
    int index = items.size();
    QAbstractItemModel::beginInsertRows( QModelIndex(), index, index);
    items.append( item );
    QAbstractItemModel::endInsertRows();
}

HistoryItem HistoryModel::getItem( int row )
{
    if( row < 0 || row >= items.count() )
    {
        return HistoryItem();
    }
    return items.at( row );
}

void HistoryModel::replaceItem(int row, const HistoryItem &item)
{
    items.replace( row, item );
    QModelIndex idx = index( row, 0 );
    QAbstractItemModel::dataChanged( idx, idx );
}

QList<HistoryItem> HistoryModel::content() const
{
    return items;
}

void HistoryModel::setContent( const QList<HistoryItem> &newItems )
{
    if( newItems.size() > 0 )
    {
        QAbstractItemModel::beginInsertRows( QModelIndex(), 0, newItems.size() - 1 );
        items = newItems;
        QAbstractItemModel::endInsertRows();
    }
}
