#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QPair>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

public slots:
    bool firstStart();
    bool systematicSorting();
    int detailLevel();
    bool wpSpecies();
    bool askForConnection();
    static bool exportWgs();
    static bool onlyDefaultCountry();
    static QString defaultCountry();
    static bool onlyDefaultAssociation();
    static QString defaultAssociation();
    static bool alwaysShowOwn();
    static int speciesVersion();
    static int locationsVersion();
    static int statusesVersion();
    static QString lang();

    bool useTiira();
    static QString tiiraUsername();
    static QString tiiraPwdHash();
    bool tiiraAutosave();
    static int tiiraServer();
    static QString tiiraServerUsername();
    static QString tiiraServerPassword();
    bool tiiraLoginOk();

    void setFirstStart(bool first);
    void setSystematicSorting(bool systematic);
    void setDetailLevel(int level);
    void setWPSpecies(bool wpspecies);
    void setOnlyDefaultCountry(bool onlyDefault);
    void setDefaultCountry(const QString &country);
    void setOnlyDefaultAssociation(bool onlyDefault);
    void setDefaultAssociation(const QString &association);
    void setAlwaysShowOwn(bool show);
    void setExportWgs(bool exportWgs);
    static void setSpeciesVersion(int ver);
    static void setLocationsVersion(int ver);
    static void setStatusesVersion(int ver);
    void askForConnection(bool ask);

    void setUseTiira(bool useTiira);
    void setTiiraUsername(const QString &username);
    void setTiiraPwdHash(const QString &password);
    void setTiiraAutosave(bool autosave);
    void setTiiraServer(int server);
    void setTiiraServerUsername(const QString &username);
    void setTiiraServerPassword(const QString &password);
    void setTiiraLoginOk(bool ok);

    void resetSettings();

signals:

};

#endif // SETTINGS_H
