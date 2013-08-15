#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractListModel>

#include "historyitem.h"

class HistoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit HistoryModel(QObject *parent = 0);

    enum Roles
    {
        FilterRole = Qt::UserRole + 1,
        IdRole,
        PlaceRole, // 35
        DateRole,
        SpeciesRole,
        DateCountRole,
        PlaceCountRole,
        SpeciesCountRole, // 40
        SortableDateRole,
        SortableTimeRole,
        SectionRole, // 43
        SpeciesAbbrRole,
        TiiraExportedRole
    };

signals:

public slots:
    Q_INVOKABLE     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE     int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE     void clear();
    Q_INVOKABLE     void addItem(const HistoryItem &item);
    Q_INVOKABLE     void addItemAtBeginning(const HistoryItem &item);
    Q_INVOKABLE     bool removeRow (int row, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex());
    Q_INVOKABLE     HistoryItem getItem(int index);
    Q_INVOKABLE     HistoryItem* get(int index);
    Q_INVOKABLE     void replaceItem(int row, const HistoryItem &item);

                    void setContent(const QList<HistoryItem> &newItems);
                    QList<HistoryItem> content() const;

private:
    QList<HistoryItem> items;
    QHash<int, QByteArray> roles;

};

#endif // HISTORYMODEL_H
