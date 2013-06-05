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
    qDebug() << "void Settings::setSorting()" << systematic;
    QSettings settings("TeSi", "xema");
    settings.setValue("systematicSorting", systematic);
}

void Settings::setWPSpecies(bool wpspecies)
{
    qDebug() << "void Settings::setWPSpecies()" << wpspecies;
    QSettings settings("TeSi", "xema");
    settings.setValue("wpSpecies", wpspecies);
}

void Settings::setOnlyDefaultCountry(bool onlyDefault)
{
    qDebug() << "void Settings::setOnlyDefaultCountry()" << onlyDefault;
    QSettings settings("TeSi", "xema");
    settings.setValue("onlyDefaultCountry", onlyDefault);
}

void Settings::setDefaultCountry(const QString &country)
{
    qDebug() << "void Settings::setDefaultCountry()" << country;
    QSettings settings("TeSi", "xema");
    settings.setValue("defaultCountry", country);
}

void Settings::setExportWgs(bool exportWgs)
{
    qDebug() << "void Settings::setExportWgs()" << exportWgs;
    QSettings settings("TeSi", "xema");
    settings.setValue("exportWgs", exportWgs);
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

bool Settings::wpSpecies()
{
    QSettings settings("TeSi", "xema");
    bool value = settings.value("wpSpecies", true).toBool();
    return value;
}

bool Settings::onlyDefaultCountry()
{
    QSettings settings("TeSi", "xema");
    bool value = settings.value("onlyDefaultCountry", false).toBool();
    return value;
}

bool Settings::askForConnection()
{
    QSettings settings("TeSi", "xema");
    bool value = settings.value("askForConnection", true).toBool();
    return value;
}

bool Settings::exportWgs()
{
    QSettings settings("TeSi", "xema");
    bool value = settings.value("exportWgs", true).toBool();
    return value;
}

QString Settings::defaultCountry()
{
    QSettings settings("TeSi", "xema");
    QString userlang = lang();
    QString defaultSetting;
    if (QString::compare(userlang, "fi", Qt::CaseInsensitive) == 0) {
        defaultSetting = "Suomi";
    } else if (QString::compare(userlang, "sv", Qt::CaseInsensitive) == 0) {
        defaultSetting = "Finland";
    } else if (QString::compare(userlang, "en", Qt::CaseInsensitive) == 0) {
        defaultSetting = "Finland";
    }
    QString value = settings.value("defaultCountry", defaultSetting).toString();
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

void Settings::askForConnection(bool ask)
{
    qDebug() << "void Settings::askForConnection()" << ask;
    QSettings settings("TeSi", "xema");
    settings.setValue("askForConnection", ask);
}

QString Settings::lang()
{
//    return "en"; // for testing
    QString locale = QLocale::system().name();
    return locale.section("_",0,0);
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
