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
        qDebug() << "ON JO TALLENNETTU";
        return;
    }
    qDebug() << "NYT PITAIS TALLENTAA" << data;
    bool headerExists = false;
    if( QFile::exists( "/Users/Tero/lokkitesti.txt" ) == true )
    {
        headerExists = true;
    }
    QFile tiedosto( "/Users/Tero/lokkitesti.txt");
    tiedosto.open(QFile::ReadWrite|QFile::Append);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    if( headerExists == false )
    {
        QString header = QString::fromUtf8("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Paikka#Lisätietoja#Atlaskoodi#Havainnoijat#Havainnoijat2#Salattu#Rivejä#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Sää\n");
        striimi << header;
    }
//    striimi << QString( "1#" );
    striimi << data;
    striimi << "\n";
    tiedosto.close();
}

QString ModelDataWriter::loadObservation( qlonglong id )
{
    qDebug() << "LUETAAN" << id;
    if( QFile::exists( "/Users/Tero/lokkitesti.txt" ) == false )
    {
        return QString();
    }
    QFile tiedosto( "/Users/Tero/lokkitesti.txt");
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
