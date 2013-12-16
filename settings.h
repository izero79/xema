#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QPair>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool firstTiiraEdit READ firstTiiraEdit WRITE setFirstTiiraEdit)
    Q_PROPERTY(bool firstTiiraDelete READ firstTiiraDelete WRITE setFirstTiiraDelete)
    Q_PROPERTY(bool useTiira READ useTiira WRITE setUseTiira)
    Q_PROPERTY(bool tiiraAutosave READ tiiraAutosave WRITE setTiiraAutosave)
    Q_PROPERTY(bool tiiraDebugMode READ tiiraDebugMode WRITE setTiiraDebugMode)
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

    static bool useTiira();
    static QString tiiraUsername();
    static QString tiiraPwdHash();
    bool tiiraAutosave();
    static int tiiraServer();
    static QString tiiraServerUsername();
    static QString tiiraServerPassword();
    bool tiiraLoginOk();
    static QString tiiraSavername();
    static bool tiiraDebugMode();

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
    void setTiiraSavername(const QString &user);
    void setTiiraDebugMode(bool debug);

    void resetSettings();

    bool firstTiiraEdit() const;
    bool firstTiiraDelete() const;

    void setFirstTiiraEdit(bool value);
    void setFirstTiiraDelete(bool value);

signals:

};

#endif // SETTINGS_H
