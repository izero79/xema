#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>
#include <QDateTime>
#include <QStringList>

#include "modeldatawriter.h"
#include "birdmodel.h"
#include "bird.h"
#include "locationmodel.h"
#include "location.h"
#include "personmodel.h"
#include "person.h"
#include "xemaenums.h"
#include "xemaconstants.h"
#include "coordinateconverter.h"
#include "statusmodel.h"
#include "status.h"

ModelDataWriter* ModelDataWriter::mDataWriter = 0;

ModelDataWriter* ModelDataWriter::instance() {
    if (!mDataWriter) {
        mDataWriter = new ModelDataWriter();
    }
    return mDataWriter;
}

ModelDataWriter::ModelDataWriter(QObject *parent) :
    QObject(parent)
{
    mCoordinates = new CoordinateConverter(this);
    checkAndCreateDirs();
}

ModelDataWriter::~ModelDataWriter() {
    delete mDataWriter;
}


void ModelDataWriter::writeNewObservation(const QString &data)
{
    if (data.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID) != "0")
    {
//        qDebug() << "ON JO TALLENNETTU, KORVATAAN";
        replaceObservation(data.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong(), data);
        return;
    }
    qlonglong newId = getNewId();

    QString newData = data;
    int pos = newData.indexOf("#");
    newData.remove(0, pos);
    QString newIdNum;
    newIdNum.setNum(newId);
    newData.prepend(newIdNum);

    bool headerExists = true;
    if (QFile::exists(dataFileDir() + "xemadata.txt") == false)
    {
        headerExists = false;
    }
    QFile tiedosto(dataFileDir() + "xemadata.txt");
    tiedosto.open(QFile::ReadWrite|QFile::Append);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    if (headerExists == false)
    {
        QString header = QString::fromUtf8("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Kunta#Paikka#X-koord#Y-koord#Tarkkuus#X-koord-linnun#Y-koord-linnun#Tarkkuus_linnun#Paikannettu#Lisätietoja#Atlaskoodi#Tallentaja#Tallennusaika#Havainnoijat#Havainnoijat2#Salattu#Koontihavainto#Kuuluu havaintoon#Rivejä#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Epäsuora havainto#Sää#Exported#\n");
//        QString header = QString::fromUtf8("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Paikka#Lisätietoja#Atlaskoodi#Havainnoijat#Havainnoijat2#Salattu#Rivejä#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Sää#Exported#\n");
        striimi << header;
    }
    striimi << newData;
    striimi << "\n";
    tiedosto.close();
}

void ModelDataWriter::replaceObservation(qlonglong id, const QString &data)
{
//    qDebug() << "KORVATAAN, ID ON" << id;
    QFile tiedosto(dataFileDir() + "xemadata.txt");
    QFile tmptiedosto(dataFileDir() + "xemadata.tmp");

    tiedosto.open(QFile::ReadOnly);
    tmptiedosto.open(QFile::ReadWrite|QFile::Append);
    QTextStream instriimi(&tiedosto);
    QTextStream outstriimi(&tmptiedosto);
    instriimi.setCodec("ISO 8859-1");
    outstriimi.setCodec("ISO 8859-1");

    QString obsLine;
    while (instriimi.atEnd() == false)
    {
        obsLine = instriimi.readLine();
        if (obsLine.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong() == id)
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
    if (QFile::exists(dataFileDir() + "xemadata.backup") == true)
    {
        QFile old(dataFileDir() + "xemadata.backup");
        old.remove();
    }
    tiedosto.rename(dataFileDir() + "xemadata.backup");
    tmptiedosto.rename(dataFileDir() + "xemadata.txt");
    tiedosto.remove();
}

void ModelDataWriter::deleteObservation(qlonglong id)
{
//    qDebug() << "DELETE ID" << id;
    QFile tiedosto(dataFileDir() + "xemadata.txt");
    QFile tmptiedosto(dataFileDir() + "xemadata.tmp");

    tiedosto.open(QFile::ReadOnly);
    tmptiedosto.open(QFile::ReadWrite|QFile::Append);
    QTextStream instriimi(&tiedosto);
    QTextStream outstriimi(&tmptiedosto);
    instriimi.setCodec("ISO 8859-1");
    outstriimi.setCodec("ISO 8859-1");

    QString obsLine;
    while (instriimi.atEnd() == false)
    {
        obsLine = instriimi.readLine();
        if (obsLine.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong() != id)
        {
            outstriimi << obsLine;
            outstriimi << "\n";
        }
    }
    tiedosto.close();
    tmptiedosto.close();
    if (QFile::exists(dataFileDir() + "xemadata.backup") == true)
    {
        QFile old(dataFileDir() + "xemadata.backup");
        old.remove();
    }
    tiedosto.rename(dataFileDir() + "xemadata.backup");
    tmptiedosto.rename(dataFileDir() + "xemadata.txt");
    tiedosto.remove();
}


void ModelDataWriter::writePersonData(PersonModel *model)
{
    QFile tiedosto(dataFileDir() + "xemapersondata.txt");
    tiedosto.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    int rows = model->rowCount();
//    qDebug() << "person rows" << rows;
    for(int i = 0; i < rows; i++)
    {
//        qDebug() << "writing row" << i;
        QString line;
        line.append(model->data(model->index(i), PersonModel::FirstNameRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), PersonModel::SurNameRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), PersonModel::RegisteredRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), PersonModel::DefaultRole).toString());
        line.append(";\n");
//        qDebug() << "line" << line;
        striimi << line;
    }
    striimi.flush();
    tiedosto.flush();
    tiedosto.close();
}

void ModelDataWriter::writeLocationData(LocationModel *model)
{
    QFile tiedosto(dataFileDir() + "xemalocationdata.txt");
    tiedosto.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    int rows = model->rowCount();
    striimi << QString("Kunta;Paikka;wgs;ykj;kunta_swe;paikka_swe;kunta_eng;paikka_eng;maa;maa_swe;maa_eng;muokattu\n");
    for(int i = 0; i < rows; i++)
    {
        QString wgs = model->data(model->index(i), LocationModel::WgsCoordinateRole).toString();
        QString ykj = model->data(model->index(i), LocationModel::YkjCoordinateRole).toString();
        QString line;
        line.append(model->data(model->index(i), LocationModel::TownRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), LocationModel::PlaceRole).toString());
        line.append(";");
        if (ykj.isEmpty() == true && wgs.isEmpty() == false)
        {
            ykj = mCoordinates->wgsToYkjString(wgs);/*
            QString x = wgs.section(":", 0, 0);
            QString y = wgs.section(":", 1, 1);
            double dx = x.toDouble();
            double dy = y.toDouble();
            double ykjx = 0;
            double ykjy = 0;
            QPair<double,double> newCoord;
            newCoord = mCoordinates->wgsToykj(dx, dy);
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
            ykj.append(ykjY);*/
        }

        if (ykj.isEmpty() == false && wgs.isEmpty() == true)
        {
            wgs = mCoordinates->ykjToWgsString(ykj);/*
            QString x = ykj.section(":", 0, 0);
            QString y = ykj.section(":", 1, 1);
            double dx = x.toDouble();
            double dy = y.toDouble();
            double wgsx = 0;
            double wgsy = 0;
            QPair<double,double> newCoord;
            newCoord = mCoordinates->ykjTowgs(dx, dy);
            wgsx = newCoord.first;
            wgsy = newCoord.second;
            QString wgsX;
            wgsX.setNum(wgsx,'g',6);
            QString wgsY;
            wgsY.setNum(wgsy,'g',6);
            wgs = wgsX;
            wgs.append(":");
            wgs.append(wgsY);*/
        }

        line.append(wgs);
        line.append(";");
        line.append(ykj);
        line.append(";");
        line.append(model->getItem(i).sweTown(true));
        line.append(";");
        line.append(model->getItem(i).swePlace(true));
        line.append(";");
        line.append(model->getItem(i).engTown(true));
        line.append(";");
        line.append(model->getItem(i).engPlace(true));
        line.append(";");
        line.append(model->getItem(i).finCountry());
        line.append(";");
        line.append(model->getItem(i).sweCountry(true));
        line.append(";");
        line.append(model->getItem(i).engCountry(true));
        line.append(";");
        if(model->getItem(i).custom()) {
            line.append("true");
        }
        else {
            line.append("false");
        }
        line.append(";\n");
        striimi << line;
    }
    tiedosto.close();
}


void ModelDataWriter::writeStatusData(StatusModel *model)
{
    QFile tiedosto(dataFileDir() + "xemastatusdata.txt");
    tiedosto.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    int rows = model->rowCount();
    striimi << QString("tila;suomeksi;ruotsiksi;englanniksi;muokattu;\n");
    for(int i = 0; i < rows; i++)
    {
        QString line;
        line.append(model->getItem(i).abbreviation());
        line.append(";");
        line.append(model->getItem(i).name());
        line.append(";");
        line.append(model->getItem(i).sweName(true));
        line.append(";");
        line.append(model->getItem(i).engName(true));
        line.append(";");
        if(model->getItem(i).custom()) {
            line.append("true");
        }
        else {
            line.append("false");
        }
        line.append(";\n");
        striimi << line;
    }
    tiedosto.close();
}

void ModelDataWriter::writeBirdData(BirdModel *model)
{
    QFile tiedosto(dataFileDir() + "xemabirddata.txt");
    tiedosto.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    int rows = model->rowCount();
    striimi << QString("Id;Ryhmät;Ryhmät_eng;Ryhmät_tiet;SUOMI;RUOTSI;LYHENNE;TIETEELLINEN;KATEGORIA;ENGLANTI;Ryhmät_swe;muokattu;\n");
    for(int i = 0; i < rows; i++)
    {
        QString line;
        line.append(model->data(model->index(i), BirdModel::IdRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::FinGroupRole).toString());
        line.append(";");
        line.append(model->getItem(i).engGroup(true));
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::LatinGroupRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::FinNameRole).toString());
        line.append(";");
        line.append(model->getItem(i).sweName(true));
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::AbbrevRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::LatinNameRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::CategoryRole).toString());
        line.append(";");
        line.append(model->getItem(i).engName(true));
        line.append(";");
        line.append(model->getItem(i).sweGroup(true));
        line.append(";");
        if(model->getItem(i).custom()) {
            line.append("true");
        }
        else {
            line.append("false");
        }
        line.append(";\n");
        striimi << line;
    }
    tiedosto.close();
}

