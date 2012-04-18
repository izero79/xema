#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>
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
#include "xemaconstants.h"
#include "atlasindex.h"
#include "atlasindexmodel.h"

ModelDataLoader::ModelDataLoader(QObject *parent) :
    QObject(parent)
{
}

void ModelDataLoader::loadBirdData( BirdModel *model )
{
    if( QFile::exists( dataFileDir() + "lokkitestibirds.txt" ))
    {
        QFile oldFile( dataFileDir() + "lokkitestibirds.txt" );
        oldFile.rename( dataFileDir() + "xemabirddata.txt" );
    }
    QFile tiedosto( dataFileDir() + "xemabirddata.txt" );
    if( tiedosto.exists() == false )
    {
        tiedosto.setFileName( ":specieslist.csv");
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
    if( QFile::exists( dataFileDir() + "lokkitestilocation.txt" ))
    {
        QFile oldFile( dataFileDir() + "lokkitestilocation.txt" );
        oldFile.rename( dataFileDir() + "xemalocationdata.txt" );
    }

    QFile tiedosto( dataFileDir() + "xemalocationdata.txt" );
    if( tiedosto.exists() == false )
    {
        tiedosto.setFileName( ":defaultlocations.csv");
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
    if( QFile::exists( dataFileDir() + "lokkitestiperson.txt" ))
    {
        QFile oldFile( dataFileDir() + "lokkitestiperson.txt" );
        oldFile.rename( dataFileDir() + "xemapersondata.txt" );
    }

    QFile tiedosto( dataFileDir() + "xemapersondata.txt" );
    if( tiedosto.exists() == false )
    {
        tiedosto.setFileName( ":defaultpersons.csv");
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
            model->addItem( person );
        }
    }
    tiedosto.close();
}

void ModelDataLoader::loadStatusData( StatusModel *model )
{
    QFile tiedosto( ":defaultstatuses.csv");
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

void ModelDataLoader::loadHistoryData( HistoryModel *model, const QString &date, const QString &place )
{
//    QFile tiedosto( dataFileDir() + "xemadata.txt" );
    QFile tiedosto( dataFileDir() + "lokkitesti.txt" );
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
        QString readPlace = line.section( '#', 6, 6 );
        QString readDate = line.section( '#', 2, 2);
        QString readTime = line.section( '#', 4, 4);
        int rowCount = line.section( '#', 12, 12).toInt();
//        qDebug() << "Riveja" << rowCount;
        QString readCount = 0;
        if( rowCount == 1 )
        {
            readCount = line.section( '#', 13, 13);
        }
        else
        {
            int count = 0;//line.section( '#', 13, 13).toInt();
            for( int i = 0; i < rowCount;i++)
            {
                count += line.section( '#', 13+(i*11),13+(i*11)).toInt();
            }
            readCount.setNum( count );
        }

//        qDebug() << "Kokonais lajin maara" << readCount;
        if( date.isEmpty() == true && place.isEmpty() == true )
        {
            HistoryItem item( line.section( '#', 0, 0 ).toLongLong(), line.section( '#', 6, 6 ), line.section( '#', 2, 2 ) );
            item.setSpecies( line.section( '#', 1, 1));
            item.addSpeciesCount( line.section( '#', 1, 1), readCount.toInt() );
            item.setTime(readTime);

            model->addItemAtBeginning( item );
            continue;
        }
        if( date.isEmpty() == false && date == readDate && place.isEmpty() == false && place == readPlace )
        {
            HistoryItem item( line.section( '#', 0, 0 ).toLongLong(), line.section( '#', 6, 6 ), line.section( '#', 2, 2 ) );
            item.setSpecies( line.section( '#', 1, 1));
            item.addSpeciesCount( line.section( '#', 1, 1), readCount.toInt() );
            item.setTime(readTime);
            model->addItemAtBeginning( item );
            continue;
        }
        // TODO other combinations
    }
}

void ModelDataLoader::loadHistoryDateData( HistoryModel *model )
{
    //    QFile tiedosto( dataFileDir() + "xemadata.txt" );
        QFile tiedosto( dataFileDir() + "lokkitesti.txt" );
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
        QString date = line.section( '#', 2, 2 );
        QString readTime = line.section( '#', 4, 4 );
        QString species = line.section( '#', 1, 1);
        int rowCount = line.section( '#', 12, 12).toInt();
//        qDebug() << "Riveja" << rowCount;
        QString readCount = 0;
        if( rowCount == 1 )
        {
            readCount = line.section( '#', 13, 13);
        }
        else
        {
            int count = 0;//line.section( '#', 13, 13).toInt();
            for( int i = 0; i < rowCount;i++)
            {
                count += line.section( '#', 13+(i*11),13+(i*11)).toInt();
            }
            readCount.setNum( count );
        }

//        qDebug() << "Kokonais lajin maara" << readCount;

        int modelRowCount = model->rowCount();
        bool sameDateFound = false;
        for( int i = 0; i < modelRowCount; i++ )
        {
//            qDebug() << "tarkistetaan" << date;
            if( model->getItem( i ).date() == date )
            {
//                qDebug() << "sama pvm" << date;
                HistoryItem tmp = model->getItem( i );
                tmp.increaseDateCount();
                tmp.addSpecies( species );
                tmp.addSpeciesCount( species, readCount.toInt());
                model->replaceItem( i, tmp);
                sameDateFound = true;
            }
        }
        if( sameDateFound == false )
        {
            HistoryItem item( line.section( '#', 0, 0 ).toLongLong(), line.section( '#', 6, 6 ), line.section( '#', 2, 2 ) );
            item.setSpecies( line.section( '#', 1, 1));
            item.addSpeciesCount( species, readCount.toInt());
            item.setTime(readTime);

            model->addItemAtBeginning( item );
        }
    }
}

