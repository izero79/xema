#include <QTimer>
#include <QDebug>

#include <QCoreApplication>
#include "applicationcontroller.h"
#include "qmlwindow.h"
#include "birdmodel.h"
#include "locationmodel.h"
#include "personmodel.h"
#include "modeldataloader.h"
#include "modeldatawriter.h"
#include "statusmodel.h"
#include "historymodel.h"
#include "atlasindexmodel.h"
#include "agemodel.h"
#include "dressmodel.h"
#include "sexmodel.h"

ApplicationController::ApplicationController(QObject *parent) :
    QObject(parent),
    mBirdModel(0),
    mPersonModel(0),
    mLocationModel(0),
    mStatusModel(0),
    mHistoryModel(0),
    mHistoryDateModel(0),
    mHistoryPlaceModel(0),
    mAtlasModel(0),
    mModelLoader(0),
    mModelWriter(0),
    mAgeModel(0),
    mDressModel(0),
    mSexModel(0)
{
    initGUI();
    QTimer::singleShot(0, this, SLOT(initObjects()));
}

void ApplicationController::initGUI()
{
    mQMLWin = new QMLWindow();
#if defined(DESKTOPWINDOW)
    mQMLWin->showNormal();
#else
    mQMLWin->showFullScreen();
#endif

#if defined(Q_WS_MAEMO_5)
    mQMLWin->setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
    connect(mQMLWin,SIGNAL(quit()),this,SLOT(quit()));
}

void ApplicationController::initObjects()
{
    mQMLWin->init();

    mModelLoader = ModelDataLoader::instance();

    mBirdModel = new BirdModel(this);
    mModelLoader->loadBirdData(mBirdModel);
    mQMLWin->setBirdModel(mBirdModel);

    mPersonModel = new PersonModel(this);
    mModelLoader->loadPersonData(mPersonModel);
    mQMLWin->setPersonModel(mPersonModel);

    mLocationModel = new LocationModel(this);
    mModelLoader->loadLocationData(mLocationModel);
    mQMLWin->setLocationModel(mLocationModel);

    mStatusModel = new StatusModel(this);
    mModelLoader->loadStatusData(mStatusModel);
    mQMLWin->setStatusModel(mStatusModel);

    mAgeModel = new AgeModel(this);
    mModelLoader->loadAgeData(mAgeModel);
    mQMLWin->setAgeModel(mAgeModel);

    mSexModel = new SexModel(this);
    mModelLoader->loadSexData(mSexModel);
    mQMLWin->setSexModel(mSexModel);

    mDressModel = new DressModel(this);
    mModelLoader->loadDressData(mDressModel);
    mQMLWin->setDressModel(mDressModel);

    mHistoryPlaceModel = new HistoryModel(this);
    mModelLoader->loadHistoryPlaceData(mHistoryPlaceModel);
    mQMLWin->setHistoryPlaceModel(mHistoryPlaceModel);

    mHistoryDateModel = new HistoryModel(this);
    mModelLoader->loadHistoryDateData(mHistoryDateModel);
    mQMLWin->setHistoryDateModel(mHistoryDateModel);

    mHistoryModel = new HistoryModel(this);
    mModelLoader->loadHistoryData(mHistoryModel);
    mQMLWin->setHistoryModel(mHistoryModel);

    mAtlasModel = new AtlasIndexModel(this);
    mModelLoader->loadAtlasData(mAtlasModel);
    mQMLWin->setAtlasModel(mAtlasModel);

    connect(mQMLWin,SIGNAL(reloadHistory()),this,SLOT(reloadHistory()));
    connect(mQMLWin,SIGNAL(loadHistoryWithDate(QString)),this,SLOT(loadHistoryWithDate(QString)));
    connect(mQMLWin,SIGNAL(loadHistoryWithDateAndPlace(QString,QString)),this,SLOT(loadHistoryWithDateAndPlace(QString,QString)));
    connect(mQMLWin,SIGNAL(restoreObservers()),this,SLOT(restoreObservers()));
    connect(mQMLWin,SIGNAL(restoreLocations()),this,SLOT(restoreLocations()));
    connect(mQMLWin,SIGNAL(restoreSpecies()),this,SLOT(restoreSpecies()));
    connect(mQMLWin,SIGNAL(saveLocations()),this,SLOT(saveLocations()));
    connect(mQMLWin,SIGNAL(importData()),this,SLOT(importData()));

    mModelWriter = ModelDataWriter::instance();
}

ApplicationController::~ApplicationController()
{
    mQMLWin->deleteLater();
    mQMLWin = 0;
    qDebug() << "ApplicationController::~ApplicationController()" << "\n\nApplication Finished";
    qDebug() << QDateTime::currentDateTime() << "\n\n";
}

void ApplicationController::reloadHistory()
{
    mHistoryModel->clear();
    mModelLoader->loadHistoryData(mHistoryModel);
    mHistoryDateModel->clear();
    mModelLoader->loadHistoryDateData(mHistoryDateModel);
    mHistoryPlaceModel->clear();
    mModelLoader->loadHistoryPlaceData(mHistoryPlaceModel);
}

void ApplicationController::loadHistoryWithDate(const QString &date)
{
    mHistoryPlaceModel->clear();
    mModelLoader->loadHistoryPlaceData(mHistoryPlaceModel, date);
}

void ApplicationController::loadHistoryWithDateAndPlace(const QString &date, const QString &place)
{
    mHistoryModel->clear();
    mModelLoader->loadHistoryData(mHistoryModel, date, place);
}

void ApplicationController::quit()
{
    mModelWriter->writePersonData(mPersonModel);
    mModelWriter->writeLocationData(mLocationModel);
    mModelWriter->writeBirdData(mBirdModel);
    mModelWriter->writeStatusData(mStatusModel);
    qApp->quit();
}

void ApplicationController::restoreSpecies()
{
//    qDebug() << "void ApplicationController::restoreSpecies()";
    mModelWriter->removeCustomSpecies();
    mBirdModel->clear();
    mModelLoader->loadBirdData(mBirdModel);
}

void ApplicationController::restoreLocations()
{
//    qDebug() << "void ApplicationController::restoreLocations()";
    mModelWriter->removeCustomLocations();
    mLocationModel->clear();
    mModelLoader->loadLocationData(mLocationModel);
}

void ApplicationController::restoreObservers()
{
//    qDebug() << "void ApplicationController::restoreObservers()";
    mModelWriter->removeCustomObservers();
    mPersonModel->clear();
    mModelLoader->loadPersonData(mPersonModel);
}

void ApplicationController::saveLocations()
{
    qDebug() << "void ApplicationController::saveLocations()";
    mModelWriter->writeLocationData(mLocationModel);
    mLocationModel->clear();
    mModelLoader->loadLocationData(mLocationModel);

}

void ApplicationController::importData()
{
    mModelWriter->importHistory(mLocationModel, mPersonModel, mBirdModel);
    reloadHistory();
}

