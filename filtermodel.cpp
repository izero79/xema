#include <QDebug>

#include "filtermodel.h"

FilterModel::FilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

int FilterModel::rowCount()
{
    if( sourceModel() == 0 )
    {
        return false;
    }
    return sourceModel()->rowCount();
}

void FilterModel::setSorting( int type, bool ascending )
{
    qDebug() << "SORT type" << type;
    Q_UNUSED( type )
    if( type == 0 )
    {
        setSortRole(Qt::UserRole + 2);

        sort( 0, Qt::AscendingOrder );
        qDebug() << "SORTattu + 2";

        return;
    }
    else if( type == 1 )
    {
        setSortRole(Qt::UserRole + 3);

        sort( 0, Qt::AscendingOrder );
        qDebug() << "SORTattu + 3";
        return;
    }
    qDebug() << "EI TANNE";

    if( ascending )
    {
        sort( 0, Qt::AscendingOrder );
    }
    else
    {
        sort( 0, Qt::DescendingOrder );
    }
}

void FilterModel::sectionAlphas()
{
    if( sourceModel() == 0 )
    {
        return;
    }
    int rowCount = 0;
    QList<QPair<QChar,int> > alphas;

    rowCount = sourceModel()->rowCount();
    QChar lastAlpha;
    for( int i = 0; i < rowCount; i++ )
    {
        QModelIndex idx = sourceModel()->index( i, 0 );

        QString title;// = sourceModel()->data( idx, MediaPieceModel::TitleRole ).toString().toUpper();
        qDebug() << "Title:" << title << "at" << i;
        if( title.startsWith( "THE " ) )
        {
            qDebug() << "Removing THE";
            title = title.right( title.length() - 4 );
        }
        else if( title.startsWith( "A " ) )
        {
            qDebug() << "Removing A";
            title = title.right( title.length() - 2 );
        }
        else
        {
            do
            {
                if( title.at( 0 ).isLetterOrNumber() == false )
                {
                    qDebug() << "Removing letter" << title.at( 0 );
                    title.remove( 0, 1 );
                }
                else
                {
                    if( title.at( 0 ) == 'A' )
                    {
                        qDebug() << "AAAAAAA";
                        if( title.at( 0 ).decomposition().isEmpty() &&
                                lastAlpha.decomposition().isEmpty() == false )
                        {
                            title = title.replace( 0, 1, lastAlpha.decomposition() );
                        }
                        qDebug() << "dec" << title.at( 0 ).decomposition();
                        qDebug() << "last dec" << lastAlpha.decomposition();
                    }
                }
            }
            while( title.at( 0 ).isLetterOrNumber() == false );
        }

        if( title.at( 0 ).isNumber() == true )
        {
            title = QString( "#" );
        }
        QChar alpha = title.at( 0 );
        if( alpha != lastAlpha )
        {
            lastAlpha = alpha;
            alphas.append( QPair<QChar,int>( alpha, i ) );
        }
    }
    for( int i = 0; i < alphas.size(); i++ )
    {
        qDebug() << "Alpha" << alphas.at( i ).first << "at" << alphas.at( i ).second;
    }
}

void FilterModel::filter( const QString &str )
{
    setFilterRole(Qt::UserRole + 1);
    QString filter = str;
    QRegExp exp( filter, Qt::CaseInsensitive, QRegExp::RegExp );
    setFilterRegExp(exp);
}

bool FilterModel::setData( int index, const QVariant &data, int role )
{
    QModelIndex idx = sourceModel()->index( index, 0 );
    qDebug() << "setting data for index" << index;
    return sourceModel()->setData( idx, data, role );
}

QVariant FilterModel::data( int index, int role )
{
    QModelIndex idx = sourceModel()->index( index, 0 );

    return sourceModel()->data( idx, role );
}