void ModelDataLoader::loadHistoryPlaceData( HistoryModel *model, const QString &date )
{
    //    QFile tiedosto( dataFileDir() + "xemadata.txt" );
        QFile tiedosto( dataFileDir() + "lokkitesti.txt" );
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
        QString readDate = line.section( '#', 2, 2 );
        QString place = line.section( '#', 6, 6 );
        QString species = line.section( '#', 1, 1);

        int rowCount = line.section( '#', 12, 12).toInt();
//        qDebug() << "Riveja" << rowCount;
        QString readCount = 0;
        if( rowCount == 1 )
        {
            readCount = line.section( '#', 13, 13);
        }
        else
        {
            int count = 0;//line.section( '#', 13, 13).toInt();
            for( int i = 0; i < rowCount;i++)
            {
                count += line.section( '#', 13+(i*11),13+(i*11)).toInt();
            }
            readCount.setNum( count );
        }

//        qDebug() << "Kokonais lajin maara" << readCount;

        bool sameDateFound = false;
        bool samePlaceFound = false;
//        qDebug() << "\nuusi itemi" << readDate << place;
        for( int i = 0; i < model->rowCount(); i++ )
        {
//            qDebug() << "tarkistetaan kohta" << i;
            sameDateFound = false;
            samePlaceFound = false;
            if( model->getItem( i ).date() == readDate )
            {
//                qDebug() << "sama date" <<  model->getItem( i ).date();
                sameDateFound = true;
            }
            else
            {
//                qDebug() << "eri date  " <<  model->getItem( i ).date() << "vs" << readDate;
                continue;
            }
            if( sameDateFound && model->getItem( i ).place() == place)
            {
//                qDebug() << "kohdassa" << i << "sama place ja date" << readDate << place;
                HistoryItem tmp = model->getItem( i );
                tmp.increasePlaceCount();
                tmp.increaseDateCount();
                tmp.addSpecies( species );
                tmp.addSpeciesCount( species, readCount.toInt());
                model->replaceItem( i, tmp);
                samePlaceFound = true;
                break;
            }
        }
        if( date.isEmpty() == false )
        {
            if( readDate == date && samePlaceFound == false)
            {
//                qDebug() << "adding with date" << readDate << place;
                HistoryItem item( line.section( '#', 0, 0 ).toLongLong(), line.section( '#', 6, 6 ), line.section( '#', 2, 2 ) );
                item.setSpecies( line.section( '#', 1, 1));
                item.addSpeciesCount( species, readCount.toInt());
                model->addItemAtBeginning( item );
            }
            continue;
        }
        else if( samePlaceFound == false )
        {
            HistoryItem item( line.section( '#', 0, 0 ).toLongLong(), line.section( '#', 6, 6 ), line.section( '#', 2, 2 ) );
            item.setSpecies( line.section( '#', 1, 1));
            item.addSpeciesCount( species, readCount.toInt());
            model->addItemAtBeginning( item );
        }
    }
}

void ModelDataLoader::loadAtlasData( AtlasIndexModel *model )
{
    QFile tiedosto( ":atlasindexes.csv");
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

QString ModelDataLoader::dataFileDir()
{
    QString appPath;
#ifdef Q_OS_SYMBIAN
    appPath = QCoreApplication::applicationDirPath();
#elif defined HARMATTAN
    appPath = QString( "/home/user/MyDocs/.xema/" );
#elif defined MAC_OS_X_VERSION_10_6
    appPath = QString( "/Users/Tero/" );
#else
    appPath = QString( "C:/" );

#endif
    return appPath;
}