void ModelDataWriter::exportHistory(bool onlyNew, LocationModel *locations, PersonModel *persons, BirdModel *birds, const QString &delimiter)
{
    QDateTime date;
    date = QDateTime::currentDateTime();
    QString fileName("xema_exported_history-");
    fileName.append(date.toString("yyyyMMdd-hhmmss"));
    fileName.append(".csv");
//    qDebug() << "void ModelDataWriter::exportHistory(bool onlyNew)" << onlyNew;
    QFile tiedosto(dataFileDir() + "xemadata.txt");
    QFile tmptiedosto(dataFileDir() + "xemadata.tmp.txt");
    QFile exporttiedosto(exportDir() + fileName);

    tiedosto.open(QFile::ReadOnly);
    tmptiedosto.open(QFile::ReadWrite|QFile::Truncate);
    exporttiedosto.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream instriimi(&tiedosto);
    QTextStream export_stream(&exporttiedosto);
    QTextStream tmp_stream(&tmptiedosto);
    instriimi.setCodec("ISO 8859-1");
    export_stream.setCodec("ISO 8859-1");
    tmp_stream.setCodec("ISO 8859-1");

    QString obsLine;
    QString header = QString::fromUtf8("Rivi-ID#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Kunta#Paikka#X-koord#Y-koord#Tarkkuus#X-koord-linnun#Y-koord-linnun#Tarkkuus_linnun#Paikannettu#Lisätietoja#Atlaskoodi#Tallentaja#Tallennusaika#Havainnoijat#Salattu#Koontihavainto#Kuuluu havaintoon#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Epäsuora havainto#Sää#Maa\n");
    if( delimiter != "#") {
        header.replace("#",";");
    }
    export_stream << header;
    while (instriimi.atEnd() == false)
    {
        obsLine = instriimi.readLine();
        if (obsLine.section("#",XemaEnums::OBS_ID,XemaEnums::OBS_ID) == "Id" ||
            obsLine.section("#",XemaEnums::OBS_ID,XemaEnums::OBS_ID) == "Rivi-ID")
        {
            tmp_stream << obsLine;
            tmp_stream << "\n";
            continue;
        }
//        qDebug() << "ObsRivi ennen exporttia" << obsLine;
        int xemaRows = obsLine.section("#", XemaEnums::OBS_ROWCOUNT, XemaEnums::OBS_ROWCOUNT).toInt();
//        qDebug() << Q_FUNC_INFO << "XEMAROWS" << xemaRows;
        int exportPos = XemaEnums::OBS_EXPORTED + ((xemaRows-1) * XemaEnums::OBS_SUBFIELDCOUNT);
        QString exported = obsLine.section('#', exportPos, exportPos);

//        qDebug() << Q_FUNC_INFO << "EXPORTED" << exported;
        int pos = -1;
        int i = 0;
        do
        {
            pos = obsLine.indexOf("#",pos+1);
//            qDebug() << "pos" << pos << i << exportPos;
            if (i == exportPos - 1)
            {
//                qDebug() << "pos" << pos << "paikka" << i;

                break;
            }
            i++;
        }
        while (pos>0);

        if (onlyNew == false || exported == "false")
        {
//            qDebug() << "VALMIS EXPORT RIVI" << formatToTiira(obsLine,locations,persons,birds);
            export_stream << formatToTiira(obsLine,locations,persons,birds,delimiter);
            export_stream << "\n";
        }
        if (obsLine.length() > 20)
        {
//            qDebug() << "rivi ennen export settia" << obsLine;
            QString newLine;
            newLine = obsLine;

            QString start = newLine.mid(0, pos);
//            qDebug() << "start" << start;

            start.append("#true#\n");
//            qDebug() << "uus rivi datassa export setin jalkeen" << start;
            tmp_stream << start;
        }
    }
    tiedosto.close();
    tmptiedosto.close();
    exporttiedosto.close();

    QFile::remove(dataFileDir() + "xemadata.backup");
    tiedosto.rename(dataFileDir() + "xemadata.backup");
    tmptiedosto.rename(dataFileDir() + "xemadata.txt");
}

void ModelDataWriter::exportOwnData(LocationModel *lModel, BirdModel *bModel, StatusModel *sModel, PersonModel *pModel) {
//    qDebug() << "exportOwnData";

    // LOCATIONS
    QFile exportfile(exportDir() + "xema_exported_locations.txt");

    exportfile.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream outstriimi(&exportfile);
    outstriimi.setCodec("ISO 8859-1");

    int lRowCount = lModel->rowCount();

    outstriimi << "town_fi;place_fi;wgs;ykj;town_sv;place_sv;town_en;place_en;country_fi;country_sv;country_en\n";

    for( int i = 0; i < lRowCount; i++ ) {
        if( lModel->getItem(i).custom() == true ) {
            outstriimi << lModel->getItem(i).town() << ";";
            outstriimi << lModel->getItem(i).place() << ";";
            outstriimi << lModel->getItem(i).wgsCoordinate() << ";";
            outstriimi << lModel->getItem(i).ykjCoordinate() << ";";
            outstriimi << lModel->getItem(i).sweTown(true) << ";";
            outstriimi << lModel->getItem(i).swePlace(true) << ";";
            outstriimi << lModel->getItem(i).engTown(true) << ";";
            outstriimi << lModel->getItem(i).engPlace(true) << ";";
            outstriimi << lModel->getItem(i).finCountry() << ";";
            outstriimi << lModel->getItem(i).sweCountry(true) << ";";
            outstriimi << lModel->getItem(i).engCountry(true) << "\n";
        }
    }

    // PERSONS
    QFile exportfile2(exportDir() + "xema_exported_persons.txt");

    exportfile2.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream outstriimi2(&exportfile2);
    outstriimi2.setCodec("ISO 8859-1");

    int pRowCount = pModel->rowCount();

    outstriimi2 << "firstname;lastname;registered;default\n";

    for( int i = 0; i < pRowCount; i++ ) {
        outstriimi2 << pModel->getItem(i).firstName() << ";";
        outstriimi2 << pModel->getItem(i).surName() << ";";
        if( pModel->getItem(i).registered() ) {
            outstriimi2 << "true;";
        }
        else {
            outstriimi2 << "false;";
        }
        if( pModel->getItem(i).defaultName() ) {
            outstriimi2 << "true\n";
        }
        else {
            outstriimi2 << "false\n";
        }
    }

    // BIRDS
    QFile exportfile3(exportDir() + "xema_exported_birds.txt");

    exportfile3.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream outstriimi3(&exportfile3);
    outstriimi3.setCodec("ISO 8859-1");

    int bRowCount = bModel->rowCount();

    outstriimi3 << "id;group_fi;group_en;group_lat;name_fi;name_sv;abbrev_lat;name_lat;category;name_en;group_sv\n";

    for( int i = 0; i < bRowCount; i++ ) {
        if( bModel->getItem(i).custom() == true ) {
            outstriimi3 << bModel->getItem(i).id() << ";";
            outstriimi3 << bModel->getItem(i).finGroup() << ";";
            outstriimi3 << bModel->getItem(i).engGroup(true) << ";";
            outstriimi3 << bModel->getItem(i).latinGroup() << ";";
            outstriimi3 << bModel->getItem(i).finName() << ";";
            outstriimi3 << bModel->getItem(i).sweName(true) << ";";
            outstriimi3 << bModel->getItem(i).abbreviation() << ";";
            outstriimi3 << bModel->getItem(i).latinName() << ";";
            outstriimi3 << bModel->getItem(i).category() << ";";
            outstriimi3 << bModel->getItem(i).engName(true) << ";";
            outstriimi3 << bModel->getItem(i).sweGroup(true) << "\n";
        }
    }


    // STATUSES
    QFile exportfile4(exportDir() + "xema_exported_statuses.txt");

    exportfile4.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream outstriimi4(&exportfile4);
    outstriimi4.setCodec("ISO 8859-1");

    int sRowCount = sModel->rowCount();

    outstriimi4 << "abbrev;status_fi;status_sv;status_en\n";

    for( int i = 0; i < sRowCount; i++ ) {
        if( sModel->getItem(i).custom() == true ) {
            outstriimi4 << sModel->getItem(i).abbreviation() << ";";
            outstriimi4 << sModel->getItem(i).name() << ";";
            outstriimi4 << sModel->getItem(i).sweName(true) << ";";
            outstriimi4 << sModel->getItem(i).engName(true) << "\n";
        }
    }

}

