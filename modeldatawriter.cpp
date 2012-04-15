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
#include "xemaconstants.h"
#include "coordinateconverter.h"

ModelDataWriter::ModelDataWriter(QObject *parent) :
    QObject(parent)
{
    mCoordinates = new CoordinateConverter(this);
}

void ModelDataWriter::writeNewObservation( const QString &data, const LocationModel &locations, const PersonModel &persons )
{
    if( data.section( '#', 0, 0 ) != "0" )
    {
//        qDebug() << "ON JO TALLENNETTU, KORVATAAN";
        replaceObservation( data.section( '#', 0, 0 ).toLongLong(), data );
        return;
    }
    qlonglong newId = getNewId();
//    qDebug() << "UUSI ID ON" << newId;

//    qDebug() << "ALUNP PITAIS TALLENTAA" << data;
    QString newData = data;
    int pos = newData.indexOf( "#" );
    newData.remove( 0, pos );
    QString newIdNum;
    newIdNum.setNum( newId );
    newData.prepend( newIdNum );
//    newData = formatToTiira(newData, locations, persons);
//    qDebug() << "NYT PITAIS TALLENTAA" << newData;
    bool headerExists = true;
    if( QFile::exists( dataFileDir() + "xemadata.txt" ) == false )
    {
        headerExists = false;
    }
    QFile tiedosto( dataFileDir() + "xemadata.txt" );
    tiedosto.open(QFile::ReadWrite|QFile::Append);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    if( headerExists == false )
    {
//        QString header = QString::fromUtf8("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Kunta#Paikka#X-koord#Y-koord#Tarkkuus#X-koord-linnun#Y-koord-linnun#Tarkkuus_linnun#Paikannettu#Lisätietoja#Atlaskoodi#Tallentaja#Tallennusaika#Havainnoijat#Salattu#Koontihavainto#Kuuluu havaintoon#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Epäsuora havainto#Sää#Exported#\n");
        QString header = QString::fromUtf8("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Paikka#Lisätietoja#Atlaskoodi#Havainnoijat#Havainnoijat2#Salattu#Rivejä#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Sää#Exported#\n");
        striimi << header;
    }
    striimi << newData;
    striimi << "\n";
    tiedosto.close();
}

void ModelDataWriter::replaceObservation( qlonglong id, const QString &data )
{
//    qDebug() << "KORVATAAN, ID ON" << id;
    QFile tiedosto( dataFileDir() + "xemadata.txt" );
    QFile tmptiedosto( dataFileDir() + "xemadata.tmp" );

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
    if( QFile::exists( dataFileDir() + "xemadata.backup" ) == true )
    {
        QFile old( dataFileDir() + "xemadata.backup" );
        old.remove();
    }
    tiedosto.rename( dataFileDir() + "xemadata.backup");
    tmptiedosto.rename( dataFileDir() + "xemadata.txt");
    tiedosto.remove();
}

QString ModelDataWriter::loadObservation( qlonglong id )
{
//    qDebug() << "LUETAAN" << id;
    if( QFile::exists( dataFileDir() + "xemadata.txt" ) == false )
    {
        return QString();
    }
    QFile tiedosto( dataFileDir() + "xemadata.txt" );
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
//    qDebug() << "DELETE ID" << id;
    QFile tiedosto( dataFileDir() + "xemadata.txt" );
    QFile tmptiedosto( dataFileDir() + "xemadata.tmp" );

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
    if( QFile::exists( dataFileDir() + "xemadata.backup" ) == true )
    {
        QFile old( dataFileDir() + "xemadata.backup" );
        old.remove();
    }
    tiedosto.rename( dataFileDir() + "xemadata.backup");
    tmptiedosto.rename( dataFileDir() + "xemadata.txt");
    tiedosto.remove();
}


void ModelDataWriter::writePersonData(PersonModel *model)
{
    QFile tiedosto( dataFileDir() + "xemapersondata.txt");
    tiedosto.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    int rows = model->rowCount();
//    qDebug() << "person rows" << rows;
    for( int i = 0; i < rows; i++ )
    {
//        qDebug() << "writing row" << i;
        QString line;
        line.append( model->data( model->index( i ), PersonModel::FirstNameRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), PersonModel::SurNameRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), PersonModel::RegisteredRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), PersonModel::DefaultRole ).toString());
        line.append( ";\n" );
