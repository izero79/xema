#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>
#include <QDateTime>

#include "modeldatawriter.h"
#include "birdmodel.h"
#include "bird.h"
#include "locationmodel.h"
#include "location.h"
#include "personmodel.h"
#include "person.h"
#include "lokkiconstants.h"

ModelDataWriter::ModelDataWriter(QObject *parent) :
    QObject(parent)
{
}

void ModelDataWriter::writeNewObservation( const QString &data )
{
    if( data.section( '#', 0, 0 ) != "0" )
    {
        qDebug() << "ON JO TALLENNETTU, KORVATAAN";
        replaceObservation( data.section( '#', 0, 0 ).toLongLong(), data );
        return;
    }
    qlonglong newId = getNewId();
    qDebug() << "UUSI ID ON" << newId;

    qDebug() << "ALUNP PITAIS TALLENTAA" << data;
    QString newData = data;
    int pos = newData.indexOf( "#" );
    newData.remove( 0, pos );
    QString newIdNum;
    newIdNum.setNum( newId );
    newData.prepend( newIdNum );
    qDebug() << "NYT PITAIS TALLENTAA" << newData;
    bool headerExists = true;
    if( QFile::exists( dataFileDir() + "lokkitesti.txt" ) == false )
    {
        headerExists = false;
    }
    QFile tiedosto( dataFileDir() + "lokkitesti.txt" );
    tiedosto.open(QFile::ReadWrite|QFile::Append);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    if( headerExists == false )
    {
        QString header = QString::fromUtf8("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Paikka#Lisätietoja#Atlaskoodi#Havainnoijat#Havainnoijat2#Salattu#Rivejä#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Sää#Exported\n");
        striimi << header;
    }
    striimi << newData;
    striimi << "\n";
    tiedosto.close();
}

void ModelDataWriter::replaceObservation( qlonglong id, const QString &data )
{
    qDebug() << "KORVATAAN, ID ON" << id;
    QFile tiedosto( dataFileDir() + "lokkitesti.txt" );
    QFile tmptiedosto( dataFileDir() + "lokkitesti.tmp" );

    tiedosto.open(QFile::ReadOnly);
    tmptiedosto.open(QFile::ReadWrite|QFile::Append);
    QTextStream instriimi(&tiedosto);
    QTextStream outstriimi(&tmptiedosto);
    instriimi.setCodec("ISO 8859-1");
    outstriimi.setCodec("ISO 8859-1");

    QString obsLine;
    while( instriimi.atEnd() == false )
    {
        obsLine = instriimi.readLine();
        if( obsLine.section( '#', 0, 0 ).toLongLong() == id )
        {
            outstriimi << data;
        }
        else
        {
            outstriimi << obsLine;
        }
        outstriimi << "\n";
    }
    tiedosto.close();
    tmptiedosto.close();
    if( QFile::exists( dataFileDir() + "lokkitesti.backup" ) == true )
    {
        QFile old( dataFileDir() + "lokkitesti.backup" );
        old.remove();
    }
    tiedosto.rename( dataFileDir() + "lokkitesti.backup");
    tmptiedosto.rename( dataFileDir() + "lokkitesti.txt");
    tiedosto.remove();
}

QString ModelDataWriter::loadObservation( qlonglong id )
{
    qDebug() << "LUETAAN" << id;
    if( QFile::exists( dataFileDir() + "lokkitesti.txt" ) == false )
    {
        return QString();
    }
    QFile tiedosto( dataFileDir() + "lokkitesti.txt" );
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    QString obsLine;
    while( striimi.atEnd() == false )
    {
        obsLine = striimi.readLine();
        if( obsLine.section( '#', 0, 0 ).toLongLong() == id )
        {
            break;
        }
    }
    tiedosto.close();
    if( obsLine.section( '#', 0, 0 ) == "Id" || obsLine.section( '#', 0, 0 ).isEmpty() == true )
    {
        return QString();
    }
    return obsLine;
}

