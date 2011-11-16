#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "modeldataloader.h"
#include "birdmodel.h"
#include "bird.h"
#include "locationmodel.h"
#include "location.h"
#include "personmodel.h"
#include "person.h"
#include "statusmodel.h"
#include "status.h"
#include "historymodel.h"
#include "historyitem.h"

ModelDataLoader::ModelDataLoader(QObject *parent) :
    QObject(parent)
{
}

void ModelDataLoader::loadBirdData( BirdModel *model )
{
    QFile tiedosto( ":lajilista.csv");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if( striimi.atEnd() == false )
    {
        striimi.readLine();
    }
    while( striimi.atEnd() == false )
    {
        QString birdLine;
        birdLine = striimi.readLine();
        Bird bird( birdLine.section( ';', 0, 0 ).toInt(), birdLine.section( ';', 1, 1 ), birdLine.section( ';', 2, 2 ), birdLine.section( ';', 3, 3 ), birdLine.section( ';', 4, 4 ), birdLine.section( ';', 5, 5 ) );
        model->addItem( bird );
    }
}

void ModelDataLoader::loadLocationData(LocationModel *model)
{
    QFile tiedosto( ":paikkalista.csv");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    while( striimi.atEnd() == false )
    {
        QString locationLine;
        locationLine = striimi.readLine();
        Location location( locationLine.section( ';', 0, 0 ), locationLine.section( ';', 1, 1 ), locationLine.section( ';', 2, 2 ), locationLine.section( ';', 3, 3 ) );
        model->addItem( location );
    }
}

void ModelDataLoader::loadPersonData(PersonModel *model)
{
    QFile tiedosto( ":ihmislista.csv");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    while( striimi.atEnd() == false )
    {
        QString personLine;
        personLine = striimi.readLine();
        bool registered = false;
        if( personLine.section( ';', 2, 2 ) == QString( "true" ) )
        {
            registered = true;
        }
        bool defaultName = false;
        if( personLine.section( ';', 3, 3 ) == QString( "true" ) )
        {
            defaultName = true;
        }
        if( personLine.isEmpty() == false )
        {
            Person person( personLine.section( ';', 0, 0 ), personLine.section( ';', 1, 1 ), registered, defaultName );
            model->addItem( person );
        }
    }
}

void ModelDataLoader::loadStatusData( StatusModel *model )
{
    QFile tiedosto( ":statuslist.csv");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if( striimi.atEnd() == false )
    {
        striimi.readLine();
    }
    while( striimi.atEnd() == false )
    {
        QString birdLine;
        birdLine = striimi.readLine();
        if( birdLine.isEmpty() == false )
        {
            Status status( birdLine.section( ';', 0, 0 ), birdLine.section( ';', 1, 1 ) );
            model->addItem( status );
        }
    }
}

void ModelDataLoader::loadHistoryData( HistoryModel *model )
{
#ifdef Q_OS_SYMBIAN
    QFile tiedosto( "c:/data/lokkitesti.txt");
#else
    QFile tiedosto( "/Users/Tero/lokkitesti.txt");
#endif
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if( striimi.atEnd() == false )
    {
        striimi.readLine();
    }
    while( striimi.atEnd() == false )
    {
        QString line;
        line = striimi.readLine();
        HistoryItem item( line.section( '#', 0, 0 ).toLongLong(), line.section( '#', 6, 6 ), line.section( '#', 2, 2 ) );
        model->addItem( item );
    }
}
