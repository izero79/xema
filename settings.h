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

    void setFirstStart( bool first );
    void setSystematicSorting( bool systematic );
    void setDetailLevel( int level );

    void resetSettings();

signals:

public slots:

};

#endif // SETTINGS_H