void ModelDataWriter::deleteObservation(qlonglong id)
{
    qDebug() << "DELETE ID" << id;
    QFile tiedosto( dataFileDir() + "lokkitesti.txt" );
    QFile tmptiedosto( dataFileDir() + "lokkitesti.tmp" );

    tiedosto.open(QFile::ReadOnly);
    tmptiedosto.open(QFile::ReadWrite|QFile::Append);
    QTextStream instriimi(&tiedosto);
    QTextStream outstriimi(&tmptiedosto);
    instriimi.setCodec("ISO 8859-1");
    outstriimi.setCodec("ISO 8859-1");

    QString obsLine;
    while( instriimi.atEnd() == false )
    {
        obsLine = instriimi.readLine();
        if( obsLine.section( '#', 0, 0 ).toLongLong() != id )
        {
            outstriimi << obsLine;
            outstriimi << "\n";
        }
    }
    tiedosto.close();
    tmptiedosto.close();
    if( QFile::exists( dataFileDir() + "lokkitesti.backup" ) == true )
    {
        QFile old( dataFileDir() + "lokkitesti.backup" );
        old.remove();
    }
    tiedosto.rename( dataFileDir() + "lokkitesti.backup");
    tmptiedosto.rename( dataFileDir() + "lokkitesti.txt");
    tiedosto.remove();
}


void ModelDataWriter::writePersonData(PersonModel *model)
{
    QFile tiedosto( dataFileDir() + "lokkitestiperson.txt");
    tiedosto.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    int rows = model->rowCount();
    qDebug() << "person rows" << rows;
    for( int i = 0; i < rows; i++ )
    {
        qDebug() << "writing row" << i;
        QString line;
        line.append( model->data( model->index( i ), PersonModel::FirstNameRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), PersonModel::SurNameRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), PersonModel::RegisteredRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), PersonModel::DefaultRole ).toString());
        line.append( ";\n" );
        qDebug() << "line" << line;
        striimi << line;
    }
    striimi.flush();
    tiedosto.flush();
    tiedosto.close();
}

void ModelDataWriter::writeLocationData(LocationModel *model)
{
    QFile tiedosto( dataFileDir() + "lokkitestilocation.txt");
    tiedosto.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    int rows = model->rowCount();
    for( int i = 0; i < rows; i++ )
    {
        QString line;
        line.append( model->data( model->index( i ), LocationModel::TownRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), LocationModel::PlaceRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), LocationModel::WgsCoordinateRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), LocationModel::YkjCoordinateRole ).toString());
        line.append( ";\n" );
        striimi << line;
    }
    tiedosto.close();
}

void ModelDataWriter::writeBirdData(BirdModel *model)
{
    QFile tiedosto( dataFileDir() + "lokkitestibirds.txt");
    tiedosto.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    int rows = model->rowCount();
    striimi << QString("\n");
    for( int i = 0; i < rows; i++ )
    {
        QString line;
        line.append( model->data( model->index( i ), BirdModel::IdRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), BirdModel::FinGroupRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), BirdModel::SweGroupRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), BirdModel::LatinGroupRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), BirdModel::FinNameRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), BirdModel::SweNameRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), BirdModel::AbbrevRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), BirdModel::LatinNameRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), BirdModel::CategoryRole ).toString());
        line.append( ";\n" );
        striimi << line;
    }
    tiedosto.close();
}

