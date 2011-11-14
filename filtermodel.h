#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include <QSortFilterProxyModel>

class FilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FilterModel(QObject *parent = 0);

signals:

public slots:

    Q_INVOKABLE int rowCount();
    Q_INVOKABLE void setSorting( int type, bool ascending );
    Q_INVOKABLE void sectionAlphas();
    Q_INVOKABLE void filter( const QString &str );
    Q_INVOKABLE bool setData( int index, const QVariant &data, int role );
    Q_INVOKABLE QVariant data( int index, int role );

private:
};

#endif // FILTERMODEL_H