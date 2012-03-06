#include <QApplication>
#include <QDeclarativeContext>
#include <QDesktopWidget>
#include <QObject>
#include <QMainWindow>
#include <QDeclarativeView>
#include <QSortFilterProxyModel>
#include <QGraphicsObject>
#include <QDebug>

#include "qmlwindow.h"
#include "filtermodel.h"
#include "birdmodel.h"
#include "locationmodel.h"
#include "personmodel.h"
#include "statusmodel.h"
#include "modeldatawriter.h"
#include "historymodel.h"
#include "atlasindexmodel.h"
#include "settings.h"

QMLWindow::QMLWindow(QWidget *parent) :
    #if defined( Q_OS_SYMBIAN ) && !defined( SYMBIAN3 )
    QMainWindow(parent),
    mView(0),
    #else
    QDeclarativeView(parent),
    #endif
    mRootContext( 0 ),
    mRootObject( 0 ),
    mFilteredPersonModel( 0 ),
    mFilteredBirdModel( 0 ),
    mFilteredLocationModel( 0 ),
    mFilteredStatusModel( 0 ),
    mFilteredHistoryModel( 0 ),
    mFilteredHistoryDateModel( 0 ),
    mFilteredHistoryPlaceModel( 0 ),
    mFilteredAtlasModel( 0 ),
    mSettings( 0 ),
    mDataWriter( 0 )
{
    qDebug() << "qmlwin 0";
#if defined( Q_OS_SYMBIAN ) && !defined( SYMBIAN3 )
    qDebug() << "qmlwin 1";
    mView = new QDeclarativeView(this);
    qDebug() << "qmlwin 2";
    mView->setResizeMode( QDeclarativeView::SizeRootObjectToView );
    qDebug() << "qmlwin 3";
    this->setCentralWidget( mView );
#else
    qDebug() << "qmlwin 1.1";
    setResizeMode( QDeclarativeView::SizeRootObjectToView );
    qDebug() << "qmlwin 2.1 3.1";
#endif

#if defined Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation, true );
    connect(QApplication::desktop(),SIGNAL(resized(int)),this,SLOT(orientationChanged()));
#endif

#if defined( Q_OS_SYMBIAN ) && !defined( SYMBIAN3 )
    qDebug() << "qmlwin 4";
    mRootContext = mView->rootContext();
#else
    qDebug() << "qmlwin 4.1";
    mRootContext = rootContext();
#endif
    qDebug() << "qmlwin 5";
    mRootContext->setContextProperty( "clockTime", "" );
    mRootContext->setContextProperty( "mainWidth", QApplication::desktop()->width() );
    mRootContext->setContextProperty( "mainHeight", QApplication::desktop()->height() );

#ifdef SYMBIAN3
    qDebug() << "qmlwin 6";
    setSource(QUrl("qrc:qml/lokki/main.qml"));
    qDebug() << "qmlwin 7";
    mRootObject = dynamic_cast<QObject*>(rootObject());
    qDebug() << "qmlwin 8";
#elif defined( Q_OS_SYMBIAN ) && !defined( SYMBIAN3 )
    qDebug() << "qmlwin 6.1";
    mView->setSource(QUrl("qrc:qml/lokki/main.qml"));
    qDebug() << "qmlwin 7.1";
    mRootObject = dynamic_cast<QObject*>(mView->rootObject());
    qDebug() << "qmlwin 8.1";
#elif defined HARMATTAN
    setSource(QUrl("qrc:qml/lokki/main.qml"));
    mRootObject = dynamic_cast<QObject*>(rootObject());
#else
    qDebug() << "qmlwin 6.3";
    setSource(QUrl("qrc:qml/lokki/main.qml"));
    qDebug() << "qmlwin 7.3";
    mRootObject = dynamic_cast<QObject*>(rootObject());
    qDebug() << "qmlwin 8.3";
#endif

#if defined Q_WS_MAEMO_5
    QMetaObject::invokeMethod(mRootObject, "setStyle",
             Q_ARG(QVariant, "maemo" ));
#else
    #if defined( Q_OS_SYMBIAN ) && !defined( SYMBIAN3 )
    qDebug() << "qmlwin 9";
    QMetaObject::invokeMethod(mRootObject, "setStyle",
                 Q_ARG(QVariant, "symbian" ));
    qDebug() << "qmlwin 10";
    #elif defined Q_WS_SIMULATOR
        QMetaObject::invokeMethod(mRootObject, "setStyle",
                 Q_ARG(QVariant, "maemo" ));
    #elif defined HARMATTAN
        QMetaObject::invokeMethod(mRootObject, "setStyle",
                 Q_ARG(QVariant, "meego" ));
    #else
        QMetaObject::invokeMethod(mRootObject, "setStyle",
                 Q_ARG(QVariant, "maemo" ));
    #endif
#endif

}

QMLWindow::~QMLWindow()
{
    qDebug() << "QMLWindow::~QMLWindow()";
}

#if defined( Q_OS_SYMBIAN ) && !defined( SYMBIAN3 )
void QMLWindow::resizeEvent( QResizeEvent *event )
{
    orientationChanged();
}
#endif

void QMLWindow::orientationChanged()
{
    QRect applicationRect = QApplication::desktop()->screenGeometry();
    qDebug() << "void QMLWindow::orientationChanged()" << "new apprect" << applicationRect;
    this->setGeometry( applicationRect );
    QString width;
    width.setNum( rect().width() );
    QString height;
    height.setNum( rect().height() );
    mRootContext->setContextProperty( "mainWidth", width );
    mRootContext->setContextProperty( "mainHeight", height );
}

