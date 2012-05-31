#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include <QSortFilterProxyModel>
#include "historyitem.h"
#include "historymodel.h"
class FilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FilterModel(QObject *parent = 0);

signals:

public slots:

    Q_INVOKABLE int count();
    Q_INVOKABLE int rowCount();
    Q_INVOKABLE void setSorting(int type, bool ascending);
    Q_INVOKABLE void sectionAlphas();
    Q_INVOKABLE void filter(const QString &str);
    Q_INVOKABLE bool setData(int index, const QVariant &data, int role);
    Q_INVOKABLE QVariant data(int index, int role);
    Q_INVOKABLE void removeRow(int index);
    Q_INVOKABLE HistoryItem* get(int index);

private:
};

#endif // FILTERMODEL_H
