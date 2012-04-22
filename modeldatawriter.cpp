#include <QFile>
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

ModelDataWriter::ModelDataWriter(QObject *parent) :
    QObject(parent)
{
    mCoordinates = new CoordinateConverter(this);
}

void ModelDataWriter::writeNewObservation(const QString &data, const LocationModel &locations, const PersonModel &persons)
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
    if (QFile::exists(dataFileDir() + "lokkitesti.txt") == false)
//    if (QFile::exists(dataFileDir() + "xemadata.txt") == false)
    {
        headerExists = false;
    }
//    QFile tiedosto(dataFileDir() + "xemadata.txt");
    QFile tiedosto(dataFileDir() + "lokkitesti.txt");
    tiedosto.open(QFile::ReadWrite|QFile::Append);
    QTextStream striimi(&tiedosto);
    striimi.setCodec("ISO 8859-1");
    if (headerExists == false)
    {
//        QString header = QString::fromUtf8("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Kunta#Paikka#X-koord#Y-koord#Tarkkuus#X-koord-linnun#Y-koord-linnun#Tarkkuus_linnun#Paikannettu#Lisätietoja#Atlaskoodi#Tallentaja#Tallennusaika#Havainnoijat#Salattu#Koontihavainto#Kuuluu havaintoon#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Epäsuora havainto#Sää#Exported#\n");
        QString header = QString::fromUtf8("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Paikka#Lisätietoja#Atlaskoodi#Havainnoijat#Havainnoijat2#Salattu#Rivejä#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Sää#Exported#\n");
        striimi << header;
    }
    striimi << newData;
    striimi << "\n";
    tiedosto.close();
}

void ModelDataWriter::replaceObservation(qlonglong id, const QString &data)
{
//    qDebug() << "KORVATAAN, ID ON" << id;
    QFile tiedosto(dataFileDir() + "lokkitesti.txt");
//    QFile tiedosto(dataFileDir() + "xemadata.txt");
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
    tmptiedosto.rename(dataFileDir() + "lokkitesti.txt");
//    tmptiedosto.rename(dataFileDir() + "xemadata.txt");
    tiedosto.remove();
}

QString ModelDataWriter::loadObservation(qlonglong id)
{
//    qDebug() << "LUETAAN" << id;
    if (QFile::exists(dataFileDir() + "lokkitesti.txt") == false)
//    if (QFile::exists(dataFileDir() + "xemadata.txt") == false)
    {
        return QString();
    }
//    QFile tiedosto(dataFileDir() + "xemadata.txt");
    QFile tiedosto(dataFileDir() + "lokkitesti.txt");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    QString obsLine;
    while (striimi.atEnd() == false)
    {
        obsLine = striimi.readLine();
        if (obsLine.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong() == id)
        {
            break;
        }
    }
    tiedosto.close();
    if (obsLine.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID) == "Id" || obsLine.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).isEmpty() == true)
    {
        return QString();
    }
    return obsLine;
}

void ModelDataWriter::deleteObservation(qlonglong id)
{
//    qDebug() << "DELETE ID" << id;
    QFile tiedosto(dataFileDir() + "lokkitesti.txt");
//    QFile tiedosto(dataFileDir() + "xemadata.txt");
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
//    tmptiedosto.rename(dataFileDir() + "xemadata.txt");
    tmptiedosto.rename(dataFileDir() + "lokkitesti.txt");
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
            ykj.append(ykjY);
        }

        if (ykj.isEmpty() == false && wgs.isEmpty() == true)
        {
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
            wgs.append(wgsY);
        }

        line.append(wgs);
        line.append(";");
        line.append(ykj);
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
    striimi << QString("\n");
    for(int i = 0; i < rows; i++)
    {
        QString line;
        line.append(model->data(model->index(i), BirdModel::IdRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::FinGroupRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::SweGroupRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::LatinGroupRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::FinNameRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::SweNameRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::AbbrevRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::LatinNameRole).toString());
        line.append(";");
        line.append(model->data(model->index(i), BirdModel::CategoryRole).toString());
        line.append(";\n");
        striimi << line;
    }
    tiedosto.close();
}