void QMLWindow::init()
{
    qDebug() << "WIN INIT START";
    mFilteredBirdModel = new FilterModel(this);
    mFilteredPersonModel = new FilterModel(this);
    mFilteredLocationModel = new FilterModel(this);
    mFilteredStatusModel = new FilterModel(this);
    mFilteredHistoryModel = new FilterModel(this);
    mFilteredHistoryDateModel = new FilterModel(this);
    mFilteredHistoryPlaceModel = new FilterModel(this);
    mFilteredAtlasModel = new FilterModel(this);

    mRootContext->setContextProperty( "birdModel", mFilteredBirdModel );
    mRootContext->setContextProperty( "personModel", mFilteredPersonModel );
    mRootContext->setContextProperty( "locationModel", mFilteredLocationModel );
    mRootContext->setContextProperty( "statusModel", mFilteredStatusModel );
    mRootContext->setContextProperty( "historyModel", mFilteredHistoryModel );
    mRootContext->setContextProperty( "historyDateModel", mFilteredHistoryDateModel );
    mRootContext->setContextProperty( "historyPlaceModel", mFilteredHistoryPlaceModel );
    mRootContext->setContextProperty( "atlasModel", mFilteredAtlasModel );

    connect(mRootObject,SIGNAL(writeNew(QString)),this,SLOT(writeNewObservation(QString)));
    connect(mRootObject,SIGNAL(readObs(QString)),this,SLOT(loadObservation(QString)));
    connect(mRootObject,SIGNAL(deleteObs(QString,QString,QString)),this,SLOT(deleteObservation(QString,QString,QString)));
    connect(mRootObject,SIGNAL(reloadHistory()),this,SIGNAL(reloadHistory()));
    connect(mRootObject,SIGNAL(saveSystematicSorting(bool)),this,SLOT(saveSystematicSorting(bool)));
    connect(mRootObject,SIGNAL(saveDetailLevel(int)),this,SLOT(saveDetailLevel(int)));
    connect(mRootObject,SIGNAL(quit()),this,SIGNAL(quit()));
    connect(mRootObject,SIGNAL(loadHistoryWithDate(QString)),this,SIGNAL(loadHistoryWithDate(QString)));
    connect(mRootObject,SIGNAL(loadHistoryWithDateAndPlace(QString,QString)),this,SIGNAL(loadHistoryWithDateAndPlace(QString,QString)));
    connect(mRootObject,SIGNAL(exportData(bool)),this,SLOT(exportData(bool)));
    connect(mRootObject,SIGNAL(restoreObservers()),this,SIGNAL(restoreObservers()));
    connect(mRootObject,SIGNAL(restoreLocations()),this,SIGNAL(restoreLocations()));
    connect(mRootObject,SIGNAL(restoreSpecies()),this,SIGNAL(restoreSpecies()));


    mSettings = new Settings( this );
    mDataWriter = new ModelDataWriter( this );

    qDebug() << "WIN INIT 1";
    QMetaObject::invokeMethod(mRootObject, "setSystematicSort",
             Q_ARG(QVariant, mSettings->systematicSorting() ));
    QMetaObject::invokeMethod(mRootObject, "setDetailLevel",
             Q_ARG(QVariant, mSettings->detailLevel() ));
    qDebug() << "WIN INIT DONE";
}

void QMLWindow::setBirdModel( BirdModel *model )
{
    mFilteredBirdModel->setSourceModel( model );
}

void QMLWindow::setPersonModel(PersonModel *model)
{
    mFilteredPersonModel->setSourceModel( model );
}

void QMLWindow::setLocationModel(LocationModel *model)
{
    mFilteredLocationModel->setSourceModel( model );
}

void QMLWindow::setStatusModel(StatusModel *model)
{
    mFilteredStatusModel->setSourceModel( model );
}

void QMLWindow::setHistoryModel(HistoryModel *model)
{
    mFilteredHistoryModel->setSourceModel( model );
}

void QMLWindow::setHistoryDateModel(HistoryModel *model)
{
    mFilteredHistoryDateModel->setSourceModel( model );
}

void QMLWindow::setHistoryPlaceModel(HistoryModel *model)
{
    mFilteredHistoryPlaceModel->setSourceModel( model );
}

void QMLWindow::setAtlasModel(AtlasIndexModel *model)
{
    mFilteredAtlasModel->setSourceModel( model );
}

void QMLWindow::writeNewObservation( const QString &data )
{
    mDataWriter->writeNewObservation( data );
}

void QMLWindow::loadObservation( const QString &id )
{
    qlonglong idNum = id.toLongLong();
    QString data = mDataWriter->loadObservation( idNum );
    QMetaObject::invokeMethod(mRootObject, "dataLoaded",
             Q_ARG(QVariant, data ));
}

void QMLWindow::deleteObservation( const QString &id, const QString &date, const QString &place )
{
    qlonglong idNum = id.toLongLong();
    mDataWriter->deleteObservation( idNum );
    reloadHistory();
    loadHistoryWithDateAndPlace( date, place );
}

void QMLWindow::saveDetailLevel( int level )
{
    mSettings->setDetailLevel( level );
}

void QMLWindow::saveSystematicSorting( bool systematic )
{
    mSettings->setSystematicSorting( systematic );
}

void QMLWindow::exportData(bool onlyNew)
{
    mDataWriter->exportHistory(onlyNew);
}


