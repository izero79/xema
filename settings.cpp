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
    QSettings settings( "TeSi", "lokki");
    bool firstStartup = settings.value( "firstStartup", true ).toBool();
    return firstStartup;
}

void Settings::setFirstStart( bool first )
{
    QSettings settings( "TeSi", "lokki");
    settings.setValue( "firstStartup", first );
}

void Settings::resetSettings()
{
    QSettings settings( "TeSi", "lokki");
    settings.remove( "detailLevel" );
    settings.remove( "systematicSorting" );
    settings.remove( "" );
}

void Settings::setSystematicSorting( bool systematic )
{
    qDebug() << "void Settings::setSorting()";
    QSettings settings( "TeSi", "lokki");
    settings.setValue( "systematicSorting", systematic );
}

void Settings::setDetailLevel( int level )
{
    qDebug() << "void Settings::setDetailLevel()";
    QSettings settings( "TeSi", "lokki");
    settings.setValue( "detailLevel", level );
}

bool Settings::systematicSorting()
{
    QSettings settings( "TeSi", "lokki");
    bool value = settings.value( "systematicSorting", false ).toBool();
    qDebug() << "ASETUKSISTA SORT" << value;
    return value;
}

int Settings::detailLevel()
{
    QSettings settings( "TeSi", "lokki");
    int value = settings.value( "detailLevel", 1 ).toInt();
    qDebug() << "ASETUKSISTA LEVEL" << value;
    return value;
}
