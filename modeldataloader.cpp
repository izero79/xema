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
#include "lokkiconstants.h"
#include "atlasindex.h"
#include "atlasindexmodel.h"

ModelDataLoader::ModelDataLoader(QObject *parent) :
    QObject(parent)
{
}

void ModelDataLoader::loadBirdData( BirdModel *model )
{
    QFile tiedosto( filePath + "lokkitestibirds.txt" );
    if( tiedosto.exists() == false )
    {
        tiedosto.setFileName( ":lajilista.csv");
    }
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
        Bird bird( birdLine.section( ';', 0, 0 ).toInt(), birdLine.section( ';', 1, 1 ), birdLine.section( ';', 2, 2 ),
                   birdLine.section( ';', 3, 3 ), birdLine.section( ';', 4, 4 ), birdLine.section( ';', 5, 5 ),
                   birdLine.section( ';', 7, 7 ), birdLine.section( ';', 6, 6 ), birdLine.section( ';', 8, 8 ) );
        model->addItem( bird );
    }
}

void ModelDataLoader::loadLocationData(LocationModel *model)
{
    QFile tiedosto( filePath + "lokkitestilocation.txt" );
    if( tiedosto.exists() == false )
    {
        tiedosto.setFileName( ":paikkalista.csv");
    }
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
    QFile tiedosto( filePath + "lokkitestiperson.txt" );
    if( tiedosto.exists() == false )
    {
        tiedosto.setFileName( ":ihmislista.csv");
    }
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    while( striimi.atEnd() == false )
    {
        QString personLine;
        personLine = striimi.readLine();
        bool registered = false;
        if( personLine.section( ';', 2, 2 ) == "true" )
        {
            registered = true;
        }
        bool defaultName = false;
        if( personLine.section( ';', 3, 3 ) == "true" )
        {
            defaultName = true;
        }
        if( personLine.isEmpty() == false && personLine.contains( ";" ) )
        {
            Person person( personLine.section( ';', 0, 0 ), personLine.section( ';', 1, 1 ), registered, defaultName );
            qDebug() << "person loaded";
            model->addItem( person );
        }
    }
    tiedosto.close();
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
    qDebug() << "loadHistoryData 0";
    QFile tiedosto( filePath + "lokkitesti.txt" );
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    qDebug() << "loadHistoryData 1";
    if( striimi.atEnd() == false )
    {
        striimi.readLine();
    }
    qDebug() << "loadHistoryData 2";
    while( striimi.atEnd() == false )
    {
        QString line;
        line = striimi.readLine();
        HistoryItem item( line.section( '#', 0, 0 ).toLongLong(), line.section( '#', 6, 6 ), line.section( '#', 2, 2 ) );
        model->addItem( item );
    }
    qDebug() << "loadHistoryData done";
}

void ModelDataLoader::loadAtlasData( AtlasIndexModel *model )
{
    QFile tiedosto( ":atlasindex.csv");
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
        if( line.isEmpty() == false )
        {
            AtlasIndex index( line.section( ';', 0, 0 ).toInt() );
            model->addItem( index );
        }
    }
}
