#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include <QDebug>
#include "applicationcontroller.h"
#include <QLabel>

#include "debugprinter.h"
#include "dummydebugprinter.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{/*
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/lokki/main.qml"));
    viewer->showExpanded();

    return app->exec();*/
    QApplication app(argc, argv);

#if defined ( Q_WS_MAEMO_5 ) || defined( Q_OS_SYMBIAN )

    QLabel label( 0, Qt::SplashScreen|Qt::WindowStaysOnTopHint );
    QPixmap pixmap(":/qml/splash_240x200.png");

    QPalette pal;
    pal.setColor( QPalette::Window, QColor(0,0,0));
    label.setPalette( pal );
    label.setAutoFillBackground(true);
    label.setPixmap(pixmap);
    label.setAlignment( Qt::AlignCenter );
    label.showFullScreen();
    label.raise();
    app.processEvents();
#endif

#ifdef DEBUGONLYTOFILE
    // Check if there's a debuglog and remove it
    bool fileExists = false;
    #if defined( Q_OS_SYMBIAN )
        fileExists = QFile::exists("c:/data/lokki-debuglog.txt");
        QFile logFile("c:/data/lokki-debuglog.txt");
        logFile.remove();
    #elif defined( Q_WS_MAEMO_5 ) || defined( HARMATTAN )
        fileExists = QFile::exists("/home/user/MyDocs/lokki-debuglog.txt");
        QFile logFile("/home/user/MyDocs/lokki-debuglog.txt");
        logFile.remove();
    #endif
    //Lets register our custom handler, before we start
        if( fileExists == true )
        {
            qInstallMsgHandler(DebugPrinter);
        }
        else
        {
            qInstallMsgHandler(DummyDebugPrinter);
        }
#endif
    qDebug() << "\n\nApplication Start";
    qDebug() << QDateTime::currentDateTime() << "\n";

    QString locale = QLocale::system().name();

    qDebug() << "System locale:" << locale;
/*
    QString lang = SystemInfoProvider::currentLanguage();
    if( lang.isEmpty() == false && lang != locale )
    {
        locale = lang;
    }

    qDebug() << "System language:" << locale;

    QTranslator translator;
    bool ok = translator.load(QString(":/lokki_" ) + locale);
    qDebug() << "Translator loaded ok:" << ok;
    app.installTranslator(&translator);
*/
    ApplicationController cont;
//    app.installEventFilter(&cont); // Installing the event filter

    return app.exec();
}
