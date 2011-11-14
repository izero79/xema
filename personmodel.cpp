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
    roles[SelectedRole] = "selected";
    setRoleNames(roles);
}

int PersonModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent )
    return items.count();
}

void PersonModel::clear()
{
    QAbstractItemModel::beginRemoveRows( QModelIndex(), 0, items.count() );
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

    if( role == FilterRole )
    {
        return item.name();
    }
    else if( role == IndexRole )
    {
        return index.row();
    }
    else if( role == NameRole )
    {
        return item.name();
    }
    else if( role == RegisteredRole )
    {
        return item.registered();
    }
    else if( role == DefaultRole )
    {
        return item.defaultName();
    }
    else if( role == SelectedRole )
    {
        return item.selected();
    }
    return QVariant();
}

bool PersonModel::removeRow ( int row, const QModelIndex & parent)
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

bool PersonModel::removeRows ( int row, int count, const QModelIndex & parent)
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


void PersonModel::addItem(const Person &item)
{
    int index = items.size();
    QAbstractItemModel::beginInsertRows( QModelIndex(), index, index);
    items.append( item );
    QAbstractItemModel::endInsertRows();
}

Person PersonModel::getItem( int row )
{
    if( row < 0 || row >= items.count() )
    {
        return Person();
    }
    return items.at( row );
}

QList<Person> PersonModel::content() const
{
    return items;
}

void PersonModel::setContent( const QList<Person> &newItems )
{
    if( newItems.size() > 0 )
    {
        QAbstractItemModel::beginInsertRows( QModelIndex(), 0, newItems.size() - 1 );
        items = newItems;
        QAbstractItemModel::endInsertRows();
    }
}

bool PersonModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if( role != SelectedRole )
    {
        return false;
    }
    Person tmp = items.at( index.row() );
    tmp.setSelected( value.toBool() );
    items.replace( index.row(), tmp );
    QAbstractItemModel::dataChanged( index, index );
    return true;
}