void ModelDataWriter::exportHistory(bool onlyNew, LocationModel *locations, PersonModel *persons, BirdModel *birds)
{
    QDateTime date;
    date = QDateTime::currentDateTime();
    QString fileName("xemaexport-");
    fileName.append(date.toString("yyyyMMdd-hhmmss"));
    fileName.append(".csv");
//    qDebug() << "void ModelDataWriter::exportHistory(bool onlyNew)" << onlyNew;
//    QFile tiedosto(dataFileDir() + "xemadata.txt");
    QFile tiedosto(dataFileDir() + "lokkitesti.txt");
    QFile newtiedosto(dataFileDir() + "xemadata.tmp.txt");
    QFile tmptiedosto(exportDir() + fileName);

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
    while (instriimi.atEnd() == false)
    {
        obsLine = instriimi.readLine();
        if (obsLine.section("#",XemaEnums::OBS_ID,XemaEnums::OBS_ID) == "Id")
        {
            outstriimi2 << obsLine;
            outstriimi2 << "\n";
            continue;
        }
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
            outstriimi << formatToTiira(obsLine,locations,persons,birds);
            outstriimi << "\n";
        }
        if (obsLine.length() > 20)
        {
//            qDebug() << "rivi ennen export settia" << obsLine;
            QString newLine;
            newLine = obsLine;

            QString start = newLine.mid(0, pos);
            if (start.endsWith("#") == false)
            {
                start.append("#");
            }
            start.append("true#\n");
//            qDebug() << "uus rivi export setin jalkeen" << start;
            outstriimi2 << start;
        }
    }
    tiedosto.close();
    tmptiedosto.close();
    newtiedosto.close();

    QFile::remove(dataFileDir() + "xemadata.backup");
    tiedosto.rename(dataFileDir() + "xemadata.backup");
//    newtiedosto.rename(dataFileDir() + "xemadata.txt");
    newtiedosto.rename(dataFileDir() + "lokkitesti.txt");
}

qlonglong ModelDataWriter::getNewId()
{
//    qDebug() << "LUETAAN IDT";
//    if (QFile::exists(dataFileDir() + "xemadata.txt") == false)
    if (QFile::exists(dataFileDir() + "lokkitesti.txt") == false)
    {
        return 1;
    }
//    QFile tiedosto(dataFileDir() + "xemadata.txt");
    QFile tiedosto(dataFileDir() + "lokkitesti.txt");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
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
    appPath = QString("/Users/Tero/");
#else
    appPath = QString("C:/");

#endif
    return appPath;
}

