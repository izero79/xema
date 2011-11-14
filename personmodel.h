#ifndef PERSONMODEL_H
#define PERSONMODEL_H

#include <QAbstractListModel>

#include "person.h"

class PersonModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PersonModel(QObject *parent = 0);

    enum Roles
    {
        FilterRole = Qt::UserRole + 1,
        IndexRole,
        NameRole,
        RegisteredRole,
        DefaultRole,
        SelectedRole = Qt::EditRole
    };

signals:

public slots:
    Q_INVOKABLE     QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    Q_INVOKABLE     int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    Q_INVOKABLE     void clear();
    Q_INVOKABLE     void addItem( const Person &item );
    Q_INVOKABLE     bool removeRow ( int row, const QModelIndex & parent = QModelIndex() );
    Q_INVOKABLE     bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
    Q_INVOKABLE     Person getItem( int index );
    Q_INVOKABLE     bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

                    void setContent( const QList<Person> &newItems );
                    QList<Person> content() const;

private:
    QList<Person> items;
    QHash<int, QByteArray> roles;

};

#endif // PERSONMODEL_H
