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
#include "settings.h"

QMLWindow::QMLWindow(QWidget *parent) :
    #ifdef Q_OS_SYMBIAN
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
    mSettings( 0 ),
    mDataWriter( 0 )
{
#ifdef Q_OS_SYMBIAN
    mView = new QDeclarativeView(this);
    mView->setResizeMode( QDeclarativeView::SizeRootObjectToView );
    this->setCentralWidget( mView );
#else
    setResizeMode( QDeclarativeView::SizeRootObjectToView );
#endif

#if defined Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation, true );
    connect(QApplication::desktop(),SIGNAL(resized(int)),this,SLOT(orientationChanged()));
#endif

#ifdef Q_OS_SYMBIAN
    mRootContext = mView->rootContext();
#else
    mRootContext = rootContext();
#endif
    mRootContext->setContextProperty( "clockTime", "" );
    mRootContext->setContextProperty( "mainWidth", QApplication::desktop()->width() );
    mRootContext->setContextProperty( "mainHeight", QApplication::desktop()->height() );

#ifdef SYMBIAN3
    mView->setSource(QUrl("qrc:qml/lokki/main.qml"));
    mRootObject = dynamic_cast<QObject*>(mView->rootObject());
#elif defined Q_OS_SYMBIAN
    mView->setSource(QUrl("qrc:qml/lokki/main.qml"));
    mRootObject = dynamic_cast<QObject*>(mView->rootObject());
#elif defined HARMATTAN
    setSource(QUrl("qrc:qml/lokki/main.qml"));
    mRootObject = dynamic_cast<QObject*>(rootObject());
#else
    setSource(QUrl("qrc:qml/lokki/main.qml"));
    mRootObject = dynamic_cast<QObject*>(rootObject());
#endif

#if defined Q_WS_MAEMO_5
    QMetaObject::invokeMethod(mRootObject, "setStyle",
             Q_ARG(QVariant, "maemo" ));
#else
    #if defined Q_OS_SYMBIAN
    QMetaObject::invokeMethod(mRootObject, "setStyle",
                 Q_ARG(QVariant, "symbian" ));
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

#ifdef Q_OS_SYMBIAN
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
    mFilteredBirdModel = new FilterModel(this);
    mFilteredPersonModel = new FilterModel(this);
    mFilteredLocationModel = new FilterModel(this);
    mFilteredStatusModel = new FilterModel(this);
    mFilteredHistoryModel = new FilterModel(this);

    mRootContext->setContextProperty( "birdModel", mFilteredBirdModel );
    mRootContext->setContextProperty( "personModel", mFilteredPersonModel );
    mRootContext->setContextProperty( "locationModel", mFilteredLocationModel );
    mRootContext->setContextProperty( "statusModel", mFilteredStatusModel );
    mRootContext->setContextProperty( "historyModel", mFilteredHistoryModel );

    connect(mRootObject,SIGNAL(writeNew(QString)),this,SLOT(writeNewObservation(QString)));
    connect(mRootObject,SIGNAL(readObs(QString)),this,SLOT(loadObservation(QString)));
    connect(mRootObject,SIGNAL(reloadHistory()),this,SIGNAL(reloadHistory()));
    connect(mRootObject,SIGNAL(saveSystematicSorting(bool)),this,SLOT(saveSystematicSorting(bool)));
    connect(mRootObject,SIGNAL(saveDetailLevel(int)),this,SLOT(saveDetailLevel(int)));

    mSettings = new Settings( this );
    mDataWriter = new ModelDataWriter( this );

    qDebug() << "INIT";
    QMetaObject::invokeMethod(mRootObject, "setSystematicSort",
             Q_ARG(QVariant, mSettings->systematicSorting() ));
    QMetaObject::invokeMethod(mRootObject, "setDetailLevel",
             Q_ARG(QVariant, mSettings->detailLevel() ));
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

void QMLWindow::saveDetailLevel( int level )
{
    mSettings->setDetailLevel( level );
}

void QMLWindow::saveSystematicSorting( bool systematic )
{
    mSettings->setSystematicSorting( systematic );
}
