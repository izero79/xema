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
        QString header = QString::fromUtf8("Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Paikka#Lisätietoja#Atlaskoodi#Havainnoijat#Havainnoijat2#Salattu#Rivejä#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Sää\n");
        striimi << header;
    }
    striimi << data;
    striimi << "\n";
    tiedosto.close();
}
