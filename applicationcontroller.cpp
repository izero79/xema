#include <QTimer>
#include <QDebug>

#include "applicationcontroller.h"
#include "qmlwindow.h"
#include "birdmodel.h"
#include "locationmodel.h"
#include "personmodel.h"
#include "modeldataloader.h"

ApplicationController::ApplicationController(QObject *parent) :
    QObject(parent),
    mBirdModel( 0 ),
    mPersonModel( 0 ),
    mLocationModel( 0 )
{
    initGUI();
    QTimer::singleShot( 0, this, SLOT(initObjects()) );
}

void ApplicationController::initGUI()
{
    mQMLWin = new QMLWindow();
#if defined( DESKTOPWINDOW )
    mQMLWin->showNormal();
#else
    mQMLWin->showFullScreen();
#endif

#if defined(Q_WS_MAEMO_5)
    mQMLWin->setAttribute( Qt::WA_Maemo5StackedWindow );
#endif
}

void ApplicationController::initObjects()
{
    mQMLWin->init();

    mBirdModel = new BirdModel(this);
    ModelDataLoader::loadBirdData( mBirdModel );
    mQMLWin->setBirdModel( mBirdModel );

    mPersonModel = new PersonModel(this);
    ModelDataLoader::loadPersonData( mPersonModel );
    mQMLWin->setPersonModel( mPersonModel );

    mLocationModel = new LocationModel(this);
    ModelDataLoader::loadLocationData( mLocationModel );
    mQMLWin->setLocationModel( mLocationModel );

}

ApplicationController::~ApplicationController()
{
    qDebug() << "ApplicationController::~ApplicationController()";
    mQMLWin->deleteLater();
    mQMLWin = 0;
    qDebug() << "ApplicationController::~ApplicationController()" << "\n\nApplication Finished";
    qDebug() << QDateTime::currentDateTime() << "\n\n";
}

