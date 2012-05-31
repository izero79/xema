#include <QApplication>
#include <QDeclarativeContext>
#include <QtDeclarative>
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
#include "modeldataloader.h"
#include "historymodel.h"
#include "atlasindexmodel.h"
#include "settings.h"
#include "xemaenums.h"
#include "agemodel.h"
#include "dressmodel.h"
#include "sexmodel.h"

QMLWindow::QMLWindow(QWidget *parent) :
    #if defined(Q_OS_SYMBIAN) && !defined(SYMBIAN3)
    QMainWindow(parent),
    mView(0),
    #else
    QDeclarativeView(parent),
    #endif
    mRootContext(0),
    mRootObject(0),
    mPersonModel(0),
    mFilteredPersonModel(0),
    mBirdModel(0),
    mFilteredBirdModel(0),
    mLocationModel(0),
    mStatusModel(0),
    mFilteredLocationModel(0),
    mFilteredStatusModel(0),
    mFilteredHistoryModel(0),
    mFilteredHistoryDateModel(0),
    mFilteredHistoryPlaceModel(0),
    mFilteredAtlasModel(0),
    mSettings(0),
    mDataWriter(0),
    mDataLoader(0)
{
#if defined(Q_OS_SYMBIAN) && !defined(SYMBIAN3)
    mView = new QDeclarativeView(this);
    mView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    this->setCentralWidget(mView);
#else
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
#endif

#if defined Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation, true);
    connect(QApplication::desktop(),SIGNAL(resized(int)),this,SLOT(orientationChanged()));
#endif

#if defined(Q_OS_SYMBIAN) && !defined(SYMBIAN3)
    mRootContext = mView->rootContext();
#else
    mRootContext = rootContext();
#endif
    mRootContext->setContextProperty("clockTime", "");
    mRootContext->setContextProperty("mainWidth", QApplication::desktop()->width());
    mRootContext->setContextProperty("mainHeight", QApplication::desktop()->height());

    qmlRegisterType<XemaEnums>("XemaEnums", 1, 0, "XemaEnums");

#ifdef SYMBIAN3
    setSource(QUrl("qrc:qml/symbian3/main.qml"));
    mRootObject = dynamic_cast<QObject*>(rootObject());
#elif defined(Q_OS_SYMBIAN) && !defined(SYMBIAN3)
    mView->setSource(QUrl("qrc:qml/symbian/main.qml"));
    mRootObject = dynamic_cast<QObject*>(mView->rootObject());
#elif defined HARMATTAN
    setSource(QUrl("qrc:qml/harmattan/main.qml"));
    mRootObject = dynamic_cast<QObject*>(rootObject());
#else
    setSource(QUrl("qrc:qml/harmattan/main.qml"));
    mRootObject = dynamic_cast<QObject*>(rootObject());
#endif
}

QMLWindow::~QMLWindow()
{
    qDebug() << "QMLWindow::~QMLWindow()";
}

#if defined(Q_OS_SYMBIAN) && !defined(SYMBIAN3)
void QMLWindow::resizeEvent(QResizeEvent *event)
{
    orientationChanged();
}
#endif

void QMLWindow::orientationChanged()
{
    QRect applicationRect = QApplication::desktop()->screenGeometry();
    this->setGeometry(applicationRect);
    QString width;
    width.setNum(rect().width());
    QString height;
    height.setNum(rect().height());
    mRootContext->setContextProperty("mainWidth", width);
    mRootContext->setContextProperty("mainHeight", height);
}

