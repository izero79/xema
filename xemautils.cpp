#include <QDebug>
#include <QDir>

#include "xemautils.h"

XemaUtils::XemaUtils()
{
}

QString XemaUtils::dataFileDir()
{
    QString appPath;
#ifdef Q_OS_SYMBIAN
    appPath = QCoreApplication::applicationDirPath();
#elif defined HARMATTAN
    appPath = QString("/home/user/MyDocs/.xema/");
#elif defined MAC_OS_X_VERSION_10_6
    appPath = QString("/Users/Tero/xema/");
#else
    appPath = QString("C:/");

#endif
    return appPath;
}

QString XemaUtils::exportDir()
{
    QString appPath = QString("xema/");
    QString exportDir = appPath + QString("exported/");
    QString basePath;
#ifdef Q_OS_SYMBIAN
    if (QFile::exists("E:/")) {
        basePath = QString("E:/");
    }
    else {
        basePath = QString("C:/data/");
    }
#elif defined HARMATTAN
    basePath = QString("/home/user/MyDocs/");
#elif defined MAC_OS_X_VERSION_10_6
    basePath = QString("/Users/Tero/");
#else
    basePath = QString("C:/");
#endif


    return basePath+exportDir;
}

QString XemaUtils::importDir()
{
    QString appPath = QString("xema/");
    QString importDir = appPath + QString("import/");
    QString basePath;
#ifdef Q_OS_SYMBIAN
    if (QFile::exists("E:/")) {
        basePath = QString("E:/");
    }
    else {
        basePath = QString("C:/data/");
    }
#elif defined HARMATTAN
    basePath = QString("/home/user/MyDocs/");
#elif defined MAC_OS_X_VERSION_10_6
    basePath = QString("/Users/Tero/");
#else
    basePath = QString("C:/");
#endif


    return basePath+importDir;
}

QString XemaUtils::importedDir()
{
    QString appPath = QString("xema/");
    QString importDir = appPath + QString("imported/");
    QString basePath;
#ifdef Q_OS_SYMBIAN
    if (QFile::exists("E:/")) {
        basePath = QString("E:/");
    }
    else {
        basePath = QString("C:/data/");
    }
#elif defined HARMATTAN
    basePath = QString("/home/user/MyDocs/");
#elif defined MAC_OS_X_VERSION_10_6
    basePath = QString("/Users/Tero/");
#else
    basePath = QString("C:/");
#endif


    return basePath+importDir;
}

void XemaUtils::checkAndCreateDirs() {
    qDebug() << "checkAndCreateDirs";
    QString appPath = QString("xema/");
    QString exportDir = appPath + QString("exported/");
    QString importDir = appPath + QString("import/");
    QString importedDir = appPath + QString("imported/");
    QString basePath;
#ifdef Q_OS_SYMBIAN
    if (QFile::exists("E:/")) {
        basePath = QString("E:/");
    }
    else {
        basePath = QString("C:/data/");
    }


#elif defined HARMATTAN
    basePath = QString("/home/user/MyDocs/");
    if (QFile::exists(basePath+".xema/") == false) {
        QDir dir;
        dir.mkpath(basePath+".xema/");
    }

#elif defined MAC_OS_X_VERSION_10_6
    basePath = QString("/Users/Tero/");
#else
    basePath = QString("C:/");
#endif
    QDir dir;

    if (QFile::exists(basePath+exportDir) == false) {
        dir.mkpath(basePath+exportDir);
    }
    if (QFile::exists(basePath+importDir) == false) {
        dir.mkpath(basePath+importDir);
    }
    if (QFile::exists(basePath+importedDir) == false) {
        dir.mkpath(basePath+importedDir);
    }

}
