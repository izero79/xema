#include <QTimer>
#include <QDebug>

#include "applicationcontroller.h"
#include "qmlwindow.h"

ApplicationController::ApplicationController(QObject *parent) :
    QObject(parent)
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
//    mQMLWin->init();

}

ApplicationController::~ApplicationController()
{
    qDebug() << "ApplicationController::~ApplicationController()";
    mQMLWin->deleteLater();
    mQMLWin = 0;
    qDebug() << "ApplicationController::~ApplicationController()" << "\n\nApplication Finished";
    qDebug() << QDateTime::currentDateTime() << "\n\n";
}