void ModelDataWriter::exportHistory( bool onlyNew )
{
    QDateTime date;
    date = QDateTime::currentDateTime();
    QString fileName( "lokkiexport-" );
    fileName.append(date.toString("yyyyMMdd-hhmmss"));
    fileName.append(".csv");
    qDebug() << "void ModelDataWriter::exportHistory( bool onlyNew )" << onlyNew;
    QFile tiedosto( dataFileDir() + "lokkitesti.txt" );
    QFile newtiedosto( dataFileDir() + "lokkitesti.tmp.txt" );
    QFile tmptiedosto( exportDir() + fileName );

    tiedosto.open(QFile::ReadOnly);
    newtiedosto.open(QFile::ReadWrite|QFile::Truncate);
    tmptiedosto.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream instriimi(&tiedosto);
    QTextStream outstriimi(&tmptiedosto);
    QTextStream outstriimi2(&newtiedosto);
    instriimi.setCodec("ISO 8859-1");
    outstriimi.setCodec("ISO 8859-1");
    outstriimi2.setCodec("ISO 8859-1");

    QString obsLine;
    while( instriimi.atEnd() == false )
    {
        obsLine = instriimi.readLine();
        if( onlyNew == false || obsLine.section( '#', 24, 24 ) != QString("true") )
        {
            outstriimi << obsLine;
            outstriimi << "\n";
        }
        if( obsLine.length() > 20 )
        {
            qDebug() << "rivi ennen export settia" << obsLine;
            QString newLine;
            newLine = obsLine;
            int count = obsLine.count("#");
            int exportPos = 0;
            if( count == 25 )
            {
                qDebug() << "COUNT 25";
                exportPos = obsLine.lastIndexOf("#", obsLine.length() - 3 );
            }
            else if( count == 24 )
            {
                qDebug() << "COUNT 24";
                if( obsLine.endsWith( "#" ) == false )
                {
                    qDebug() << "COUNT 24 1";
                    exportPos = obsLine.lastIndexOf("#", obsLine.length() - 3 );
                }
                else
                {
                    qDebug() << "COUNT 24 2";
                    exportPos = obsLine.length();
                }
            }
            else if( count == 23 )
            {
                qDebug() << "COUNT 23";
                exportPos = obsLine.length();
            }
            QString start = newLine.mid( 0, exportPos + 1 );
            if( start.endsWith( "#" ) == false )
            {
                start.append("#");
            }
            start.append( "true#\n");
            qDebug() << "uus rivi export setin jalkeen" << start;
            outstriimi2 << start;
        }
    }
    tiedosto.close();
    tmptiedosto.close();
    newtiedosto.close();

    tiedosto.rename( dataFileDir() + "lokkitesti.backup");
    newtiedosto.rename( dataFileDir() + "lokkitesti.txt");
}

qlonglong ModelDataWriter::getNewId()
{
    qDebug() << "LUETAAN IDT";
    if( QFile::exists( dataFileDir() + "lokkitesti.txt" ) == false )
    {
        return 1;
    }
    QFile tiedosto( dataFileDir() + "lokkitesti.txt" );
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    QString obsLine;
    qlonglong maxId = 0;
    while( striimi.atEnd() == false )
    {
        obsLine = striimi.readLine();
        if( obsLine.section( '#', 0, 0 ).toLongLong() > maxId )
        {
            maxId = obsLine.section( '#', 0, 0 ).toLongLong();
        }
    }
    maxId++;
    tiedosto.close();
    return maxId;
}

QString ModelDataWriter::dataFileDir()
{
    QString appPath;
#ifdef Q_OS_SYMBIAN
    appPath = QCoreApplication::applicationDirPath();
#elif defined HARMATTAN
    appPath = QString( "/home/user/MyDocs/.lokki/" );
#else
    appPath = QString( "C:/" );

#endif
    return appPath;
}

QString ModelDataWriter::exportDir()
{
    QString appPath;
#ifdef Q_OS_SYMBIAN
    appPath = QString( "C:/data/" );
#elif defined HARMATTAN
    appPath = QString( "/home/user/MyDocs/" );
#else
    appPath = QString( "C:/" );

#endif
    return appPath;
}

void ModelDataWriter::removeCustomSpecies()
{
    QFile tiedosto( dataFileDir() + "lokkitestibirds.txt");
    tiedosto.remove();
}

void ModelDataWriter::removeCustomLocations()
{
    QFile tiedosto( dataFileDir() + "lokkitestilocation.txt");
    tiedosto.remove();
}

void ModelDataWriter::removeCustomObservers()
{
    QFile tiedosto( dataFileDir() + "lokkitestiperson.txt");
    tiedosto.remove();
}


