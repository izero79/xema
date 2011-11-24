#include <QFile>
#include <QTextStream>
#include <QDebug>


#include "modeldatawriter.h"

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
#ifdef Q_OS_SYMBIAN
    if( QFile::exists( "c:/data/lokkitesti.txt" ) == false )
#else
    if( QFile::exists( "/Users/Tero/lokkitesti.txt" ) == false )
#endif
    {
        headerExists = false;
    }
#ifdef Q_OS_SYMBIAN
    QFile tiedosto( "c:/data/lokkitesti.txt");
#else
    QFile tiedosto( "/Users/Tero/lokkitesti.txt");
#endif
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

    bool headerExists = false;
#ifdef Q_OS_SYMBIAN
    if( QFile::exists( "c:/data/lokkitesti.txt" ) == false )
#else
    if( QFile::exists( "/Users/Tero/lokkitesti.txt" ) == false )
#endif
    {
        headerExists = true;
    }
#ifdef Q_OS_SYMBIAN
    QFile tiedosto( "c:/data/lokkitesti.txt");
    QFile tmptiedosto( "c:/data/lokkitesti.tmp");
#else
    QFile tiedosto( "/Users/Tero/lokkitesti.txt");
    QFile tmptiedosto( "/Users/Tero/lokkitesti.tmp");
#endif
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
#ifdef Q_OS_SYMBIAN
    if( QFile::exists( "c:/data/lokkitesti.backup" ) == true )
    {
        QFile old( "c:/data/lokkitesti.backup" );
        old.remove();
    }
    tiedosto.rename( "c:/data/lokkitesti.backup");
    tmptiedosto.rename( "c:/data/lokkitesti.txt");
#else
    if( QFile::exists( "/Users/Tero/lokkitesti.backup" ) == true )
    {
        QFile old( "/Users/Tero/lokkitesti.backup" );
        old.remove();
    }
    tiedosto.rename( "/Users/Tero/lokkitesti.backup");
    tmptiedosto.rename( "/Users/Tero/lokkitesti.txt");
#endif
    tiedosto.remove();
}

QString ModelDataWriter::loadObservation( qlonglong id )
{
    qDebug() << "LUETAAN" << id;
#ifdef Q_OS_SYMBIAN
    if( QFile::exists( "c:/data/lokkitesti.txt" ) == false )
#else
    if( QFile::exists( "/Users/Tero/lokkitesti.txt" ) == false )
#endif
    {
        return QString();
    }
#ifdef Q_OS_SYMBIAN
    QFile tiedosto( "c:/data/lokkitesti.txt");
#else
    QFile tiedosto( "/Users/Tero/lokkitesti.txt");
#endif
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

qlonglong ModelDataWriter::getNewId()
{
    qDebug() << "LUETAAN IDT";
#ifdef Q_OS_SYMBIAN
    if( QFile::exists( "c:/data/lokkitesti.txt" ) == false )
#else
    if( QFile::exists( "/Users/Tero/lokkitesti.txt" ) == false )
#endif
    {
        return 1;
    }
#ifdef Q_OS_SYMBIAN
    QFile tiedosto( "c:/data/lokkitesti.txt");
#else
    QFile tiedosto( "/Users/Tero/lokkitesti.txt");
#endif
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