//        qDebug() << "line" << line;
        striimi << line;
    }
    striimi.flush();
    tiedosto.flush();
    tiedosto.close();
}

void ModelDataWriter::writeLocationData(LocationModel *model)
{
    QFile tiedosto( dataFileDir() + "xemalocationdata.txt");
    tiedosto.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    int rows = model->rowCount();
    for( int i = 0; i < rows; i++ )
    {
        QString wgs = model->data( model->index( i ), LocationModel::WgsCoordinateRole ).toString();
        QString ykj = model->data( model->index( i ), LocationModel::YkjCoordinateRole ).toString();
        QString line;
        line.append( model->data( model->index( i ), LocationModel::TownRole ).toString());
        line.append( ";" );
        line.append( model->data( model->index( i ), LocationModel::PlaceRole ).toString());
        line.append( ";" );
        if( ykj.isEmpty() == true && wgs.isEmpty() == false )
        {
            QString x = wgs.section( ":", 0, 0);
            QString y = wgs.section( ":", 1, 1 );
            double dx = x.toDouble();
            double dy = y.toDouble();
            double ykjx = 0;
            double ykjy = 0;
            QPair<double,double> newCoord;
            newCoord = mCoordinates->wgsToykj( dx, dy );
            ykjx = newCoord.first;
            ykjy = newCoord.second;
            long newX = ykjx;
            long newY = ykjy;
            QString ykjX;
            ykjX.setNum(newX);
            QString ykjY;
            ykjY.setNum(newY);
            ykj = ykjX;
            ykj.append(":");
            ykj.append( ykjY );
        }

        if( ykj.isEmpty() == false && wgs.isEmpty() == true )
        {
            QString x = ykj.section( ":", 0, 0);
            QString y = ykj.section( ":", 1, 1 );
            double dx = x.toDouble();
            double dy = y.toDouble();
            double wgsx = 0;
            double wgsy = 0;
            QPair<double,double> newCoord;
            newCoord = mCoordinates->ykjTowgs( dx, dy );
            wgsx = newCoord.first;
            wgsy = newCoord.second;
            QString wgsX;
            wgsX.setNum(wgsx,'g',6);
            QString wgsY;
            wgsY.setNum(wgsy,'g',6);
            wgs = wgsX;
            wgs.append(":");
            wgs.append( wgsY );
        }

        line.append( wgs );
        line.append( ";" );
        line.append( ykj );
        line.append( ";\n" );
        striimi << line;
    }
    tiedosto.close();
}

