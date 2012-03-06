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
    else if( role == FirstNameRole )
    {
        return item.firstName();
    }
    else if( role == SurNameRole )
    {
        return item.surName();
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
    qDebug() << "remove row" << row;
    if ( row < 0 || row >= items.count() )
    {
        return false;
    }
    QAbstractItemModel::beginRemoveRows( QModelIndex(), row, row );
    qDebug() << "items before remove" << items.count();
    items.removeAt( row );
    qDebug() << "items after remove" << items.count();
    QAbstractItemModel::endRemoveRows();
    return true;
}

bool PersonModel::removeRows ( int row, int count, const QModelIndex & parent)
{
    Q_UNUSED( parent )
    qDebug() << "remove rows" << row << count;
    if ( row < 0 || row >= items.count() || (count - 1) + row > items.count() )
    {
        return false;
    }
    QAbstractItemModel::beginRemoveRows( QModelIndex(), row, row + ( count - 1 ) );
    for( int i = row; i < row + count; i++ )
    {
        qDebug() << "removing row" << i;
        items.removeAt( row );
    }
    QAbstractItemModel::endRemoveRows();
    qDebug() << "remove rows - rowcount now" << items.count();
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
    qDebug() << "joo" << index.row() << items.count();
    int row = index.row();
    Person tmp;
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
    case RegisteredRole:
        tmp.setRegistered( value.toBool() );
        break;
    case DefaultRole:
        tmp.setDefault( value.toBool() );
        break;
    case FirstNameRole:
        tmp.setFirstName( value.toString() );
        break;
    case SurNameRole:
        tmp.setSurName( value.toString() );
        break;
    case SelectedRole:
        tmp.setSelected( value.toBool() );
        break;
    default:
        break;
    }
    qDebug() << "setData 2" << row << items.count();
    if( row >= items.count() )
    {
        qDebug() << "setData 3";
        QAbstractItemModel::beginInsertRows( QModelIndex(), items.count(), items.count() );
        qDebug() << "setData 4";
        items.append( tmp );
        qDebug() << "setData 5" << items.count();
        QAbstractItemModel::endInsertRows();
    }
    else
    {
        qDebug() << "setData 6";
        items.replace( index.row(), tmp );
        qDebug() << "setData 7";
    }
    qDebug() << "setData 8";
    QAbstractItemModel::dataChanged( index, index );
    qDebug() << "setData 9";
    return true;
}