void QMLWindow::init()
{
    mFilteredBirdModel = new FilterModel(this);
    mFilteredPersonModel = new FilterModel(this);
    mFilteredLocationModel = new FilterModel(this);
    mFilteredStatusModel = new FilterModel(this);
    mFilteredHistoryModel = new FilterModel(this);
    mFilteredHistoryDateModel = new FilterModel(this);
    mFilteredHistoryPlaceModel = new FilterModel(this);
    mFilteredAtlasModel = new FilterModel(this);
    mFilteredSexModel = new FilterModel(this);
    mFilteredDressModel = new FilterModel(this);
    mFilteredAgeModel = new FilterModel(this);

    mRootContext->setContextProperty("birdModel", mFilteredBirdModel);
    mRootContext->setContextProperty("personModel", mFilteredPersonModel);
    mRootContext->setContextProperty("locationModel", mFilteredLocationModel);
    mRootContext->setContextProperty("statusModel", mFilteredStatusModel);
    mRootContext->setContextProperty("historyModel", mFilteredHistoryModel);
    mRootContext->setContextProperty("historyDateModel", mFilteredHistoryDateModel);
    mRootContext->setContextProperty("historyPlaceModel", mFilteredHistoryPlaceModel);
    mRootContext->setContextProperty("atlasModel", mFilteredAtlasModel);
    mRootContext->setContextProperty("sexModel", mFilteredSexModel);
    mRootContext->setContextProperty("dressModel", mFilteredDressModel);
    mRootContext->setContextProperty("ageModel", mFilteredAgeModel);

    QString majorVersion;
    majorVersion.setNum( MAJORVERSION );
    QString minorVersion;
    minorVersion.setNum( MINORVERSION );
    QString patchVersion;
    patchVersion.setNum( PATCHVERSION );
    QString version( majorVersion + "." + minorVersion + "." + patchVersion );
    mRootObject->setProperty( "versionString", version );

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
    connect(mRootObject,SIGNAL(restoreStatuses()),this,SIGNAL(restoreStatuses()));
    connect(mRootObject,SIGNAL(clearHistory()),this,SIGNAL(clearHistory()));
    connect(mRootObject,SIGNAL(saveLocations()),this,SIGNAL(saveLocations()));
    connect(mRootObject,SIGNAL(savePersons()),this,SIGNAL(savePersons()));
    connect(mRootObject,SIGNAL(importData()),this,SLOT(importData()));
    connect(mRootObject,SIGNAL(exportOwnData()),this,SLOT(exportOwnData()));
    connect(mRootObject,SIGNAL(importOwnData()),this,SLOT(importOwnData()));
    connect(mRootObject,SIGNAL(openUrl(QString)),this,SLOT(openBrowser(QString)));

    QString locale = QLocale::system().name();
    QString lang = locale.section("_",0,0);
    mRootObject->setProperty( "currentLanguage", lang );

    mSettings = new Settings(this);
    mDataWriter = ModelDataWriter::instance();
    mDataLoader = ModelDataLoader::instance();

    QMetaObject::invokeMethod(mRootObject, "setSystematicSort",
             Q_ARG(QVariant, mSettings->systematicSorting()));
    QMetaObject::invokeMethod(mRootObject, "setDetailLevel",
             Q_ARG(QVariant, mSettings->detailLevel()));

}

void QMLWindow::setBirdModel(BirdModel *model)
{
    mBirdModel = model;
    mFilteredBirdModel->setSourceModel(model);
    mFilteredBirdModel->setDynamicSortFilter(true);

}

void QMLWindow::setPersonModel(PersonModel *model)
{
    mPersonModel = model;
    mFilteredPersonModel->setSourceModel(model);
    mFilteredBirdModel->setDynamicSortFilter(true);
}

void QMLWindow::setLocationModel(LocationModel *model)
{
    mLocationModel = model;
    mFilteredLocationModel->setSourceModel(model);
    mFilteredLocationModel->setSortRole(LocationModel::TownRole);
    mFilteredLocationModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    mFilteredLocationModel->sort(0);
    mFilteredLocationModel->setDynamicSortFilter(true);
}

void QMLWindow::setStatusModel(StatusModel *model)
{
    mStatusModel = model;
    mFilteredStatusModel->setSourceModel(model);
}

void QMLWindow::setAgeModel(AgeModel *model)
{
    mFilteredAgeModel->setSourceModel(model);
}

