#include <QDebug>
#include <QSettings>
#include <QStringList>

#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}

bool Settings::firstStart()
{
    QSettings settings("TeSi", "xema");
    bool firstStartup = settings.value("firstStartup", true).toBool();
    return firstStartup;
}

void Settings::setFirstStart(bool first)
{
    QSettings settings("TeSi", "xema");
    settings.setValue("firstStartup", first);
}

void Settings::resetSettings()
{
    QSettings settings("TeSi", "xema");
    settings.remove("detailLevel");
    settings.remove("systematicSorting");
    settings.remove("");
}

void Settings::setSystematicSorting(bool systematic)
{
    qDebug() << "void Settings::setSorting()";
    QSettings settings("TeSi", "xema");
    settings.setValue("systematicSorting", systematic);
}

void Settings::setDetailLevel(int level)
{
    QSettings settings("TeSi", "xema");
    settings.setValue("detailLevel", level);
}

bool Settings::systematicSorting()
{
    QSettings settings("TeSi", "xema");
    bool value = settings.value("systematicSorting", false).toBool();
    return value;
}

int Settings::detailLevel()
{
    QSettings settings("TeSi", "xema");
    int value = settings.value("detailLevel", 1).toInt();
    return value;
}

int Settings::speciesVersion()
{
    QSettings settings("TeSi", "xema");
    int value = settings.value("speciesVersion", 0).toInt();
    return value;
}

int Settings::locationsVersion()
{
    QSettings settings("TeSi", "xema");
    int value = settings.value("locationsVersion", 0).toInt();
    return value;
}

int Settings::statusesVersion()
{
    QSettings settings("TeSi", "xema");
    int value = settings.value("statusesVersion", 0).toInt();
    return value;
}

void Settings::setSpeciesVersion(int ver)
{
    QSettings settings("TeSi", "xema");
    settings.setValue("speciesVersion", ver);
}

void Settings::setLocationsVersion(int ver)
{
    QSettings settings("TeSi", "xema");
    settings.setValue("locationsVersion", ver);
}

void Settings::setStatusesVersion(int ver)
{
    QSettings settings("TeSi", "xema");
    settings.setValue("statusesVersion", ver);
}