QString ModelDataWriter::exportDir()
{
    QString appPath;
#ifdef Q_OS_SYMBIAN
    appPath = QString("C:/data/");
#elif defined HARMATTAN
    appPath = QString("/home/user/MyDocs/");
#elif defined MAC_OS_X_VERSION_10_6
    appPath = QString("/Users/Tero/");
#else
    appPath = QString("C:/");

#endif
    return appPath;
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

void ModelDataWriter::removeCustomObservers()
{
    QFile tiedosto(dataFileDir() + "xemapersondata.txt");
    tiedosto.remove();
}

QString ModelDataWriter::formatToTiira(const QString &data, LocationModel *locations, PersonModel *persons, BirdModel *birds)
{
    for(int i = 0; i < data.count("#"); i++)
    {
        qDebug() << "kentta" << i << "arvo" << data.section("#",i,i);
    }
    QString id = data.section("#",XemaEnums::OBS_ID, XemaEnums::OBS_ID);
    QString species = data.section("#",XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES);
    QString datetime = data.section("#",XemaEnums::OBS_DATE1,XemaEnums::OBS_TIME2);
    QString location = data.section("#", XemaEnums::OBS_PLACE, XemaEnums::OBS_PLACE);
    QString vali = data.section("#",XemaEnums::OBS_INFO,XemaEnums::OBS_ATLAS);
    QString nimet = data.section("#",XemaEnums::OBS_REGPERSON, XemaEnums::OBS_OTHERPERSON);
    QString hidden = data.section("#",XemaEnums::OBS_HIDDEN, XemaEnums::OBS_HIDDEN);
    int xemaRows = data.section("#",XemaEnums::OBS_ROWCOUNT,XemaEnums::OBS_ROWCOUNT).toInt();
    QString town = location.section(",", 0, 0);
    QString place = location.section(", ", 1);
    QString toka = "#" + town + "#" + place;
    QString eka = id;

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
    eka += species;
    eka += "#";
    if (datetime.section("#",3,3).isEmpty() == true)
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
    rowCount = locations->rowCount();

    // add location coordinates if found
    bool locationAdded = false;
    for(int i = 0; i < rowCount; i++)
    {
        if (locations->getItem(i).town() == town && locations->getItem(i).place() == place)
        {
            QString coordinate = locations->getItem(i).wgsCoordinate();
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
    eka += QString("######");
    eka += vali;
    eka += "#";

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
    }
    eka += "#";
    // tiira saving time
    eka += "#";

//    qDebug() << "NIMET ennen muutosta" << nimet;
    if (nimet.length() > 1 && nimet.endsWith('#') == false) {
        nimet.replace("#", ", ");
    }
    else {
        nimet.replace("#", "");
    }
    eka += nimet;
    eka += "#";
    eka += hidden;
    eka += "#";

    // koontihavainto ja kuuluu havaintoon
    eka += "#";
    eka += "#";

    // include exported or not (24 vs 25)
    QString loppu = data.section("#", XemaEnums::OBS_WEATHER+((xemaRows-1)*XemaEnums::OBS_SUBFIELDCOUNT),
                                 XemaEnums::OBS_WEATHER+((xemaRows-1)*XemaEnums::OBS_SUBFIELDCOUNT));
    loppu.prepend("#");
    loppu.append("#");

    QString firstRow = data.section("#", XemaEnums::OBS_BIRDCOUNT, XemaEnums::OBS_NEST);
    firstRow.replace("#koiras#", "#k#");
    firstRow.replace("#naaras#", "#n#");
    firstRow.replace("#pariutuneet#", "#p#");
    eka += firstRow;
    eka += loppu;

    if (xemaRows > 1) {
        for(int i = 1; i < xemaRows; i++)
        {
            eka += "\n";
            eka += data.section("#", 0, 0);
            eka += "#######################";
            QString rivi = data.section("#", XemaEnums::OBS_BIRDCOUNT+(i*XemaEnums::OBS_SUBFIELDCOUNT), XemaEnums::OBS_NEST+(i*XemaEnums::OBS_SUBFIELDCOUNT));
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

void ModelDataWriter::importHistory(LocationModel *locations,  PersonModel *persons,  BirdModel *birds)
{
    qDebug() << "IMPORT";
    QFile importfile(dataFileDir() + "xemadata_import.txt");
    importfile.open(QFile::ReadOnly);
    QTextStream importstream(&importfile);
//    ("Id#Laji#Pvm1#Pvm2#Kello_hav_1#Kello_hav_2#Paikka#Lisätietoja#Atlaskoodi#Havainnoijat#Havainnoijat2#Salattu#Rivejä#Määrä#Kello_lintu_1#Kello_lintu_2#Sukupuoli#Puku#Ikä#Tila#Lisätietoja_2#Parvi#Bongattu#Pesintä#Sää#Exported#\n");

    if (importstream.atEnd() == false)
    {
        if (importstream.readLine().section("#", XemaEnums::TIIRA_ID, XemaEnums::TIIRA_ID) != "Id")
        {
            importstream.reset();
        }
    }
    QStringList prevLines;
    QString currentLine;
    while (importstream.atEnd() == false)
    {
        if (prevLines.length() == 0)
        {
            prevLines.append(importstream.readLine());
            qDebug() << "IMPORT eka line" << prevLines;
            continue;
        }
        currentLine = importstream.readLine();
        if (prevLines.last().section("#", XemaEnums::TIIRA_ID, XemaEnums::TIIRA_ID) == currentLine.section("#", XemaEnums::TIIRA_ID, XemaEnums::TIIRA_ID))
        {
            qDebug() << "Sama id"<< currentLine;
            prevLines.append(currentLine);
            continue;
        }
        else
        {
            qDebug() << "IMPORT eri id, kasittele edelliset nyt" << currentLine;
            importLine(prevLines, locations, persons, birds);
        }
    }
    prevLines.clear();
    prevLines.append(currentLine);
    importLine(prevLines, locations, persons, birds);
}

void ModelDataWriter::importLine(const QStringList &lines, LocationModel *locations, PersonModel *persons, BirdModel *birds)
{
    if (lines.size() < 1)
    {
        return;
    }
    qDebug() << "IMPORT lines" << lines;
    QString rowCount;
    rowCount.setNum(lines.size());
    QString readyLine;
    readyLine += lines.at(0).section("#", XemaEnums::TIIRA_ID, XemaEnums::TIIRA_ID);
    readyLine.append("#");
    QString speciesAbbrev = lines.at(0).section("#",XemaEnums::TIIRA_SPECIES_ABBR, XemaEnums::TIIRA_SPECIES_ABBR);
    for(int i = 0; i < birds->rowCount(); i++)
    {
        if (birds->getItem(i).abbreviation() == speciesAbbrev)
        {
            readyLine += birds->getItem(i).finName();
            break;
        }
    }
    readyLine.append("#");
    readyLine += lines.at(0).section("#", XemaEnums::TIIRA_DATE1, XemaEnums::TIIRA_TIME2);
    readyLine.append("#");
    QString town;
    town += lines.at(0).section("#", XemaEnums::TIIRA_TOWN, XemaEnums::TIIRA_TOWN);
    QString place;
    place += lines.at(0).section("#", XemaEnums::TIIRA_LOCATION, XemaEnums::TIIRA_LOCATION);
    readyLine += town;
    readyLine.append(", ");
    readyLine += place;
    readyLine.append("#");

    readyLine += lines.at(0).section("#", XemaEnums::TIIRA_INFO, XemaEnums::TIIRA_ATLAS);
    readyLine.append("#");

    QString saver = lines.at(0).section("#", XemaEnums::TIIRA_SAVER, XemaEnums::TIIRA_SAVER);
    QString observers = lines.at(0).section("#", XemaEnums::TIIRA_PERSONS, XemaEnums::TIIRA_PERSONS);
    observers.remove(saver);
    qDebug() << "OBS:" <<observers;
    QStringList observerList = observers.split(",");
    bool saverRegistered = false;
    QString registeredPeople;
    QString otherPeople;

    for(int i = 0; i < persons->rowCount(); i++)
    {
        if (persons->getItem(i).name() == saver)
        {
            saverRegistered = true;
            break;
        }
    }

    if(saverRegistered)
    {
        registeredPeople += saver;
        registeredPeople.append(",");
    }
    else
    {
        otherPeople += saver;
        otherPeople.append(",");
    }

    for(int i = 0;i < observerList.length(); i++)
    {
        bool found = false;
        for(int j = 0; j < persons->rowCount(); j++)
        {
            qDebug() << "verrataan" << persons->getItem(j).name() << observerList.at(i).trimmed();
            if (persons->getItem(j).registered() && persons->getItem(j).name() == observerList.at(i).trimmed())
            {
                registeredPeople += observerList.at(i).trimmed();
                registeredPeople.append(",");
                found = true;
                break;
            }
        }
        if(!found)
        {
            otherPeople += observerList.at(i).trimmed();
            otherPeople.append(",");
        }
    }

    if (registeredPeople.endsWith(","))
    {
        registeredPeople = registeredPeople.left(registeredPeople.length()-1);
    }
    if (otherPeople.endsWith(","))
    {
        otherPeople = otherPeople.left(otherPeople.length()-1);
    }
    if (registeredPeople.startsWith(","))
    {
        registeredPeople.remove(0, 1);
    }
    if (otherPeople.startsWith(","))
    {
        otherPeople.remove(0, 1);
    }

    readyLine += registeredPeople;
    readyLine.append("#");
    readyLine += otherPeople;

    // hidden
    readyLine.append("#");
    if(QString::compare(lines.at(0).section("#", XemaEnums::TIIRA_HIDDEN, XemaEnums::TIIRA_HIDDEN),"X", Qt::CaseInsensitive)==0)
    {
        readyLine.append("true#");
    }
    else
    {
        readyLine.append("false#");
    }
    // koontihavainto, kuuluu havaintoon 21,22
    readyLine += rowCount;
    readyLine.append("#");

    QString weather = lines.at(0).section("#",XemaEnums::TIIRA_INDIRECT); // WEATHER ON INDIRECTIN PAIKALLA
    for(int i = 0;i < lines.length();i++)
    {
        QString row;
        row = lines.at(i).section("#", XemaEnums::TIIRA_BIRDCOUNT, XemaEnums::TIIRA_NEST);
        qDebug() << "alarivi" << row;
        readyLine += row.section("#",0,2);
        if (row.section("#",3,3) == "k")
        {
            readyLine.append("#koiras#");
        }
        else if (row.section("#",3,3) == "n")
        {
            readyLine.append("#naaras#");
        }
        else
        {
            readyLine.append("#");
            readyLine += row.section("#",3,3);
            readyLine.append("#");
        }
        readyLine += row.section("#",4,8);
        readyLine.append("#");
        if(QString::compare(row.section("#",9,9),"X", Qt::CaseInsensitive)==0)
        {
            readyLine.append("true#");
        }
        else
        {
            readyLine.append("false#");
        }
        if(QString::compare(row.section("#",10,10),"X", Qt::CaseInsensitive)==0)
        {
            readyLine.append("true#");
        }
        else
        {
            readyLine.append("false#");
        }
    }
    readyLine.append(weather);
    // exported to true
    readyLine.append("#true#");

    qDebug() << "VALMIS RIVI" << readyLine;

    // tarkista onko id jo käytössä, jos niin luo uusi
    // kirjoita rivi tiedostoon

}
