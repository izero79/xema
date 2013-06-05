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
    bool onlyDefaultCountry();
    bool askForConnection();
    static bool exportWgs();
    static QString defaultCountry();
    static int speciesVersion();
    static int locationsVersion();
    static int statusesVersion();
    static QString lang();

    void setFirstStart(bool first);
    void setSystematicSorting(bool systematic);
    void setDetailLevel(int level);
    void setWPSpecies(bool wpspecies);
    void setOnlyDefaultCountry(bool onlyDefault);
    void setDefaultCountry(const QString &country);
    void setExportWgs(bool exportWgs);
    static void setSpeciesVersion(int ver);
    static void setLocationsVersion(int ver);
    static void setStatusesVersion(int ver);
    void askForConnection(bool ask);

    void resetSettings();

signals:

};

#endif // SETTINGS_H