qlonglong ModelDataWriter::getNewId()
{
//    qDebug() << "LUETAAN IDT";
    if (QFile::exists(dataFileDir() + "xemadata.txt") == false)
    {
        return 1;
    }
    QFile tiedosto(dataFileDir() + "xemadata.txt");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    QString obsLine;
    qlonglong maxId = 0;
    while (striimi.atEnd() == false)
    {
        obsLine = striimi.readLine();
        if (obsLine.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong() > maxId)
        {
            maxId = obsLine.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong();
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
    appPath = QString("/home/user/MyDocs/.xema/");
#elif defined MAC_OS_X_VERSION_10_6
    appPath = QString("/Users/Tero/xema/");
#else
    appPath = QString("C:/");

#endif
    return appPath;
}

QString ModelDataWriter::exportDir()
{
    QString appPath = QString("xema/");
    QString exportDir = appPath + QString("exported/");
    QString basePath;
#ifdef Q_OS_SYMBIAN
    if (QFile::exists("E:/")) {
        basePath = QString("E:/");
    }
    else {
        basePath = QString("C:/data/");
    }
#elif defined HARMATTAN
    basePath = QString("/home/user/MyDocs/");
#elif defined MAC_OS_X_VERSION_10_6
    basePath = QString("/Users/Tero/");
#else
    basePath = QString("C:/");
#endif


    return basePath+exportDir;
}

QString ModelDataWriter::importDir()
{
    QString appPath = QString("xema/");
    QString importDir = appPath + QString("import/");
    QString basePath;
#ifdef Q_OS_SYMBIAN
    if (QFile::exists("E:/")) {
        basePath = QString("E:/");
    }
    else {
        basePath = QString("C:/data/");
    }
#elif defined HARMATTAN
    basePath = QString("/home/user/MyDocs/");
#elif defined MAC_OS_X_VERSION_10_6
    basePath = QString("/Users/Tero/");
#else
    basePath = QString("C:/");
#endif


    return basePath+importDir;
}

QString ModelDataWriter::importedDir()
{
    QString appPath = QString("xema/");
    QString importDir = appPath + QString("imported/");
    QString basePath;
#ifdef Q_OS_SYMBIAN
    if (QFile::exists("E:/")) {
        basePath = QString("E:/");
    }
    else {
        basePath = QString("C:/data/");
    }
#elif defined HARMATTAN
    basePath = QString("/home/user/MyDocs/");
#elif defined MAC_OS_X_VERSION_10_6
    basePath = QString("/Users/Tero/");
#else
    basePath = QString("C:/");
#endif


    return basePath+importDir;
}

void ModelDataWriter::removeCustomSpecies()
{
    QFile tiedosto(dataFileDir() + "xemabirddata.txt");
    tiedosto.remove();
}

void ModelDataWriter::removeCustomLocations()
{
    QFile tiedosto(dataFileDir() + "xemalocationdata.txt");
    tiedosto.remove();
}

void ModelDataWriter::removeCustomStatuses()
{
    QFile tiedosto(dataFileDir() + "xemastatusdata.txt");
    tiedosto.remove();
}

void ModelDataWriter::removeCustomObservers()
{
    QFile tiedosto(dataFileDir() + "xemapersondata.txt");
    tiedosto.remove();
}

void ModelDataWriter::removeHistory()
{
    QFile tiedosto(dataFileDir() + "xemadata.txt");
    tiedosto.remove();
}

QString ModelDataWriter::formatToTiira(const QString &data, LocationModel *locations, PersonModel *persons, BirdModel *birds, const QString &delimiter)
{

//    for(int i = 0; i < data.count("#"); i++)
//    {
//        qDebug() << "kentta" << i << "arvo" << data.section("#",i,i);
//    }
    QString id = data.section("#",XemaEnums::OBS_ID, XemaEnums::OBS_ID);
    QString species = data.section("#",XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES);
    QString datetime = data.section("#",XemaEnums::OBS_DATE1,XemaEnums::OBS_TIME2);
    QString vali = data.section("#",XemaEnums::OBS_INFO,XemaEnums::OBS_ATLAS);
    QString nimet = data.section("#",XemaEnums::OBS_REGPERSON, XemaEnums::OBS_OTHERPERSON);
    QString hidden = data.section("#",XemaEnums::OBS_HIDDEN, XemaEnums::OBS_HIDDEN);
    int xemaRows = data.section("#",XemaEnums::OBS_ROWCOUNT,XemaEnums::OBS_ROWCOUNT).toInt();
    QString town = data.section("#", XemaEnums::OBS_TOWN, XemaEnums::OBS_TOWN);
    QString place = data.section("#", XemaEnums::OBS_LOCATION, XemaEnums::OBS_LOCATION);
    QString birdX = data.section("#", XemaEnums::OBS_BIRD_XCOORD, XemaEnums::OBS_BIRD_XCOORD);
    QString birdY = data.section("#", XemaEnums::OBS_BIRD_YCOORD, XemaEnums::OBS_BIRD_YCOORD);
    qDebug() << "EXPORT, birdX " << birdX;
    qDebug() << "EXPORT, birdY " << birdY;

    QString toka = "#" + town + "#" + place;
    QString eka = id;
//    qDebug() << "EXPORT, eka 2" << eka;

    int rowCount = birds->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        if(birds->getItem(i).finName() == species)
        {
            species = birds->getItem(i).abbreviation();
            break;
        }
    }

    eka += "#";
//    qDebug() << "EXPORT, eka 3" << eka;
    eka += species;
//    qDebug() << "EXPORT, eka 4" << eka;
    eka += "#";
//    qDebug() << "EXPORT, eka 5" << eka;
//    qDebug() << "EXPORT, datetime" << datetime;
    if (datetime.section("#",3,3).isEmpty() == true)
    {
//        qDebug() << "EXPORT, eka 5.1" << eka;
        eka += datetime.section("#",0,2);
//        qDebug() << "EXPORT, eka 5.2" << eka;
        eka += "#";
//        qDebug() << "EXPORT, eka 5.3" << eka;
        eka += datetime.section("#",2,2);
//        qDebug() << "EXPORT, eka 5.4" << eka;
    }
    else
    {
        eka += datetime;
//        qDebug() << "EXPORT, eka 6" << eka;
    }
    rowCount = locations->rowCount();

    // add location coordinates if found
    bool locationAdded = false;
    QString country = "";
    for(int i = 0; i < rowCount; i++)
    {
        if (locations->getItem(i).town() == town && locations->getItem(i).place() == place)
        {
            // save country
            country =  locations->getItem(i).finCountry();
            QString coordinate = locations->getItem(i).wgsCoordinate();
            //qDebug() << "export paikka" << coordinate;
            if (coordinate == "" || coordinate == "0:0" || coordinate == "0") {
                break;
            }
            QString x = coordinate.section(":", 0, 0);
            QString y = coordinate.section(":", 1, 1);
            double dx = x.toDouble();
            double dy = y.toDouble();
            double ykjx = 0;
            double ykjy = 0;
            QPair<double,double> newCoord;
            newCoord = mCoordinates->wgsToykj(dx, dy);
            ykjx = newCoord.first;
            ykjy = newCoord.second;
            long newX = ykjx;
            long newY = ykjy;
            QString ykjX;
            ykjX.setNum(newX);
            QString ykjY;
            ykjY.setNum(newY);

            // tiiraan koordinaatit toisinpäin
            toka.append("#" + ykjY + "#" + ykjX);
            locationAdded = true;
            break;
        }
    }
    if (locationAdded == false)
    {
        toka.append("##");
    }

    eka += toka;
//    qDebug() << "EXPORT, eka 7" << eka;
    eka += QString("##");
    eka += birdX;
    eka += "#";
    eka += birdY;
    eka += "#";
    eka += QString ("##");
//    qDebug() << "EXPORT, eka 8" << eka;
    eka += vali;
//    qDebug() << "EXPORT, eka 9" << eka;
    eka += "#";
//    qDebug() << "EXPORT, eka 10" << eka;

    // add data saver (default name if found)
    rowCount = persons->rowCount();
    QString defaultName;
    for(int i = 0; i < rowCount; i++)
    {
        if (persons->getItem(i).defaultName() == true)
        {
            defaultName = persons->getItem(i).firstName() + " " + persons->getItem(i).surName();
            break;
        }
    }

    if (nimet.contains(defaultName) == true)
    {
        eka += defaultName;
//        qDebug() << "EXPORT, eka 11" << eka;
    }
    eka += "#";
//    qDebug() << "EXPORT, eka 12" << eka;
    // tiira saving time
    eka += "#";
//    qDebug() << "EXPORT, eka 13" << eka;

//    qDebug() << "NIMET ennen muutosta" << nimet;
    if (nimet.length() > 1 && nimet.endsWith('#') == false) {
        nimet.replace("#", ", ");
    }
    else {
        nimet.replace("#", "");
    }
    if (nimet.endsWith(", ")) {
        nimet = nimet.mid(0, nimet.length() - 2);
    }
    eka += nimet;
//    qDebug() << "EXPORT, eka 14" << eka;
    eka += "#";
//    qDebug() << "EXPORT, eka 15" << eka;
    eka += hidden;
//    qDebug() << "EXPORT, eka 16" << eka;
    eka += "#";
//    qDebug() << "EXPORT, eka 17" << eka;

    // koontihavainto ja kuuluu havaintoon
    eka += "#";
//    qDebug() << "EXPORT, eka 18" << eka;
    eka += "#";
//    qDebug() << "EXPORT, eka 19" << eka;

    // include exported or not (24 vs 25)
    QString loppu = data.section("#", XemaEnums::OBS_WEATHER+((xemaRows-1)*XemaEnums::OBS_SUBFIELDCOUNT),
                                 XemaEnums::OBS_WEATHER+((xemaRows-1)*XemaEnums::OBS_SUBFIELDCOUNT));

    loppu.prepend("#");
    loppu.append("#");
    loppu.append(country);
    loppu.append("#");

    QString firstRow = data.section("#", XemaEnums::OBS_BIRDCOUNT, XemaEnums::OBS_INDIRECT);
    firstRow.replace("#koiras#", "#k#");
    firstRow.replace("#naaras#", "#n#");
    eka += firstRow;
//    qDebug() << "EXPORT, eka 20" << eka;
    eka += loppu;
//    qDebug() << "EXPORT, eka 21" << eka;

    if (xemaRows > 1) {
        for(int i = 1; i < xemaRows; i++)
        {
            eka += "\n";
//            qDebug() << "EXPORT, eka 22.1" << eka;
            eka += data.section("#", 0, 0);
//            qDebug() << "EXPORT, eka 22.2" << eka;
            eka += "#######################";
//            qDebug() << "EXPORT, eka 22.3" << eka;
            QString rivi = data.section("#", XemaEnums::OBS_BIRDCOUNT+(i*XemaEnums::OBS_SUBFIELDCOUNT), XemaEnums::OBS_INDIRECT+(i*XemaEnums::OBS_SUBFIELDCOUNT));
            rivi.replace("#koiras#", "#k#");
            rivi.replace("#naaras#", "#n#");
            eka += rivi;
//            qDebug() << "EXPORT, eka 22.4" << eka;
            eka += loppu;
//            qDebug() << "EXPORT, eka 22.5" << eka;
        }
    }

//    qDebug() << "EKA ENNEN MUUTOSTA" << eka;
    eka.replace("#true","#X", Qt::CaseSensitive);
//    qDebug() << "EXPORT, eka 23" << eka;
    eka.replace("#false","#", Qt::CaseSensitive);
//    qDebug() << "EXPORT, eka 24" << eka;
//    qDebug() << "EKA MUUTOKSEN JALKEEN" << eka;
    if( eka.endsWith( "#") == true ) {
        eka.remove(eka.size()-1,1);
    }
    if( delimiter != "#" ) {
        eka.replace("#", delimiter);
    }
    qDebug() << "FORMAT TO TIIRA JALKEEN" << eka;
    return eka;
}

int ModelDataWriter::importHistory(LocationModel *locations,  PersonModel *persons)
{
    int importError = XemaEnums::IMPORT_NOERRORS;
//    qDebug() << "IMPORT";
    QDir dir;
    dir.cd(importDir());
    QStringList filters;
    filters << "*history*.csv" << "*history*.txt" << "*record*.csv" << "*record*.txt";;
    QStringList importFiles;
    importFiles = dir.entryList(filters,QDir::Files);
//    qDebug() << "IMPORT" << importFiles;
    for( int fileno = 0; fileno < importFiles.length(); fileno++ ) {
        QFile importfile(importDir() + importFiles.at(fileno));
        importfile.open(QFile::ReadOnly);
        QTextStream importstream(&importfile);
        importstream.setCodec("ISO 8859-1");

        QString delimiter("#");
        QMap<int, int> sectionMap;

        if (importstream.atEnd() == false)
        {
            QString line = importstream.readLine();
            if (line.count(delimiter) < 34) {
//                qDebug() << "SETTING DELIMITER TO ;" << line.count(";");
                delimiter = ";";
            }
            if (line.count(delimiter) < 34) {
//                qDebug() << "INVALID FILE";
                if (!(importError&XemaEnums::IMPORT_HISTORYERROR)) {
                    importError += XemaEnums::IMPORT_HISTORYERROR;
                }
                continue;
            }

            if (line.section(delimiter, XemaEnums::TIIRA_ID, XemaEnums::TIIRA_ID) != "Id" &&
                line.section(delimiter, XemaEnums::TIIRA_ID, XemaEnums::TIIRA_ID) != "Rivi-ID" &&
                line.section(delimiter, XemaEnums::TIIRA_ID, XemaEnums::TIIRA_ID) != "Havainto id" )
            {
                importError += XemaEnums::IMPORT_HISTORYERROR;
                continue;
            }
            sectionMap = getHistorySectionNumbers(line, delimiter);
        }
        //qDebug() << Q_FUNC_INFO << sectionMap;
        QStringList prevLines;
        QString currentLine;
        bool previousNotHandled = true;
        while (importstream.atEnd() == false)
        {
            static int loopCount = 0;
            loopCount++;
            if( !( loopCount % 30 ) )
            {
                QCoreApplication::processEvents();
            }
            if (prevLines.length() == 0)
            {
                prevLines.append(importstream.readLine());
//                qDebug() << "IMPORT eka line" << prevLines;
                continue;
            }
            currentLine = importstream.readLine();
            if (prevLines.last().section(delimiter, XemaEnums::TIIRA_ID, XemaEnums::TIIRA_ID) == currentLine.section(delimiter, XemaEnums::TIIRA_ID, XemaEnums::TIIRA_ID))
            {
//                qDebug() << "Sama id"<< currentLine;
                prevLines.append(currentLine);
                previousNotHandled = true;
                continue;
            }
            else
            {
//                qDebug() << "IMPORT eri id, kasittele edelliset nyt" << currentLine;
                importLineWithSections(sectionMap, prevLines, locations, persons, delimiter);
                prevLines.clear();
                prevLines.append(currentLine);
                previousNotHandled = false;
            }
        }

        importLineWithSections(sectionMap, prevLines, locations, persons, delimiter);

        QDateTime date;
        date = QDateTime::currentDateTime();
        QString importedFileName("history-imported-");
        importedFileName.append(date.toString("yyyyMMdd-hhmmss"));
        importedFileName.append(".csv");
        int nameNumber = 0;
        while (QFile::exists(importedDir()+importedFileName)) {
            QString no;
            no.setNum(nameNumber);
            importedFileName.append("_"+no);
            nameNumber++;
        }
        importfile.rename(importedDir()+importedFileName);
        if (!(importError&XemaEnums::IMPORT_HISTORY_OK)) {
        importError += XemaEnums::IMPORT_HISTORY_OK;
        }
    }
//    qDebug() << "returning" << importError;
    return importError;
}

void ModelDataWriter::importLineWithSections(const QMap<int, int> sectionMap, const QStringList &lines, LocationModel *locations, PersonModel *persons, const QString &delimiter)
{
    if (lines.size() < 1)
    {
        return;
    }
//    qDebug() << "IMPORT lines" << lines;

    QString rowCount;
    rowCount.setNum(lines.size());
    QString readyLine;
// using id 0 to save
//    readyLine += lines.at(0).section(delimiter, XemaEnums::TIIRA_ID, XemaEnums::TIIRA_ID);
//    readyLine.append(delimiter);
    readyLine.append("0#");
    QString readLine = lines.at(0);
    int tiira_abbr = sectionMap.value(XemaEnums::TIIRA_SPECIES_ABBR);
    QString speciesAbbrev = readLine.section(delimiter,tiira_abbr,tiira_abbr);
    readyLine.append(speciesAbbrev);

    int tiira_date1 = sectionMap.value(XemaEnums::TIIRA_DATE1);
    int tiira_date2 = sectionMap.value(XemaEnums::TIIRA_DATE2);
    QString date1 = readLine.section(delimiter, tiira_date1, tiira_date1);
    QString date2 = readLine.section(delimiter, tiira_date2, tiira_date2);

    int faultyLine = 0;
    readyLine.append("#");
    // fix import from old data
//    qDebug() << "DELIMITER COUNT" << readLine.count(delimiter);
    if (date1.isEmpty() && date2.isEmpty() == false) {
        readyLine += date2;
        readyLine.append("#");
        faultyLine++;
        int firstMark = readLine.indexOf(delimiter);
        readLine.remove(firstMark,1);
    }
    else {
        readyLine += date1;
        readyLine.append("#");
        readyLine += date2;
    }
    readyLine.append("#");
    int tiira_time1 = sectionMap.value(XemaEnums::TIIRA_TIME1);
    readyLine += readLine.section(delimiter, tiira_time1, tiira_time1);
    readyLine.append("#");
    int tiira_time2 = sectionMap.value(XemaEnums::TIIRA_TIME2);
    readyLine += readLine.section(delimiter, tiira_time2, tiira_time2);
    readyLine.append("#");

    int tiira_town = sectionMap.value(XemaEnums::TIIRA_TOWN);
    QString town = readLine.section(delimiter, tiira_town, tiira_town);
    if (town.isEmpty()) {
        faultyLine++;
        int firstMark = readLine.indexOf(delimiter);
        readLine.remove(firstMark,1);
        town = readLine.section(delimiter, tiira_town, tiira_town);
    }
    readyLine += town;
    readyLine.append("#");

    int tiira_location = sectionMap.value(XemaEnums::TIIRA_LOCATION);
    QString location = readLine.section(delimiter, tiira_location, tiira_location);
    int tiira_country = sectionMap.value(XemaEnums::TIIRA_EXTRA_COUNTRY);
    QString country = readLine.section(delimiter, tiira_country, tiira_country);
    readyLine += location;
    readyLine.append("#");
    int tiira_xcoord = sectionMap.value(XemaEnums::TIIRA_XCOORD);
    int tiira_ycoord = sectionMap.value(XemaEnums::TIIRA_YCOORD);

    if (locations) {
        bool locationExists = false;
        for (int i = 0; i < locations->rowCount(); i++) {
            if (QString::compare(town,locations->getItem(i).town(), Qt::CaseInsensitive) == 0 ) {
                if (QString::compare(location,locations->getItem(i).place(), Qt::CaseInsensitive) == 0 ) {
                    locationExists = true;
                    break;
                }
            }
        }
        if (locationExists == false && town.trimmed().isEmpty() == false && location.trimmed().isEmpty() == false) {
            Location tmp( town, location );
            QString x;
            QString y;
            QString ykj;
            QString wgs;
            x = readLine.section(delimiter, tiira_xcoord, tiira_xcoord);
            y = readLine.section(delimiter, tiira_ycoord, tiira_ycoord);
            ykj.append(x);
            ykj.append(":");
            ykj.append(y);
            if (x.isEmpty() == false && y.isEmpty() == false && x != "0" && y != "0")
            {
                double dx = x.toDouble();
                double dy = y.toDouble();
                double wgsx = 0;
                double wgsy = 0;
                QPair<double,double> newCoord;
                newCoord = mCoordinates->ykjTowgs(dx, dy);
                wgsx = newCoord.first;
                wgsy = newCoord.second;
                QString wgsX;
                wgsX.setNum(wgsx,'g',6);
                QString wgsY;
                wgsY.setNum(wgsy,'g',6);
                wgs = wgsX;
                wgs.append(":");
                wgs.append(wgsY);
                tmp.setYKJCoordinate(ykj);
                tmp.setWGSCoordinate(wgs);
            }
            if (country.isEmpty() == false) {
              tmp.setFinCountry(country);
            }
            tmp.setCustom(true);
            locations->addItem(tmp);
        }
    }

    QString x = readLine.section(delimiter, tiira_xcoord, tiira_xcoord);
    QString y = readLine.section(delimiter, tiira_ycoord, tiira_ycoord);

    if (x.isEmpty() == false && x != "0" && y.isEmpty() == false && y != "0") {
        readyLine += x;
    }
    readyLine.append("#");

    if (x.isEmpty() == false && x != "0" && y.isEmpty() == false && y != "0") {
        readyLine += y;
    }
    readyLine.append("#");

    int tiira_accuracy = sectionMap.value(XemaEnums::TIIRA_ACCURACY);
    readyLine += readLine.section(delimiter, tiira_accuracy, tiira_accuracy);
    readyLine.append("#");

    int tiira_bird_xcoord = sectionMap.value(XemaEnums::TIIRA_BIRD_XCOORD);
    readyLine += readLine.section(delimiter, tiira_bird_xcoord, tiira_bird_xcoord);
    readyLine.append("#");

    int tiira_bird_ycoord = sectionMap.value(XemaEnums::TIIRA_BIRD_YCOORD);
    readyLine += readLine.section(delimiter, tiira_bird_ycoord, tiira_bird_ycoord);
    readyLine.append("#");

    int tiira_bird_accuracy = sectionMap.value(XemaEnums::TIIRA_BIRD_ACCURACY);
    readyLine += readLine.section(delimiter, tiira_bird_accuracy, tiira_bird_accuracy);
    readyLine.append("#");

    int tiira_paikannettu = sectionMap.value(XemaEnums::TIIRA_PAIKANNETTU);
    readyLine += readLine.section(delimiter, tiira_paikannettu, tiira_paikannettu);
    readyLine.append("#");

    int tiira_info = sectionMap.value(XemaEnums::TIIRA_INFO);
    readyLine += readLine.section(delimiter, tiira_info, tiira_info);
    readyLine.append("#");

    int tiira_atlas = sectionMap.value(XemaEnums::TIIRA_ATLAS);
    readyLine += readLine.section(delimiter, tiira_atlas, tiira_atlas);
    readyLine.append("#");

    int tiira_saver = sectionMap.value(XemaEnums::TIIRA_SAVER);
    QString saver = readLine.section(delimiter, tiira_saver, tiira_saver);
    readyLine += saver;
    readyLine.append("#");

    int tiira_savetime = sectionMap.value(XemaEnums::TIIRA_SAVETIME);
    readyLine += readLine.section(delimiter, tiira_savetime, tiira_savetime);
    readyLine.append("#");

    int tiira_persons = sectionMap.value(XemaEnums::TIIRA_PERSONS);
    QString observers = readLine.section(delimiter, tiira_persons, tiira_persons);
//    observers.remove(saver);
//    qDebug() << "OBS:" <<observers;
    QStringList observerList = observers.split(",");
    QString registeredPeople;

    for (int i = 0;i < observerList.length(); i++)
    {
        registeredPeople += observerList.at(i).trimmed();
        registeredPeople.append(",");
        if (persons)
        {
            bool personFound = false;
            for (int j = 0; j < persons->rowCount(); j++) {
                if (observerList.at(i).trimmed() == persons->getItem(j).name()) {
                    personFound = true;
                    break;
                }
            }
            if (personFound == false) {
                QString firstName = observerList.at(i).trimmed().section(" ",0,0);
                QString lastName = observerList.at(i).trimmed().section(" ",1);
                if( firstName.trimmed().isEmpty() == false && lastName.trimmed().isEmpty() == false) {
                    Person tmp(firstName.trimmed(), lastName.trimmed(), false );
                    persons->addItem(tmp);
                }
            }
        }
    }

    if (registeredPeople.endsWith(","))
    {
        registeredPeople = registeredPeople.left(registeredPeople.length()-1);
    }
    if (registeredPeople.startsWith(","))
    {
        registeredPeople.remove(0, 1);
    }

    readyLine += registeredPeople;
    readyLine.append("#");
    readyLine.append("#"); // other people

    // hidden
    int tiira_hidden = sectionMap.value(XemaEnums::TIIRA_HIDDEN);
    if(QString::compare(readLine.section(delimiter, tiira_hidden, tiira_hidden),"X", Qt::CaseInsensitive)==0)
    {
        readyLine.append("true#");
    }
    else
    {
        readyLine.append("false#");
    }
    // koontihavainto, kuuluu havaintoon 21,22
    readyLine.append("#");
    readyLine.append("#");
    readyLine += rowCount;
    readyLine.append("#");

    int tiira_weather = sectionMap.value(XemaEnums::TIIRA_EXTRA_WEATHER);
    QString weather = readLine.section(delimiter,tiira_weather,tiira_weather);
    for(int i = 0;i < lines.length();i++)
    {
        QString row;
        int tiira_birdcount = sectionMap.value(XemaEnums::TIIRA_BIRDCOUNT);
        int offset = tiira_birdcount;
        int tiira_birdtime1 = sectionMap.value(XemaEnums::TIIRA_BIRDTIME1);
        int tiira_birdtime2 = sectionMap.value(XemaEnums::TIIRA_BIRDTIME2);
        int tiira_sex = sectionMap.value(XemaEnums::TIIRA_SEX);
        int tiira_dress = sectionMap.value(XemaEnums::TIIRA_DRESS);
        int tiira_age = sectionMap.value(XemaEnums::TIIRA_AGE);
        int tiira_status = sectionMap.value(XemaEnums::TIIRA_STATUS);
        int tiira_birdinfo = sectionMap.value(XemaEnums::TIIRA_BIRDINFO);
        int tiira_loft = sectionMap.value(XemaEnums::TIIRA_LOFT);
        int tiira_bongaus = sectionMap.value(XemaEnums::TIIRA_BONGAUS);
        int tiira_nest = sectionMap.value(XemaEnums::TIIRA_NEST);
        int tiira_indirect = sectionMap.value(XemaEnums::TIIRA_INDIRECT);

        row = lines.at(i).section(delimiter, tiira_birdcount, tiira_indirect);
        for ( int faults = faultyLine; faults > 0; faults--) {
            int firstMark = row.indexOf(delimiter);
            row.remove(firstMark,1);
        }

        //qDebug() << "alarivi" << i << "data" << row;
        readyLine += row.section(delimiter,tiira_birdcount-offset,tiira_birdcount-offset);
        readyLine.append("#");
        readyLine += row.section(delimiter,tiira_birdtime1-offset,tiira_birdtime1-offset);
        readyLine.append("#");
        readyLine += row.section(delimiter,tiira_birdtime2-offset,tiira_birdtime2-offset);
        if (row.section(delimiter,tiira_sex-offset,tiira_sex-offset) == "k")
        {
            readyLine.append("#koiras#");
        }
        else if (row.section(delimiter,tiira_sex-offset,tiira_sex-offset) == "n")
        {
            readyLine.append("#naaras#");
        }
        else if (row.section(delimiter,tiira_sex-offset,tiira_sex-offset) == "p")
        {
            readyLine.append("#pariutuneet#");
        }
        else
        {
            readyLine.append("#");
            readyLine += row.section(delimiter,tiira_sex-offset,tiira_sex-offset);
            readyLine.append("#");
        }
        readyLine += row.section(delimiter,tiira_dress-offset,tiira_dress-offset);
        readyLine.append("#");
        readyLine += row.section(delimiter,tiira_age-offset,tiira_age-offset);
        readyLine.append("#");
        readyLine += row.section(delimiter,tiira_status-offset,tiira_status-offset);
        readyLine.append("#");
        readyLine += row.section(delimiter,tiira_birdinfo-offset,tiira_birdinfo-offset);
        readyLine.append("#");
        readyLine += row.section(delimiter,tiira_loft-offset,tiira_loft-offset);
        readyLine.append("#");
        if(QString::compare(row.section(delimiter,tiira_bongaus-offset,tiira_bongaus-offset),"X", Qt::CaseInsensitive)==0)
        {
            readyLine.append("true#");
        }
        else
        {
            readyLine.append("false#");
        }
        if(QString::compare(row.section(delimiter,tiira_nest-offset,tiira_nest-offset),"X", Qt::CaseInsensitive)==0)
        {
            readyLine.append("true#");
        }
        else
        {
            readyLine.append("false#");
        }

        readyLine += row.section(delimiter,tiira_indirect-offset,tiira_indirect-offset);
        readyLine.append("#");
    }
    readyLine.append(weather);
    // exported to true
    readyLine.append("#true#");

    //qDebug() << "VALMIS RIVI" << readyLine;

    writeNewObservation(readyLine);
}


void ModelDataWriter::checkAndCreateDirs() {
    qDebug() << "checkAndCreateDirs";
    QString appPath = QString("xema/");
    QString exportDir = appPath + QString("exported/");
    QString importDir = appPath + QString("import/");
    QString importedDir = appPath + QString("imported/");
    QString basePath;
#ifdef Q_OS_SYMBIAN
    if (QFile::exists("E:/")) {
        basePath = QString("E:/");
    }
    else {
        basePath = QString("C:/data/");
    }


#elif defined HARMATTAN
    basePath = QString("/home/user/MyDocs/");
    if (QFile::exists(basePath+".xema/") == false) {
        QDir dir;
        dir.mkpath(basePath+".xema/");
    }

#elif defined MAC_OS_X_VERSION_10_6
    basePath = QString("/Users/Tero/");
#else
    basePath = QString("C:/");
#endif
    QDir dir;

    if (QFile::exists(basePath+exportDir) == false) {
        dir.mkpath(basePath+exportDir);
    }
    if (QFile::exists(basePath+importDir) == false) {
        dir.mkpath(basePath+importDir);
    }
    if (QFile::exists(basePath+importedDir) == false) {
        dir.mkpath(basePath+importedDir);
    }

}

int ModelDataWriter::importOwnData( LocationModel *locations, PersonModel *persons, BirdModel *birds, StatusModel *statuses) {
    int importError = XemaEnums::IMPORT_NOERRORS;
    QDir dir;
    dir.cd(importDir());
    QStringList filters;
    filters << "*location*.csv" << "*location*.txt";
    QStringList importFiles;
    importFiles = dir.entryList(filters,QDir::Files);
//    qDebug() << "IMPORT LOCATIONS" << importFiles;

    for( int fileno = 0; fileno < importFiles.length(); fileno++ ) {
        QFile importfile(importDir() + importFiles.at(fileno));
        importfile.open(QFile::ReadOnly);
        QTextStream importstream(&importfile);
        importstream.setCodec("ISO 8859-1");

        QString delimiter("#");
        int delimCount = 0;
        QMap<int, int> sectionMap;

        if (importstream.atEnd() == false) {
            QString line = importstream.readLine();
            delimCount = line.count(delimiter);
            if (delimCount != 7 && delimCount != 8 && delimCount != 10 && delimCount != 11) {
                qDebug() << "SETTING DELIMITER TO ;" << line.count(";");
                delimiter = ";";
                delimCount = line.count(delimiter);
            }
            if (delimCount != 7 && delimCount != 8 && delimCount != 10 && delimCount != 11) {
                qDebug() << "INVALID LOCATION FILE";
                if (!(importError&XemaEnums::IMPORT_LOCATIONERROR)) {
                    importError += XemaEnums::IMPORT_LOCATIONERROR;
                }
                continue;
            }
            sectionMap = getLocationSectionNumbers(line, delimiter);
            importstream.seek(0);
        }
        while (importstream.atEnd() == false) {
            QString locationLine;
            locationLine = importstream.readLine();
            QString firstSection = locationLine.section(';', XemaEnums::LOCATION_TOWN, XemaEnums::LOCATION_TOWN);
            if (firstSection == "kunta" || firstSection == "town_fi" ) {
                continue;
            }
            int location_town = sectionMap.value(XemaEnums::LOCATION_TOWN);
            int location_place = sectionMap.value(XemaEnums::LOCATION_PLACE);
            int location_wgs = sectionMap.value(XemaEnums::LOCATION_WGS);
            int location_ykj = sectionMap.value(XemaEnums::LOCATION_YKJ);
            int location_swetown = sectionMap.value(XemaEnums::LOCATION_SWETOWN);
            int location_engtown = sectionMap.value(XemaEnums::LOCATION_ENGTOWN);
            int location_sweplace = sectionMap.value(XemaEnums::LOCATION_SWEPLACE);
            int location_engplace = sectionMap.value(XemaEnums::LOCATION_ENGPLACE);
            int location_country = sectionMap.value(XemaEnums::LOCATION_COUNTRY);
            int location_swecountry = sectionMap.value(XemaEnums::LOCATION_SWECOUNTRY);
            int location_engcountry = sectionMap.value(XemaEnums::LOCATION_ENGCOUNTRY);

            Location location(locationLine.section(';', location_town, location_town),
                              locationLine.section(';', location_place, location_place),
                              locationLine.section(';', location_wgs, location_wgs),
                              locationLine.section(';', location_ykj, location_ykj));
            location.setSweTown(locationLine.section(';', location_swetown, location_swetown));
            location.setEngTown(locationLine.section(';', location_engtown, location_engtown));
            location.setSwePlace(locationLine.section(';', location_sweplace, location_sweplace));
            location.setEngPlace(locationLine.section(';', location_engplace, location_engplace));
            if( delimCount > 9) {
                location.setFinCountry(locationLine.section(';', location_country, location_country));
                location.setSweCountry(locationLine.section(';', location_swecountry, location_swecountry));
                location.setEngCountry(locationLine.section(';', location_engcountry, location_engcountry));
            }
            location.setCustom(true);

            int rows = locations->rowCount();
            bool matchFound = false;
            for (int i = 0; i < rows; i++) {
                if (QString::compare(location.town(), locations->getItem(i).town(), Qt::CaseInsensitive) == 0)
                {
                    if (QString::compare(location.place(), locations->getItem(i).place(), Qt::CaseInsensitive) == 0)
                    {
//                        qDebug() << "paikka loyty";
                        if (location.wgsCoordinate().isEmpty() == false && locations->getItem(i).wgsCoordinate().isEmpty()) {
                            locations->replaceItem(i,location);
                        }
                        if (location.ykjCoordinate().isEmpty() == false && locations->getItem(i).ykjCoordinate().isEmpty()) {
                            locations->replaceItem(i,location);
                        }
                        matchFound = true;
                        break;
                    }
                }
            }
            if (matchFound == false) {
                locations->addItem(location);
            }
        }
        QDateTime date;
        date = QDateTime::currentDateTime();
        QString importedLocationFileName("locations-imported-");
        importedLocationFileName.append(date.toString("yyyyMMdd-hhmmss"));
        importedLocationFileName.append(".csv");
        int nameNumber = 0;
        while (QFile::exists(importedDir()+importedLocationFileName)) {
            QString no;
            no.setNum(nameNumber);
            importedLocationFileName.append("_"+no);
            nameNumber++;
        }
        importfile.rename(importedDir()+importedLocationFileName);
        if (!(importError&XemaEnums::IMPORT_LOCATION_OK)) {
            importError += XemaEnums::IMPORT_LOCATION_OK;
        }
    }


    // PERSONS

    filters.clear();
    filters << "*person*.csv" << "*person*.txt" << "*people*.csv" << "*people*.txt" << "*observer*.csv" << "*observer*.txt";
    importFiles.clear();

    importFiles = dir.entryList(filters,QDir::Files);
//    qDebug() << "IMPORT PEOPLE" << importFiles;
    for( int fileno = 0; fileno < importFiles.length(); fileno++ ) {
        QFile importfile(importDir() + importFiles.at(fileno));
        importfile.open(QFile::ReadOnly);
        QTextStream importstream(&importfile);
        importstream.setCodec("ISO 8859-1");

        QString delimiter("#");
        QMap<int, int> sectionMap;

        if (importstream.atEnd() == false) {
            QString line = importstream.readLine();
            if (line.count(delimiter) != 3) {
//                qDebug() << "SETTING DELIMITER TO ;" << line.count(";");
                delimiter = ";";
            }
            if (line.count(delimiter) != 3) {
//                qDebug() << "INVALID PERSON FILE";
                if (!(importError&XemaEnums::IMPORT_PERSONERROR)) {
                    importError += XemaEnums::IMPORT_PERSONERROR;
                }
                continue;
            }
            importstream.seek(0);
            sectionMap = getLocationSectionNumbers(line, delimiter);
        }
        while (importstream.atEnd() == false) {
            QString importLine;
            importLine = importstream.readLine();
            if(importLine.section(';', XemaEnums::PERSON_FIRSTNAME, XemaEnums::PERSON_FIRSTNAME) == "firstname") {
                continue;
            }
            int person_registered = sectionMap.value(XemaEnums::PERSON_REGISTERED);
            int person_default = sectionMap.value(XemaEnums::PERSON_DEFAULT);
            int person_firstname = sectionMap.value(XemaEnums::PERSON_FIRSTNAME);
            int person_surname = sectionMap.value(XemaEnums::PERSON_SURNAME);
            bool registered = false;
            if (QString::compare(importLine.section(';', person_registered, person_registered), "true", Qt::CaseInsensitive) == 0) {
                registered = true;
            }
            bool defaultName = false;
            if (QString::compare(importLine.section(';', person_default, person_default), "true", Qt::CaseInsensitive) == 0) {
                defaultName = true;
            }
            Person person(importLine.section(';', person_firstname, person_firstname),
                              importLine.section(';', person_surname, person_surname),
                              registered, defaultName );
            int rows = persons->rowCount();
            bool matchFound = false;
            for (int i = 0; i < rows; i++) {
                if (QString::compare(person.name(), persons->getItem(i).name(), Qt::CaseInsensitive) == 0)
                {
//                    qDebug() << "person loyty";
                    if (person.registered() != persons->getItem(i).registered()) {
                        persons->replaceItem(i,person);
                    }
                    matchFound = true;
                    break;
                }
            }
            if (matchFound == false) {
                persons->addItem(person);
            }
        }
        QDateTime date;
        date = QDateTime::currentDateTime();
        QString importedPersonFileName("people-imported-");
        importedPersonFileName.append(date.toString("yyyyMMdd-hhmmss"));
        importedPersonFileName.append(".csv");
        int nameNumber = 0;
        while (QFile::exists(importedDir()+importedPersonFileName)) {
            QString no;
            no.setNum(nameNumber);
            importedPersonFileName.append("_"+no);
            nameNumber++;
        }
        importfile.rename(importedDir()+importedPersonFileName);
        if (!(importError&XemaEnums::IMPORT_PERSON_OK)) {
            importError += XemaEnums::IMPORT_PERSON_OK;
        }
    }


    // BIRDS

    filters.clear();
    filters << "*birds*.csv" << "*birds*.txt" << "*species*.csv" << "*species*.txt";
    importFiles.clear();

    importFiles = dir.entryList(filters,QDir::Files);
//    qDebug() << "IMPORT BIRDS" << importFiles;
    for( int fileno = 0; fileno < importFiles.length(); fileno++ ) {
        QFile importfile(importDir() + importFiles.at(fileno));
        importfile.open(QFile::ReadOnly);
        QTextStream importstream(&importfile);
        importstream.setCodec("ISO 8859-1");

        QString delimiter("#");
        QMap<int, int> sectionMap;

        if (importstream.atEnd() == false) {
            QString line = importstream.readLine();
            if (line.count(delimiter) != 10) {
//                qDebug() << "SETTING DELIMITER TO ;" << line.count(";");
                delimiter = ";";
            }
            if (line.count(delimiter) != 10) {
//                qDebug() << "INVALID BIRD FILE";
                if (!(importError&XemaEnums::IMPORT_BIRDERROR)) {
                    importError += XemaEnums::IMPORT_BIRDERROR;
                }
                continue;
            }
            importstream.seek(0);
            sectionMap = getLocationSectionNumbers(line, delimiter);
        }
        while (importstream.atEnd() == false) {
            QString importLine;
            importLine = importstream.readLine();
            QString firstSection = importLine.section(';', XemaEnums::BIRD_ID, XemaEnums::BIRD_ID);

            if( firstSection == "Id" || firstSection == "id" ) {
                continue;
            }

            int bird_id = sectionMap.value(XemaEnums::BIRD_ID);
            int bird_fin_group = sectionMap.value(XemaEnums::BIRD_FIN_GROUP);
            int bird_swe_group = sectionMap.value(XemaEnums::BIRD_SWE_GROUP);
            int bird_latin_group = sectionMap.value(XemaEnums::BIRD_LATIN_GROUP);
            int bird_fin_name = sectionMap.value(XemaEnums::BIRD_FIN_NAME);
            int bird_swe_name = sectionMap.value(XemaEnums::BIRD_SWE_NAME);
            int bird_abbrev = sectionMap.value(XemaEnums::BIRD_ABBREV);
            int bird_latin_name = sectionMap.value(XemaEnums::BIRD_LATIN_NAME);
            int bird_category = sectionMap.value(XemaEnums::BIRD_CATEGORY);
            int bird_eng_name = sectionMap.value(XemaEnums::BIRD_ENG_NAME);
            int bird_eng_group = sectionMap.value(XemaEnums::BIRD_ENG_GROUP);

            Bird bird(importLine.section(';', bird_id, bird_id).toInt(),
                       importLine.section(';', bird_fin_group, bird_fin_group),
                       importLine.section(';', bird_swe_group, bird_swe_group),
                       importLine.section(';', bird_latin_group, bird_latin_group),
                       importLine.section(';', bird_fin_name, bird_fin_name),
                       importLine.section(';', bird_swe_name, bird_swe_name),
                       importLine.section(';', bird_abbrev, bird_abbrev),
                       importLine.section(';', bird_latin_name, bird_latin_name),
                       importLine.section(';', bird_category, bird_category));
            // TODO LOC
            bird.setEngName(importLine.section(';', bird_eng_name, bird_eng_name));
            bird.setEngGroup(importLine.section(';', bird_eng_group, bird_eng_group));
            bird.setCustom(true);

            int rows = birds->rowCount();
            bool matchFound = false;
            for (int i = 0; i < rows; i++) {
                if (QString::compare(bird.abbreviation(), birds->getItem(i).abbreviation(), Qt::CaseInsensitive) == 0)
                {
//                    qDebug() << "bird loyty" << bird.finGroup() << birds->getItem(i).finGroup();
                    if (QString::compare(bird.category(), birds->getItem(i).category(), Qt::CaseInsensitive) != 0 ||
                            QString::compare(bird.finGroup(), birds->getItem(i).finGroup(), Qt::CaseInsensitive) != 0 ||
                            QString::compare(bird.sweGroup(), birds->getItem(i).sweGroup(), Qt::CaseInsensitive) != 0 ||
                            QString::compare(bird.latinGroup(), birds->getItem(i).latinGroup(), Qt::CaseInsensitive) != 0 ||
                            QString::compare(bird.finName(), birds->getItem(i).finName(), Qt::CaseInsensitive) != 0 ||
                            QString::compare(bird.sweName(), birds->getItem(i).sweName(), Qt::CaseInsensitive) != 0 ||
                            QString::compare(bird.latinName(), birds->getItem(i).latinName(), Qt::CaseInsensitive) != 0 ) {
//                        qDebug() << "eri versio, korvataan";
                        birds->replaceItem(i,bird);
                    }
                    matchFound = true;
                    break;
                }
            }
            if (matchFound == false) {
                birds->addItem(bird);
            }
        }
        QDateTime date;
        date = QDateTime::currentDateTime();
        QString importedBirdFileName("birds-imported-");
        importedBirdFileName.append(date.toString("yyyyMMdd-hhmmss"));
        importedBirdFileName.append(".csv");
        int nameNumber = 0;
        while (QFile::exists(importedDir()+importedBirdFileName)) {
            QString no;
            no.setNum(nameNumber);
            importedBirdFileName.append("_"+no);
            nameNumber++;
        }
        importfile.rename(importedDir()+importedBirdFileName);
        if (!(importError&XemaEnums::IMPORT_BIRD_OK)) {
            importError += XemaEnums::IMPORT_BIRD_OK;
        }
    }

    // STATUSES

    filters.clear();
    filters << "*status*.csv" << "*status*.txt";
    importFiles.clear();

    importFiles = dir.entryList(filters,QDir::Files);
//    qDebug() << "IMPORT STATUSES" << importFiles;
    for( int fileno = 0; fileno < importFiles.length(); fileno++ ) {
        QFile importfile(importDir() + importFiles.at(fileno));
        importfile.open(QFile::ReadOnly);
        QTextStream importstream(&importfile);
        importstream.setCodec("ISO 8859-1");

        QString delimiter("#");
        QMap<int, int> sectionMap;

        if (importstream.atEnd() == false) {
            QString line = importstream.readLine();
            if (line.count(delimiter) != 3) {
//                qDebug() << "SETTING DELIMITER TO ;" << line.count(";");
                delimiter = ";";
            }
            if (line.count(delimiter) != 3) {
//                qDebug() << "INVALID STATUS FILE";
                if (!(importError&XemaEnums::IMPORT_STATUSERROR)) {
                    importError += XemaEnums::IMPORT_STATUSERROR;
                }
                continue;
            }
            importstream.seek(0);
            sectionMap = getLocationSectionNumbers(line, delimiter);
        }
        while (importstream.atEnd() == false) {
            QString importLine;
            importLine = importstream.readLine();
            if(importLine.section(';', XemaEnums::STATUS_FINABBREV, XemaEnums::STATUS_FINABBREV) == "abbrev") {
                continue;
            }
            int status_finname = sectionMap.value(XemaEnums::STATUS_FINNAME);
            int status_swename = sectionMap.value(XemaEnums::STATUS_SWENAME);
            int status_engname = sectionMap.value(XemaEnums::STATUS_ENGNAME);
            int status_finabbrev = sectionMap.value(XemaEnums::STATUS_FINABBREV);

            Status status(importLine.section(';', status_finname, status_finname),
                       importLine.section(';', status_swename, status_swename),
                       importLine.section(';', status_engname, status_engname),
                       importLine.section(';', status_finabbrev, status_finabbrev));

            status.setCustom(true);
            int rows = statuses->rowCount();
            bool matchFound = false;
            for (int i = 0; i < rows; i++) {
                if (QString::compare(status.abbreviation(), statuses->getItem(i).abbreviation(), Qt::CaseInsensitive) == 0)
                {
//                    qDebug() << "status loyty" << status.name() << statuses->getItem(i).name();
                    if (QString::compare(status.name(), statuses->getItem(i).name(), Qt::CaseInsensitive) != 0 ||
                            QString::compare(status.sweName(), statuses->getItem(i).sweName(), Qt::CaseInsensitive) != 0 ||
                            QString::compare(status.engName(), statuses->getItem(i).engName(), Qt::CaseInsensitive) != 0 ) {
//                        qDebug() << "eri versio, korvataan";
                        statuses->replaceItem(i,status);
                    }
                    matchFound = true;
                    break;
                }
            }
            if (matchFound == false) {
                statuses->addItem(status);
            }
        }
        QDateTime date;
        date = QDateTime::currentDateTime();
        QString importedStatusFileName("statuses-imported-");
        importedStatusFileName.append(date.toString("yyyyMMdd-hhmmss"));
        importedStatusFileName.append(".csv");
        int nameNumber = 0;
        while (QFile::exists(importedDir()+importedStatusFileName)) {
            QString no;
            no.setNum(nameNumber);
            importedStatusFileName.append("_"+no);
            nameNumber++;
        }
        importfile.rename(importedDir()+importedStatusFileName);
        if (!(importError&XemaEnums::IMPORT_STATUS_OK)) {
            importError += XemaEnums::IMPORT_STATUS_OK;
        }
    }
    return importError;
}

QMap<int, int> ModelDataWriter::getHistorySectionNumbers(const QString &headerLine, const QString &delimiter) {
    QMap<int, int> sections;
    //qDebug() << Q_FUNC_INFO << "header" << headerLine;
    QStringList headerSections = headerLine.toLower().split(delimiter);
    //qDebug() << Q_FUNC_INFO << "headerSections" << headerSections;
    for (int i = XemaEnums::TIIRA_ID; i <= XemaEnums::TIIRA_EXTRA_COUNTRY; i++) {
        switch (i) {
            case XemaEnums::TIIRA_ID: {
                int index = headerSections.indexOf("rivi-id");
                if (index < 0) {
                    index = headerSections.indexOf("havainto id");
                }
                if (index < 0) {
                    index = headerSections.indexOf("id");
                }
                sections.insert(XemaEnums::TIIRA_ID, index);
                break;
            }
            case XemaEnums::TIIRA_SPECIES_ABBR: {
                int index = headerSections.indexOf("laji");
                sections.insert(XemaEnums::TIIRA_SPECIES_ABBR, index);
                break;
            }
            case XemaEnums::TIIRA_DATE1: {
                int index = headerSections.indexOf("pvm1");
                sections.insert(XemaEnums::TIIRA_DATE1, index);
                break;
            }
            case XemaEnums::TIIRA_DATE2: {
                int index = headerSections.indexOf("pvm2");
                sections.insert(XemaEnums::TIIRA_DATE2, index);
                break;
            }
            case XemaEnums::TIIRA_TIME1: {
                int index = headerSections.indexOf("kello_hav_1");
                sections.insert(XemaEnums::TIIRA_TIME1, index);
                break;
            }
            case XemaEnums::TIIRA_TIME2: {
                int index = headerSections.indexOf("kello_hav_2");
                sections.insert(XemaEnums::TIIRA_TIME2, index);
                break;
            }
            case XemaEnums::TIIRA_TOWN: {
                int index = headerSections.indexOf("kunta");
                sections.insert(XemaEnums::TIIRA_TOWN, index);
                break;
            }
            case XemaEnums::TIIRA_LOCATION: {
                int index = headerSections.indexOf("paikka");
                sections.insert(XemaEnums::TIIRA_LOCATION, index);
                break;
            }
            case XemaEnums::TIIRA_YCOORD: {
                int index = headerSections.indexOf("y-koord");
                sections.insert(XemaEnums::TIIRA_YCOORD, index);
                break;
            }
            case XemaEnums::TIIRA_XCOORD: {
                int index = headerSections.indexOf("x-koord");
                sections.insert(XemaEnums::TIIRA_XCOORD, index);
                break;
            }
            case XemaEnums::TIIRA_ACCURACY: {
                int index = headerSections.indexOf("tarkkuus");
                sections.insert(XemaEnums::TIIRA_ACCURACY, index);
                break;
            }
            case XemaEnums::TIIRA_BIRD_XCOORD: {
                int index = headerSections.indexOf("x-koord-linnun");
                sections.insert(XemaEnums::TIIRA_BIRD_XCOORD, index);
                break;
            }
            case XemaEnums::TIIRA_BIRD_YCOORD: {
                int index = headerSections.indexOf("y-koord-linnun");
                sections.insert(XemaEnums::TIIRA_BIRD_YCOORD, index);
                break;
            }
            case XemaEnums::TIIRA_BIRD_ACCURACY: {
                int index = headerSections.indexOf("tarkkuus_linnun");
                sections.insert(XemaEnums::TIIRA_BIRD_ACCURACY, index);
                break;
            }
            case XemaEnums::TIIRA_PAIKANNETTU: {
                int index = headerSections.indexOf("paikannettu");
                sections.insert(XemaEnums::TIIRA_PAIKANNETTU, index);
                break;
            }
            case XemaEnums::TIIRA_INFO: {
                int index = headerSections.indexOf("listietoja");
                sections.insert(XemaEnums::TIIRA_INFO, index);
                break;
            }
            case XemaEnums::TIIRA_ATLAS: {
                int index = headerSections.indexOf("atlaskoodi");
                sections.insert(XemaEnums::TIIRA_ATLAS, index);
                break;
            }
            case XemaEnums::TIIRA_SAVER: {
                int index = headerSections.indexOf("tallentaja");
                sections.insert(XemaEnums::TIIRA_SAVER, index);
                break;
            }
            case XemaEnums::TIIRA_SAVETIME: {
                int index = headerSections.indexOf("tallennusaika");
                sections.insert(XemaEnums::TIIRA_SAVETIME, index);
                break;
            }
            case XemaEnums::TIIRA_PERSONS: {
                int index = headerSections.indexOf("havainnoijat");
                sections.insert(XemaEnums::TIIRA_PERSONS, index);
                break;
            }
            case XemaEnums::TIIRA_HIDDEN: {
                int index = headerSections.indexOf("salattu");
                sections.insert(XemaEnums::TIIRA_HIDDEN, index);
                break;
            }
            case XemaEnums::TIIRA_KOONTIHAVAINTO: {
                int index = headerSections.indexOf("koontihavainto");
                sections.insert(XemaEnums::TIIRA_KOONTIHAVAINTO, index);
                break;
            }
            case XemaEnums::TIIRA_KUULUUHAVAINTOON: {
                int index = headerSections.indexOf("kuuluu havaintoon");
                sections.insert(XemaEnums::TIIRA_KUULUUHAVAINTOON, index);
                break;
            }
            case XemaEnums::TIIRA_BIRDCOUNT: {
                int index = headerSections.indexOf("mr");
                sections.insert(XemaEnums::TIIRA_BIRDCOUNT, index);
                break;
            }
            case XemaEnums::TIIRA_BIRDTIME1: {
                int index = headerSections.indexOf("kello_lintu_1");
                sections.insert(XemaEnums::TIIRA_BIRDTIME1, index);
                break;
            }
            case XemaEnums::TIIRA_BIRDTIME2: {
                int index = headerSections.indexOf("kello_lintu_2");
                sections.insert(XemaEnums::TIIRA_BIRDTIME2, index);
                break;
            }
            case XemaEnums::TIIRA_SEX: {
                int index = headerSections.indexOf("sukupuoli");
                sections.insert(XemaEnums::TIIRA_SEX, index);
                break;
            }
            case XemaEnums::TIIRA_DRESS: {
                int index = headerSections.indexOf("puku");
                sections.insert(XemaEnums::TIIRA_DRESS, index);
                break;
            }
            case XemaEnums::TIIRA_AGE: {
                int index = headerSections.indexOf("ik");
                sections.insert(XemaEnums::TIIRA_AGE, index);
                break;
            }
            case XemaEnums::TIIRA_STATUS: {
                int index = headerSections.indexOf("tila");
                sections.insert(XemaEnums::TIIRA_STATUS, index);
                break;
            }
            case XemaEnums::TIIRA_BIRDINFO: {
                int index = headerSections.indexOf("listietoja_2");
                sections.insert(XemaEnums::TIIRA_BIRDINFO, index);
                break;
            }
            case XemaEnums::TIIRA_LOFT: {
                int index = headerSections.indexOf("parvi");
                sections.insert(XemaEnums::TIIRA_LOFT, index);
                break;
            }
            case XemaEnums::TIIRA_BONGAUS: {
                int index = headerSections.indexOf("bongattu");
                sections.insert(XemaEnums::TIIRA_BONGAUS, index);
                break;
            }
            case XemaEnums::TIIRA_NEST: {
                int index = headerSections.indexOf("pesint");
                sections.insert(XemaEnums::TIIRA_NEST, index);
                break;
            }
            case XemaEnums::TIIRA_INDIRECT: {
                int index = headerSections.indexOf("epsuora havainto");
                sections.insert(XemaEnums::TIIRA_INDIRECT, index);
                break;
            }
            case XemaEnums::TIIRA_EXTRA_WEATHER: {
                int index = headerSections.indexOf("s");
                sections.insert(XemaEnums::TIIRA_EXTRA_WEATHER, index);
                break;
            }
            case XemaEnums::TIIRA_EXTRA_COUNTRY: {
                int index = headerSections.indexOf("maa");
                sections.insert(XemaEnums::TIIRA_EXTRA_COUNTRY, index);
                break;
            }
        }
    }

    return sections;
}

QMap<int, int> ModelDataWriter::getBirdSectionNumbers(const QString &headerLine, const QString &delimiter) {
    QMap<int, int> sections;
    //qDebug() << Q_FUNC_INFO << "header" << headerLine;
    QStringList headerSections = headerLine.toLower().split(delimiter);
    //qDebug() << Q_FUNC_INFO << "headerSections" << headerSections;
    for (int i = XemaEnums::BIRD_ID; i <= XemaEnums::BIRD_SWE_GROUP; i++) {
        switch (i) {
            case XemaEnums::BIRD_ID: {
                int index = headerSections.indexOf("id");
                sections.insert(XemaEnums::BIRD_ID, index);
                break;
            }
            case XemaEnums::BIRD_FIN_GROUP: {
                int index = headerSections.indexOf("group_fi");
                if (index < 0) {
                    index = headerSections.indexOf("ryhmä");
                }
                sections.insert(XemaEnums::BIRD_FIN_GROUP, index);
                break;
            }
            case XemaEnums::BIRD_ENG_GROUP: {
                int index = headerSections.indexOf("group_en");
                sections.insert(XemaEnums::BIRD_ENG_GROUP, index);
                break;
            }
            case XemaEnums::BIRD_LATIN_GROUP: {
                int index = headerSections.indexOf("group_lat");
                sections.insert(XemaEnums::BIRD_LATIN_GROUP, index);
                break;
            }
            case XemaEnums::BIRD_FIN_NAME: {
                int index = headerSections.indexOf("name_fi");
                if (index < 0) {
                    index = headerSections.indexOf("laji");
                }
                sections.insert(XemaEnums::BIRD_FIN_NAME, index);
                break;
            }
            case XemaEnums::BIRD_SWE_NAME: {
                int index = headerSections.indexOf("name_sv");
                if (index < 0) {
                    index = headerSections.indexOf("art");
                }
                sections.insert(XemaEnums::BIRD_SWE_NAME, index);
                break;
            }
            case XemaEnums::BIRD_LATIN_NAME: {
                int index = headerSections.indexOf("name_lat");
                if (index < 0) {
                    index = headerSections.indexOf("latin");
                }
                sections.insert(XemaEnums::BIRD_LATIN_NAME, index);
                break;
            }
            case XemaEnums::BIRD_ABBREV: {
                int index = headerSections.indexOf("abbrev_lat");
                if (index < 0) {
                    index = headerSections.indexOf("abbrev");
                }
                sections.insert(XemaEnums::BIRD_ABBREV, index);
                break;
            }
            case XemaEnums::BIRD_CATEGORY: {
                int index = headerSections.indexOf("category");
                sections.insert(XemaEnums::BIRD_CATEGORY, index);
                break;
            }
            case XemaEnums::BIRD_ENG_NAME: {
                int index = headerSections.indexOf("name_en");
                if (index < 0) {
                    index = headerSections.indexOf("species");
                }
                sections.insert(XemaEnums::BIRD_ENG_NAME, index);
                break;
            }
            case XemaEnums::BIRD_SWE_GROUP: {
                int index = headerSections.indexOf("group_sv");
                if (index < 0) {
                    index = headerSections.indexOf("grupp_sv");
                }
                sections.insert(XemaEnums::BIRD_SWE_GROUP, index);
                break;
            }
        }
    }

    return sections;
}

QMap<int, int> ModelDataWriter::getLocationSectionNumbers(const QString &headerLine, const QString &delimiter) {
    QMap<int, int> sections;
    //qDebug() << Q_FUNC_INFO << "header" << headerLine;
    QStringList headerSections = headerLine.toLower().split(delimiter);
    //qDebug() << Q_FUNC_INFO << "headerSections" << headerSections;
    for (int i = XemaEnums::LOCATION_TOWN; i <= XemaEnums::LOCATION_ENGCOUNTRY; i++) {
        switch (i) {
            case XemaEnums::LOCATION_TOWN: {
                int index = headerSections.indexOf("town_fi");
                if (index < 0) {
                    index = headerSections.indexOf("kunta");
                }
                sections.insert(XemaEnums::LOCATION_TOWN, index);
                break;
            }
            case XemaEnums::LOCATION_PLACE: {
                int index = headerSections.indexOf("place_fi");
                if (index < 0) {
                    index = headerSections.indexOf("paikka");
                }
                sections.insert(XemaEnums::LOCATION_PLACE, index);
                break;
            }
            case XemaEnums::LOCATION_WGS: {
                int index = headerSections.indexOf("wgs");
                sections.insert(XemaEnums::LOCATION_WGS, index);
                break;
            }
            case XemaEnums::LOCATION_YKJ: {
                int index = headerSections.indexOf("ykj");
                sections.insert(XemaEnums::LOCATION_YKJ, index);
                break;
            }
            case XemaEnums::LOCATION_SWETOWN: {
                int index = headerSections.indexOf("town_sv");
                if (index < 0) {
                    index = headerSections.indexOf("ort_sv");
                }
                sections.insert(XemaEnums::LOCATION_SWETOWN, index);
                break;
            }
            case XemaEnums::LOCATION_SWEPLACE: {
                int index = headerSections.indexOf("place_sv");
                if (index < 0) {
                    index = headerSections.indexOf("plats_sv");
                }
                sections.insert(XemaEnums::LOCATION_SWEPLACE, index);
                break;
            }
            case XemaEnums::LOCATION_ENGTOWN: {
                int index = headerSections.indexOf("town_en");
                sections.insert(XemaEnums::LOCATION_ENGTOWN, index);
                break;
            }
            case XemaEnums::LOCATION_ENGPLACE: {
                int index = headerSections.indexOf("place_en");
                sections.insert(XemaEnums::LOCATION_ENGPLACE, index);
                break;
            }
            case XemaEnums::LOCATION_COUNTRY: {
                int index = headerSections.indexOf("country_fi");
                sections.insert(XemaEnums::LOCATION_COUNTRY, index);
                break;
            }
            case XemaEnums::LOCATION_SWECOUNTRY: {
                int index = headerSections.indexOf("country_sv");
                sections.insert(XemaEnums::LOCATION_SWECOUNTRY, index);
                break;
            }
            case XemaEnums::LOCATION_ENGCOUNTRY: {
                int index = headerSections.indexOf("country_en");
                sections.insert(XemaEnums::LOCATION_ENGCOUNTRY, index);
                break;
            }
        }
    }

    return sections;
}

QMap<int, int> ModelDataWriter::getPersonSectionNumbers(const QString &headerLine, const QString &delimiter) {
    QMap<int, int> sections;
    //qDebug() << Q_FUNC_INFO << "header" << headerLine;
    QStringList headerSections = headerLine.toLower().split(delimiter);
    //qDebug() << Q_FUNC_INFO << "headerSections" << headerSections;
    for (int i = XemaEnums::PERSON_FIRSTNAME; i <= XemaEnums::PERSON_DEFAULT; i++) {
        switch (i) {
            case XemaEnums::PERSON_FIRSTNAME: {
                int index = headerSections.indexOf("firstname");
                sections.insert(XemaEnums::PERSON_FIRSTNAME, index);
                break;
            }
            case XemaEnums::PERSON_SURNAME: {
                int index = headerSections.indexOf("lastname");
                if (index < 0) {
                    index = headerSections.indexOf("surname");
                }
                sections.insert(XemaEnums::PERSON_SURNAME, index);
                break;
            }
            case XemaEnums::PERSON_REGISTERED: {
                int index = headerSections.indexOf("registered");
                sections.insert(XemaEnums::PERSON_REGISTERED, index);
                break;
            }
            case XemaEnums::PERSON_DEFAULT: {
                int index = headerSections.indexOf("default");
                sections.insert(XemaEnums::PERSON_DEFAULT, index);
                break;
            }
        }
    }

    return sections;
}

QMap<int, int> ModelDataWriter::getStatusSectionNumbers(const QString &headerLine, const QString &delimiter) {
    QMap<int, int> sections;
    //qDebug() << Q_FUNC_INFO << "header" << headerLine;
    QStringList headerSections = headerLine.toLower().split(delimiter);
    //qDebug() << Q_FUNC_INFO << "headerSections" << headerSections;
    for (int i = XemaEnums::STATUS_FINABBREV; i <= XemaEnums::STATUS_ENGNAME; i++) {
        switch (i) {
            case XemaEnums::STATUS_FINABBREV: {
                int index = headerSections.indexOf("abbrev");
                sections.insert(XemaEnums::STATUS_FINABBREV, index);
                break;
            }
            case XemaEnums::STATUS_FINNAME: {
                int index = headerSections.indexOf("status_fi");
                if (index < 0) {
                    index = headerSections.indexOf("tila");
                }
                sections.insert(XemaEnums::STATUS_FINNAME, index);
                break;
            }
            case XemaEnums::STATUS_SWENAME: {
                int index = headerSections.indexOf("status_sv");
                sections.insert(XemaEnums::STATUS_SWENAME, index);
                break;
            }
            case XemaEnums::STATUS_ENGNAME: {
                int index = headerSections.indexOf("status_en");
                sections.insert(XemaEnums::STATUS_ENGNAME, index);
                break;
            }
        }
    }

    return sections;
}
