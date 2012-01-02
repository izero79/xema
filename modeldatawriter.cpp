#include <QFile>
#include <QTextStream>
#include <QDebug>


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
    if( QFile::exists( filePath + "lokkitesti.txt" ) == false )
    {
        headerExists = false;
    }
    QFile tiedosto( filePath + "lokkitesti.txt" );
    tiedosto.open(QFile::ReadWrite|QFile::Append);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    if( headerExists == false )
    {
        QString header = QString::fromUtf8("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Paikka#Lisätietoja#Atlaskoodi#Havainnoijat#Havainnoijat2#Salattu#Rivejä#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Sää\n");
        striimi << header;
    }
    striimi << newData;
    striimi << "\n";
    tiedosto.close();
}

void ModelDataWriter::replaceObservation( qlonglong id, const QString &data )
{
    qDebug() << "KORVATAAN, ID ON" << id;
/*
    bool headerExists = false;
    if( QFile::exists( filePath + "lokkitesti.txt" ) == true )
    {
        headerExists = true;
    }*/
    QFile tiedosto( filePath + "lokkitesti.txt" );
    QFile tmptiedosto( filePath + "lokkitesti.tmp" );

    tiedosto.open(QFile::ReadOnly);
    tmptiedosto.open(QFile::ReadWrite|QFile::Append);
    QTextStream instriimi(&tiedosto);
    QTextStream outstriimi(&tmptiedosto);
    instriimi.setCodec("ISO 8859-1");
    outstriimi.setCodec("ISO 8859-1");
/*
    if( headerExists == false )
    {
        QString header = QString::fromUtf8("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Paikka#Lisätietoja#Atlaskoodi#Havainnoijat#Havainnoijat2#Salattu#Rivejä#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Sää\n");
        outstriimi << header;
    }*/
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
    if( QFile::exists( filePath + "lokkitesti.backup" ) == true )
    {
        QFile old( filePath + "lokkitesti.backup" );
        old.remove();
    }
    tiedosto.rename( filePath + "lokkitesti.backup");
    tmptiedosto.rename( filePath + "lokkitesti.txt");
    tiedosto.remove();
}

QString ModelDataWriter::loadObservation( qlonglong id )
{
    qDebug() << "LUETAAN" << id;
    if( QFile::exists( filePath + "lokkitesti.txt" ) == false )
    {
        return QString();
    }
    QFile tiedosto( filePath + "lokkitesti.txt" );
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

void ModelDataWriter::writePersonData(PersonModel *model)
{
    QFile tiedosto( filePath + "lokkitestiperson.txt");
    tiedosto.open(QFile::ReadWrite);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    for( int i = 0; i < model->rowCount(); i++ )
    {
        QString line;
        line.append( model->data( model->index( i ), PersonModel::FirstNameRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), PersonModel::SurNameRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), PersonModel::RegisteredRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), PersonModel::DefaultRole ).toString());
        line.append( ";\n" );
        striimi << line;
    }
    tiedosto.close();
}

void ModelDataWriter::writeLocationData(LocationModel *model)
{
    QFile tiedosto( filePath + "lokkitestilocation.txt");
    tiedosto.open(QFile::ReadWrite);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    for( int i = 0; i < model->rowCount(); i++ )
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
    QFile tiedosto( filePath + "lokkitestibirds.txt");
    tiedosto.open(QFile::ReadWrite);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    for( int i = 0; i < model->rowCount(); i++ )
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
        line.append( model->data( model->index( i ), BirdModel::LatinNameRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), BirdModel::AbbrevRole ).toString());
        line.append( "\n" );
        line.append( model->data( model->index( i ), BirdModel::CategoryRole ).toString());
        line.append( "\n" );
        striimi << line;
    }
    tiedosto.close();
}

qlonglong ModelDataWriter::getNewId()
{
    qDebug() << "LUETAAN IDT";
    if( QFile::exists( filePath + "lokkitesti.txt" ) == false )
    {
        return 1;
    }
    QFile tiedosto( filePath + "lokkitesti.txt" );
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