void QMLWindow::setDressModel(DressModel *model)
{
    mFilteredDressModel->setSourceModel(model);
}

void QMLWindow::setSexModel(SexModel *model)
{
    mFilteredSexModel->setSourceModel(model);
}

void QMLWindow::setHistoryModel(HistoryModel *model)
{
    mFilteredHistoryModel->setSourceModel(model);
    mFilteredHistoryModel->setSortRole(HistoryModel::SortableTimeRole);
    mFilteredHistoryModel->setDynamicSortFilter(true);
}

void QMLWindow::setHistoryDateModel(HistoryModel *model)
{
    mFilteredHistoryDateModel->setSourceModel(model);
    mFilteredHistoryDateModel->setSortRole(HistoryModel::SortableDateRole);
    mFilteredHistoryDateModel->sort(0, Qt::DescendingOrder);
    mFilteredHistoryDateModel->setDynamicSortFilter(true);
}

void QMLWindow::setHistoryPlaceModel(HistoryModel *model)
{
    mFilteredHistoryPlaceModel->setSourceModel(model);
    mFilteredHistoryPlaceModel->setSortRole(HistoryModel::SortableTimeRole);
    mFilteredHistoryPlaceModel->setDynamicSortFilter(true);
}

void QMLWindow::setAtlasModel(AtlasIndexModel *model)
{
    mFilteredAtlasModel->setSourceModel(model);
}

void QMLWindow::writeNewObservation(const QString &data)
{
    mDataWriter->writeNewObservation(data);//, *mLocationModel, *mPersonModel);
}

void QMLWindow::loadObservation(const QString &id)
{
    qlonglong idNum = id.toLongLong();
    QString data = mDataLoader->loadObservation(idNum);
    QMetaObject::invokeMethod(mRootObject, "dataLoaded",
             Q_ARG(QVariant, data));
}

void QMLWindow::deleteObservation(const QString &id, const QString &date, const QString &place)
{
    qlonglong idNum = id.toLongLong();
    mDataWriter->deleteObservation(idNum);
    setProcessing(true);
    loadHistoryWithDateAndPlace(date, place);
    loadHistoryWithDate(date);
    reloadHistory();
    setProcessing(false);
}

void QMLWindow::saveDetailLevel(int level)
{
    mSettings->setDetailLevel(level);
}

void QMLWindow::saveSystematicSorting(bool systematic)
{
    mSettings->setSystematicSorting(systematic);
}

void QMLWindow::exportData(bool onlyNew)
{
    setProcessing(true);
    mDataWriter->exportHistory(onlyNew,mLocationModel,mPersonModel,mBirdModel);
    setProcessing(false);
}

void QMLWindow::exportOwnData()
{
    setProcessing(true);
    mDataWriter->exportOwnData(mLocationModel,mPersonModel,mBirdModel);
    setProcessing(false);
}

void QMLWindow::importOwnData()
{
    setProcessing(true);
    int err = mDataWriter->importOwnData(mLocationModel,mPersonModel,mBirdModel,mStatusModel);
    setProcessing(false);
    QMetaObject::invokeMethod(mRootObject, "importError",
             Q_ARG(QVariant,err));
}

void QMLWindow::importData()
{
    setProcessing(true);
    int err = mDataWriter->importHistory(mLocationModel, mPersonModel, mBirdModel);
    if (err&XemaEnums::IMPORT_HISTORY_OK) {
        reloadHistory();
    }
    QMetaObject::invokeMethod(mRootObject, "importError",
             Q_ARG(QVariant,err));
    setProcessing(false);
}


void QMLWindow::openBrowser( const QString &url )
{
    qDebug() << "void QMLWindow::openBrowser( const QString url )" << url;
    QDesktopServices::openUrl(QUrl(url));
}

void QMLWindow::setProcessing(bool processing) {
    mRootObject->setProperty( "cppProcessing", processing );
}
