#include <QTimer>
#include <QDebug>
#include <QThreadPool>
#include <QRunnable>

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
#include "systeminfoprovider.h"
#include "directionmodel.h"
#include "settings.h"
#include "dataloader.h"
#include "xemaenums.h"


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
    mSexModel(0),
    mDirectionModel(0),
    mSettings(0)
{
    qDebug() << "\n\nSystem Info:" << SystemInfoProvider::versionInfo() << "\n\n";
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

    mSettings = new Settings(this);
    bool finOnly = !mSettings->wpSpecies();
    mBirdModel = new BirdModel(this);
    mModelLoader->loadInitialBirdData(mBirdModel, finOnly);
    mQMLWin->setBirdModel(mBirdModel);

    mPersonModel = new PersonModel(this);
    mModelLoader->loadPersonData(mPersonModel);
    mQMLWin->setPersonModel(mPersonModel);

    mLocationModel = new LocationModel(this);
    mModelLoader->loadInitialLocationData(mLocationModel);
    mQMLWin->setLocationModel(mLocationModel);

    mStatusModel = new StatusModel(this);
    mModelLoader->loadInitialStatusData(mStatusModel);
    mQMLWin->setStatusModel(mStatusModel);

    mAgeModel = new AgeModel(this);
    mModelLoader->loadAgeData(mAgeModel);
    mQMLWin->setAgeModel(mAgeModel);

    mSexModel = new SexModel(this);
    mModelLoader->loadSexData(mSexModel);
    mQMLWin->setSexModel(mSexModel);

    mDirectionModel = new DirectionModel(this);
    mModelLoader->loadDirectionData(mDirectionModel);
    mQMLWin->setDirectionModel(mDirectionModel);

    mDressModel = new DressModel(this);
    mModelLoader->loadDressData(mDressModel);
    mQMLWin->setDressModel(mDressModel);


    mHistoryPlaceModel = new HistoryModel(this);
    mQMLWin->setHistoryPlaceModel(mHistoryPlaceModel);


    qRegisterMetaType<QModelIndex>("QModelIndex");


    mHistoryDateModel = new HistoryModel(this);
/*
    DataLoader* tLoad = new DataLoader(XemaEnums::LOAD_HISTORY_DATE);
    connect(tLoad,SIGNAL(historyLoaded()),mQMLWin,SLOT(setProcessingFalse()));
    tLoad->setHistoryModel(mHistoryDateModel);
    QThreadPool::globalInstance()->start(tLoad);
*/
    mModelLoader->loadHistoryDateData(mHistoryDateModel);
    mQMLWin->setHistoryDateModel(mHistoryDateModel);

    mHistoryModel = new HistoryModel(this);
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
    connect(mQMLWin,SIGNAL(restoreStatuses()),this,SLOT(restoreStatuses()));
    connect(mQMLWin,SIGNAL(saveLocations()),this,SLOT(saveLocations()));
    connect(mQMLWin,SIGNAL(savePersons()),this,SLOT(savePersons()));
    connect(mQMLWin,SIGNAL(saveBirds()),this,SLOT(saveBirds()));
    connect(mQMLWin,SIGNAL(clearHistory()),this,SLOT(clearHistory()));
    connect(mQMLWin,SIGNAL(reloadBirds()),this,SLOT(reloadBirds()));

    mModelWriter = ModelDataWriter::instance();
    mQMLWin->setProcessing(false);
}

ApplicationController::~ApplicationController()
{
    mModelWriter->writePersonData(mPersonModel);
    mModelWriter->writeLocationData(mLocationModel);
    mModelWriter->writeBirdData(mBirdModel);
    mModelWriter->writeStatusData(mStatusModel);
    mQMLWin->deleteLater();
    mQMLWin = 0;
    qDebug() << "ApplicationController::~ApplicationController()" << "\n\nApplication Finished";
    qDebug() << QDateTime::currentDateTime() << "\n\n";
}

void ApplicationController::reloadHistory()
{
    mQMLWin->setProcessing(true);
    mHistoryDateModel->clear();
    mModelLoader->loadHistoryDateData(mHistoryDateModel);
    mQMLWin->setProcessing(false);
}

