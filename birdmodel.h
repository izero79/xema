#ifndef BIRDMODEL_H
#define BIRDMODEL_H

#include <QAbstractListModel>

#include "bird.h"

class BirdModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BirdModel(QObject *parent = 0);

    enum Roles
    {
        FilterRole = Qt::UserRole + 1,
        IdRole,
        FinNameRole,
        SweNameRole,
        AbbrevRole,
        LatinNameRole,
        CategoryRole
    };

signals:

public slots:
    Q_INVOKABLE     QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    Q_INVOKABLE     int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    Q_INVOKABLE     void clear();
    Q_INVOKABLE     void addItem( const Bird &item );
    Q_INVOKABLE     bool removeRow ( int row, const QModelIndex & parent = QModelIndex() );
    Q_INVOKABLE     bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
    Q_INVOKABLE     Bird getItem( int index );

                    void setContent( const QList<Bird> &newItems );
                    QList<Bird> content() const;

private:
    QList<Bird> items;
    QHash<int, QByteArray> roles;
};

#endif // BIRDMODEL_H
