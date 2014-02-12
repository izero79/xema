#include <QDebug>
#include <QSettings>
#include <QStringList>

#include "settings.h"
#include "tiirachecksumcalculator.h"

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

void Settings::setOnlyDefaultAssociation(bool onlyDefault)
{
    qDebug() << "void Settings::setOnlyDefaultAssociation()" << onlyDefault;
    QSettings settings("TeSi", "xema");
    settings.setValue("onlyDefaultAssociation", onlyDefault);
}

void Settings::setDefaultAssociation(const QString &association)
{
    qDebug() << "void Settings::setDefaultAssociation()" << association;
    QSettings settings("TeSi", "xema");
    settings.setValue("defaultAssociation", association);
}

void Settings::setAlwaysShowOwn(bool show)
{
    qDebug() << "void Settings::setAlwaysShowOwn()" << show;
    QSettings settings("TeSi", "xema");
    settings.setValue("alwaysShowOwn", show);
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

bool Settings::onlyDefaultCountry()
{
    QSettings settings("TeSi", "xema");
    bool value = settings.value("onlyDefaultCountry", false).toBool();
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

QString Settings::defaultAssociation()
{
    QSettings settings("TeSi", "xema");
    QString value = settings.value("defaultAssociation", "").toString();
    return value;
}

bool Settings::onlyDefaultAssociation()
{
    QSettings settings("TeSi", "xema");
    bool value = settings.value("onlyDefaultAssociation", false).toBool();
    return value;
}

bool Settings::alwaysShowOwn()
{
    QSettings settings("TeSi", "xema");
    bool value = settings.value("alwaysShowOwn", false).toBool();
    qDebug() << "alwaysShowOwn" << value;
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

bool Settings::useTiira()
{
    QSettings settings("TeSi", "xema");
    bool value = settings.value("useTiira", false).toBool();
    qDebug() << Q_FUNC_INFO << value;
    return value;
}

QString Settings::tiiraUsername()
{
    QSettings settings("TeSi", "xema");
    QString value = settings.value("tiiraUsername", "").toString();
    qDebug() << Q_FUNC_INFO << value;
    return value;
}

QString Settings::tiiraPwdHash()
{
    QSettings settings("TeSi", "xema");
    QString value = settings.value("tiiraPwdHash", "").toString();
    qDebug() << Q_FUNC_INFO << value;
    return value;
}

bool Settings::tiiraAutosave()
{
    QSettings settings("TeSi", "xema");
    bool value = settings.value("tiiraAutosave", false).toBool();
    qDebug() << Q_FUNC_INFO << value;
    return value;
}

int Settings::tiiraServer()
{
    QSettings settings("TeSi", "xema");
    int value = settings.value("tiiraServer", 0).toInt();
    qDebug() << Q_FUNC_INFO << value;
    return value;
}

QString Settings::tiiraServerUsername()
{
    QSettings settings("TeSi", "xema");
    QString value = settings.value("tiiraServerUsername", "").toString();
    qDebug() << Q_FUNC_INFO << value;
    return value;
}

QString Settings::tiiraServerPassword()
{
    QSettings settings("TeSi", "xema");
    QString value = settings.value("tiiraServerPassword", "").toString();
    qDebug() << Q_FUNC_INFO << value;
    return value;
}

bool Settings::tiiraLoginOk()
{
    QSettings settings("TeSi", "xema");
    bool value = settings.value("tiiraLoginOk", false).toBool();
    qDebug() << Q_FUNC_INFO << value;
    return value;
}

void Settings::setUseTiira(bool useTiira)
{
    qDebug() << Q_FUNC_INFO << useTiira;
    QSettings settings("TeSi", "xema");
    settings.setValue("useTiira", useTiira);
}

void Settings::setTiiraUsername(const QString &username)
{
    qDebug() << Q_FUNC_INFO << username;
    QSettings settings("TeSi", "xema");
    settings.setValue("tiiraUsername", username);
}

void Settings::setTiiraPwdHash(const QString &password)
{
    qDebug() << Q_FUNC_INFO << password;
    QString hash = TiiraChecksumCalculator::getMd5(password);
    QSettings settings("TeSi", "xema");
    settings.setValue("tiiraPwdHash", hash);
}

void Settings::setTiiraAutosave(bool autosave)
{
    qDebug() << Q_FUNC_INFO << autosave;
    QSettings settings("TeSi", "xema");
    settings.setValue("tiiraAutosave", autosave);
}

void Settings::setTiiraServer(int server)
{
    qDebug() << Q_FUNC_INFO << server;
    QSettings settings("TeSi", "xema");
    settings.setValue("tiiraServer", server);
}

void Settings::setTiiraServerUsername(const QString &username)
{
    qDebug() << Q_FUNC_INFO << username;
    QSettings settings("TeSi", "xema");
    settings.setValue("tiiraServerUsername", username);
}

void Settings::setTiiraServerPassword(const QString &password)
{
    qDebug() << Q_FUNC_INFO << password;
    QSettings settings("TeSi", "xema");
    settings.setValue("tiiraServerPassword", password);
}

void Settings::setTiiraLoginOk(bool ok)
{
    qDebug() << Q_FUNC_INFO << ok;
    QSettings settings("TeSi", "xema");
    settings.setValue("tiiraLoginOk", ok);
}

void Settings::setTiiraSavername(const QString &user)
{
    qDebug() << Q_FUNC_INFO << user;
    QSettings settings("TeSi", "xema");
    settings.setValue("tiiraSaverName", user);
}

QString Settings::tiiraSavername()
{
    QSettings settings("TeSi", "xema");
    QString value = settings.value("tiiraSaverName", "").toString();
    qDebug() << Q_FUNC_INFO << value;
    return value;
}

bool Settings::firstTiiraEdit() const {
    QSettings settings("TeSi", "xema");
    bool value = settings.value("firstTiiraEdit", true).toBool();
    qDebug() << Q_FUNC_INFO << value;
    return value;
}

bool Settings::firstTiiraDelete() const {
    QSettings settings("TeSi", "xema");
    bool value = settings.value("firstTiiraDelete", true).toBool();
    qDebug() << Q_FUNC_INFO << value;
    return value;
}

void Settings::setFirstTiiraEdit(bool value) {
    QSettings settings("TeSi", "xema");
    settings.setValue("firstTiiraEdit", value);
    qDebug() << Q_FUNC_INFO << value;
}

void Settings::setFirstTiiraDelete(bool value) {
    QSettings settings("TeSi", "xema");
    settings.setValue("firstTiiraDelete", value);
    qDebug() << Q_FUNC_INFO << value;
}

void Settings::setTiiraDebugMode(bool debug)
{
    qDebug() << Q_FUNC_INFO << debug;
    QSettings settings("TeSi", "xema");
    settings.setValue("tiiraDebugMode", debug);
}

bool Settings::tiiraDebugMode()
{
    QSettings settings("TeSi", "xema");
    bool value = settings.value("tiiraDebugMode", false).toBool();
    qDebug() << Q_FUNC_INFO << value;
    return value;
}
