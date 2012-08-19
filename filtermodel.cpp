#include <QDebug>

#include "filtermodel.h"

FilterModel::FilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

int FilterModel::count()
{
    return rowCount();
}

int FilterModel::rowCount()
{
    if (sourceModel() == 0)
    {
        return false;
    }
    return sourceModel()->rowCount();
}

void FilterModel::setSorting(int type, bool ascending)
{
//    qDebug() << "SORT type" << type;
    Q_UNUSED(type)
    if (type == 0)
    {
        setSortRole(Qt::UserRole + 3);

        sort(0, Qt::AscendingOrder);

        return;
    }
    else if (type == 1)
    {
        setSortRole(Qt::UserRole + 100);

        sort(0, Qt::AscendingOrder);
        return;
    }
//    qDebug() << "Tanne";

    if (ascending)
    {
        sort(0, Qt::AscendingOrder);
    }
    else
    {
        sort(0, Qt::DescendingOrder);
    }
}

void FilterModel::sectionAlphas()
{
    if (sourceModel() == 0)
    {
        return;
    }
    int rowCount = 0;
    QList<QPair<QChar,int> > alphas;

    rowCount = sourceModel()->rowCount();
    QChar lastAlpha;
    for(int i = 0; i < rowCount; i++)
    {
        QModelIndex idx = sourceModel()->index(i, 0);

        QString title;// = sourceModel()->data(idx, MediaPieceModel::TitleRole).toString().toUpper();
        if (title.startsWith("THE "))
        {
            title = title.right(title.length() - 4);
        }
        else if (title.startsWith("A "))
        {
            title = title.right(title.length() - 2);
        }
        else
        {
            do
            {
                if (title.at(0).isLetterOrNumber() == false)
                {
                    title.remove(0, 1);
                }
                else
                {
                    if (title.at(0) == 'A')
                    {
                        if (title.at(0).decomposition().isEmpty() &&
                                lastAlpha.decomposition().isEmpty() == false)
                        {
                            title = title.replace(0, 1, lastAlpha.decomposition());
                        }
                    }
                }
            }
            while (title.at(0).isLetterOrNumber() == false);
        }

        if (title.at(0).isNumber() == true)
        {
            title = QString("#");
        }
        QChar alpha = title.at(0);
        if (alpha != lastAlpha)
        {
            lastAlpha = alpha;
            alphas.append(QPair<QChar,int>(alpha, i));
        }
    }
}

void FilterModel::filter(const QString &str)
{
    setFilterRole(Qt::UserRole + 1);
    QString filter = str;
    QRegExp exp(filter, Qt::CaseInsensitive, QRegExp::RegExp);
    setFilterRegExp(exp);
}

bool FilterModel::setData(int index, const QVariant &data, int role)
{
    QModelIndex idx = sourceModel()->index(index, 0);
    return sourceModel()->setData(idx, data, role);
}

QVariant FilterModel::data(int index, int role)
{
//    qDebug() << Q_FUNC_INFO;
    QModelIndex idx = sourceModel()->index(index, 0);

    return sourceModel()->data(idx, role);
}

void FilterModel::removeRow(int index)
{
    sourceModel()->removeRow(index);
}

HistoryItem* FilterModel::get(int index)
{
    qDebug() << Q_FUNC_INFO;
    return ((HistoryModel*)sourceModel())->get(index);
}