void ModelDataWriter::writeBirdData(BirdModel *model)
{
    QFile tiedosto( dataFileDir() + "xemabirddata.txt");
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

void ModelDataWriter::exportHistory( bool onlyNew, const LocationModel &locations, const PersonModel &persons, const BirdModel &birds )
{
    QDateTime date;
    date = QDateTime::currentDateTime();
    QString fileName( "xemaexport-" );
    fileName.append(date.toString("yyyyMMdd-hhmmss"));
    fileName.append(".csv");
//    qDebug() << "void ModelDataWriter::exportHistory( bool onlyNew )" << onlyNew;
    QFile tiedosto( dataFileDir() + "xemadata.txt" );
    QFile newtiedosto( dataFileDir() + "xemadata.tmp.txt" );
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
    QString header = QString::fromUtf8("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Kunta#Paikka#X-koord#Y-koord#Tarkkuus#X-koord-linnun#Y-koord-linnun#Tarkkuus_linnun#Paikannettu#Lisätietoja#Atlaskoodi#Tallentaja#Tallennusaika#Havainnoijat#Salattu#Koontihavainto#Kuuluu havaintoon#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Epäsuora havainto#Sää#\n");
    outstriimi << header;
    while( instriimi.atEnd() == false )
    {
        obsLine = instriimi.readLine();
        if( obsLine.section("#",0,0) == "Id" )
        {
            outstriimi2 << obsLine;
            outstriimi2 << "\n";
            continue;
        }
        int xemaRows = obsLine.section("#",12,12).toInt();
//        qDebug() << Q_FUNC_INFO << "XEMAROWS" << xemaRows;
        int exportPos = 25 + ( (xemaRows-1) * 11 );
        QString exported = obsLine.section( '#', exportPos, exportPos );

//        qDebug() << Q_FUNC_INFO << "EXPORTED" << exported;
        int pos = -1;
        int i = 0;
        do
        {
            pos = obsLine.indexOf("#",pos+1);
//            qDebug() << "pos" << pos << i << exportPos;
            if( i == exportPos - 1 )
            {
//                qDebug() << "pos" << pos << "paikka" << i;

                break;
            }
            i++;
        }
        while(pos>0);

        if( onlyNew == false || exported == "false" )
        {
            outstriimi << formatToTiira( obsLine,locations,persons,birds );
            outstriimi << "\n";
        }
        if( obsLine.length() > 20 )
        {
//            qDebug() << "rivi ennen export settia" << obsLine;
            QString newLine;
            newLine = obsLine;

            QString start = newLine.mid( 0, pos );
            if( start.endsWith( "#" ) == false )
            {
                start.append("#");
            }
            start.append( "true#\n");
//            qDebug() << "uus rivi export setin jalkeen" << start;
            outstriimi2 << start;
        }
    }
    tiedosto.close();
    tmptiedosto.close();
    newtiedosto.close();

    QFile::remove( dataFileDir() + "xemadata.backup" );
    tiedosto.rename( dataFileDir() + "xemadata.backup");
    newtiedosto.rename( dataFileDir() + "xemadata.txt");
}

qlonglong ModelDataWriter::getNewId()
{
//    qDebug() << "LUETAAN IDT";
    if( QFile::exists( dataFileDir() + "xemadata.txt" ) == false )
    {
        return 1;
    }
    QFile tiedosto( dataFileDir() + "xemadata.txt" );
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
    appPath = QString( "/home/user/MyDocs/.xema/" );
#elif defined MAC_OS_X_VERSION_10_6
    appPath = QString( "/Users/Tero/" );
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
#elif defined MAC_OS_X_VERSION_10_6
    appPath = QString( "/Users/Tero/" );
#else
    appPath = QString( "C:/" );

#endif
    return appPath;
}

void ModelDataWriter::removeCustomSpecies()
{
    QFile tiedosto( dataFileDir() + "xemabirddata.txt");
    tiedosto.remove();
}

void ModelDataWriter::removeCustomLocations()
{
    QFile tiedosto( dataFileDir() + "xemalocationdata.txt");
    tiedosto.remove();
}

void ModelDataWriter::removeCustomObservers()
{
    QFile tiedosto( dataFileDir() + "xemapersondata.txt");
    tiedosto.remove();
}

QString ModelDataWriter::formatToTiira(const QString &data, const LocationModel &locations, const PersonModel &persons, const BirdModel &birds)
{
    for( int i = 0; i < data.count("#"); i++)
    {
        qDebug() << "kentta" << i << "arvo" << data.section("#",i,i);
    }
    QString id = data.section("#",0, 0);
    QString species = data.section("#",1, 1);
    QString datetime = data.section("#",2,5);
    QString location = data.section( "#", 6, 6 );
    QString vali = data.section("#",7,8);
    QString nimet = data.section("#",9,10);
    QString hidden = data.section("#",11,11);
    int xemaRows = data.section("#",12,12).toInt();
    QString town = location.section( ",", 0, 0 );
    QString place = location.section( ", ", 1 );
    QString toka = "#" + town + "#" + place;
    QString eka = id;

    int rowCount = birds.rowCount();
    for( int i = 0; i < rowCount; i++ )
    {
        if(birds.getItem(i).finName() == species)
        {
            species = birds.getItem(i).abbreviation();
            break;
        }
    }

    eka += "#";
    eka += species;
    eka += "#";
    if( datetime.section("#",3,3).isEmpty() == true )
    {
        eka += "#";
        eka += datetime.section("#",0,2);
        eka += "#";
        eka += datetime.section("#",2,2);
        eka += "#";
    }
    else
    {
        eka += datetime;
    }
    rowCount = locations.rowCount();

    // add location coordinates if found
    bool locationAdded = false;
    for( int i = 0; i < rowCount; i++ )
    {
        if( locations.getItem( i ).town() == town && locations.getItem( i ).place() == place )
        {
            QString coordinate = locations.getItem(i).wgsCoordinate();
            QString x = coordinate.section( ":", 0, 0);
            QString y = coordinate.section( ":", 1, 1 );
            double dx = x.toDouble();
            double dy = y.toDouble();
            double ykjx = 0;
            double ykjy = 0;
            QPair<double,double> newCoord;
            newCoord = mCoordinates->wgsToykj( dx, dy );
            ykjx = newCoord.first;
            ykjy = newCoord.second;
            long newX = ykjx;
            long newY = ykjy;
            QString ykjX;
            ykjX.setNum(newX);
            QString ykjY;
            ykjY.setNum(newY);

            // tiiraan koordinaatit toisinpäin
            toka.append( "#" + ykjY + "#" + ykjX );
            locationAdded = true;
            break;
        }
    }
    if( locationAdded == false )
    {
        toka.append("##");
    }

    eka += toka;
    eka += QString("######");
    eka += vali;
    eka += "#";

    // add data saver (default name if found)
    rowCount = persons.rowCount();
    QString defaultName;
    for( int i = 0; i < rowCount; i++ )
    {
        if( persons.getItem( i ).defaultName() == true )
        {
            defaultName = persons.getItem( i ).firstName() + " " + persons.getItem( i ).surName();
            break;
        }
    }

    if( nimet.contains(defaultName) == true )
    {
        eka += defaultName;
    }
    eka += "#";
    // tiira saving time
    eka += "#";

//    qDebug() << "NIMET ennen muutosta" << nimet;
    if( nimet.length() > 1 && nimet.endsWith('#') == false) {
        nimet.replace( "#", ", " );
    }
    else {
        nimet.replace( "#", "" );
    }
    eka += nimet;
    eka += "#";
    eka += hidden;
    eka += "#";

    // koontihavainto ja kuuluu havaintoon
    eka += "#";
    eka += "#";

    // include exported or not (24 vs 25)
    QString loppu = data.section( "#", 24+((xemaRows-1)*11),24+((xemaRows-1)*11));
    loppu.prepend("#");
    loppu.append("#");

    QString firstRow = data.section( "#", 13, 23);
    firstRow.replace("#koiras#", "#k#");
    firstRow.replace("#naaras#", "#n#");
    firstRow.replace("#pariutuneet#", "#p#");
    eka += firstRow;
    eka += loppu;

    if( xemaRows > 1 ) {
        for( int i = 1; i < xemaRows; i++ )
        {
            eka += "\n";
            eka += data.section("#", 0, 0);
            eka += "#######################";
            QString rivi = data.section( "#", 13+(i*11), 23+(i*11));
            rivi.replace("#koiras#", "#k#");
            rivi.replace("#naaras#", "#n#");
            eka += rivi;
            eka += loppu;
        }
    }

//    qDebug() << "EKA ENNEN MUUTOSTA" << eka;
    eka.replace("#true","#X", Qt::CaseSensitive);
    eka.replace("#false","#", Qt::CaseSensitive);
//    qDebug() << "EKA MUUTOKSEN JALKEEN" << eka;
    return eka;
}

void ModelDataWriter::importHistory()
{
    qDebug() << "IMPORT";
    QFile importfile( dataFileDir() + "xemadata_import.txt" );
    importfile.open(QFile::ReadOnly);
    QTextStream importstream(&importfile);
//    ("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Paikka#Lisätietoja#Atlaskoodi#Havainnoijat#Havainnoijat2#Salattu#Rivejä#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Sää#Exported#\n");

    if( importstream.atEnd() == false )
    {
        if( importstream.readLine().section("#",0,0) != "Id" )
        {
            importstream.reset();
        }
    }
    while( importstream.atEnd() == false )
    {
        QString line;
        line = importstream.readLine();
        qDebug() << "IMPORT line" << line;
/*
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
        // TODO other combinations*/
    }
}
