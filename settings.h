#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QPair>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    bool firstStart();
    bool systematicSorting();
    int detailLevel();
    static int speciesVersion();
    static int locationsVersion();
    static int statusesVersion();

    void setFirstStart(bool first);
    void setSystematicSorting(bool systematic);
    void setDetailLevel(int level);
    static void setSpeciesVersion(int ver);
    static void setLocationsVersion(int ver);
    static void setStatusesVersion(int ver);

    void resetSettings();

signals:

public slots:

};

#endif // SETTINGS_H