void ApplicationController::loadHistoryWithDate(const QString &date)
{
    mQMLWin->setProcessing(true);
    mHistoryPlaceModel->clear();
    mModelLoader->loadHistoryPlaceData(mHistoryPlaceModel, date);
    mQMLWin->setProcessing(false);
}

void ApplicationController::loadHistoryWithDateAndPlace(const QString &date, const QString &place)
{
    mQMLWin->setProcessing(true);
    mHistoryModel->clear();
    mModelLoader->loadHistoryData(mHistoryModel, date, place);
    mQMLWin->setProcessing(false);
}

void ApplicationController::quit()
{
//    mModelWriter->writePersonData(mPersonModel);
//    mModelWriter->writeLocationData(mLocationModel);
//    mModelWriter->writeBirdData(mBirdModel);
//    mModelWriter->writeStatusData(mStatusModel);
    qApp->quit();
}

void ApplicationController::restoreSpecies()
{
    qDebug() << "void ApplicationController::restoreSpecies()";
    mQMLWin->setProcessing(true);
    mModelWriter->removeCustomSpecies();
    mBirdModel->clear();
    bool finOnly = !mSettings->wpSpecies();
    mModelLoader->loadDefaultBirdData(mBirdModel, finOnly);
    mQMLWin->setProcessing(false);
}

void ApplicationController::restoreLocations()
{
    qDebug() << "void ApplicationController::restoreLocations()";
    mQMLWin->setProcessing(true);
    mModelWriter->removeCustomLocations();
    mLocationModel->clear();
    mModelLoader->loadDefaultLocationData(mLocationModel);
    mQMLWin->setProcessing(false);
}

void ApplicationController::restoreObservers()
{
    qDebug() << "void ApplicationController::restoreObservers()";
    mQMLWin->setProcessing(true);
    mModelWriter->removeCustomObservers();
    mPersonModel->clear();
    mModelLoader->loadPersonData(mPersonModel);
    mQMLWin->setProcessing(false);
}

void ApplicationController::restoreStatuses()
{
    qDebug() << "void ApplicationController::restoreStatuses()";
    mQMLWin->setProcessing(true);
    mModelWriter->removeCustomStatuses();
    mStatusModel->clear();
    mModelLoader->loadDefaultStatusData(mStatusModel);
    mQMLWin->setProcessing(false);
}

void ApplicationController::clearHistory()
{
    qDebug() << "void ApplicationController::clearHistory()";
    mQMLWin->setProcessing(true);
    mModelWriter->removeHistory();
    mHistoryModel->clear();
    mHistoryDateModel->clear();
    mHistoryPlaceModel->clear();
    mModelLoader->loadHistoryDateData(mHistoryDateModel);
    mQMLWin->setProcessing(false);
}

void ApplicationController::reloadBirds()
{
    qDebug() << "void ApplicationController::reloadBirds()";
    mQMLWin->setProcessing(true);
    mBirdModel->clear();
    bool finOnly = !mSettings->wpSpecies();
    mModelLoader->reloadInitialBirdData(mBirdModel, finOnly);
    mQMLWin->setProcessing(false);
}

void ApplicationController::saveLocations()
{
    qDebug() << "void ApplicationController::saveLocations()";
    mModelWriter->writeLocationData(mLocationModel);
    mLocationModel->clear();
    mModelLoader->loadInitialLocationData(mLocationModel);

}

void ApplicationController::savePersons()
{
    qDebug() << "void ApplicationController::savePersons()";
    mModelWriter->writePersonData(mPersonModel);
    mPersonModel->clear();
    mModelLoader->loadPersonData(mPersonModel);

}

void ApplicationController::saveBirds()
{
    qDebug() << "void ApplicationController::saveBirds()";
    mModelWriter->writeBirdData(mBirdModel);
    mBirdModel->clear();
    bool finOnly = !mSettings->wpSpecies();
    mModelLoader->loadInitialBirdData(mBirdModel, finOnly);

}
