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
    striimi << QString("Kunta;Paikka;wgs;ykj;kunta_swe;paikka_swe;kunta_eng;paikka_eng;muokattu\n");
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
        line.append(";");
        line.append(model->getItem(i).sweTown(true));
        line.append(";");
        line.append(model->getItem(i).swePlace(true));
        line.append(";");
        line.append(model->getItem(i).engTown(true));
        line.append(";");
        line.append(model->getItem(i).engPlace(true));
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

void ModelDataWriter::exportHistory(bool onlyNew, LocationModel *locations, PersonModel *persons, BirdModel *birds)
{
    QDateTime date;
    date = QDateTime::currentDateTime();
    QString fileName("xema_exported_history-");
    fileName.append(date.toString("yyyyMMdd-hhmmss"));
    fileName.append(".csv");
//    qDebug() << "void ModelDataWriter::exportHistory(bool onlyNew)" << onlyNew;
    QFile tiedosto(dataFileDir() + "xemadata.txt");
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
//            qDebug() << "VALMIS EXPORT RIVI" << formatToTiira(obsLine,locations,persons,birds);
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
    newtiedosto.rename(dataFileDir() + "xemadata.txt");
}

void ModelDataWriter::exportOwnData() {
//    qDebug() << "exportOwnData";

    // LOCATIONS
    QFile ownlocationfile(dataFileDir() + "xemalocationdata.txt");
    ownlocationfile.open(QFile::ReadOnly);
    QTextStream instream1(&ownlocationfile);
    instream1.setCodec("ISO 8859-1");

    QFile origlocationfile(":defaultlocations.csv");
    origlocationfile.open(QFile::ReadOnly);
    QTextStream instream2(&origlocationfile);
    instream2.setCodec("ISO 8859-1");

    QFile exportfile(exportDir() + "xema_exported_locations.txt");

    exportfile.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream outstriimi(&exportfile);
    outstriimi.setCodec("ISO 8859-1");

    while (instream1.atEnd() == false)
    {
        QString ownLocationLine;
        ownLocationLine = instream1.readLine();
        bool matchFound = false;
        instream2.seek(0);
        while (instream2.atEnd() == false)
        {
            QString origLocationLine;
            origLocationLine = instream2.readLine();
            if (ownLocationLine == origLocationLine) {
                matchFound = true;
                break;
            }
        }
        if (matchFound == false) {
            outstriimi << ownLocationLine;
            outstriimi << "\n";
        }
    }

    // PERSONS

    QFile ownpersonfile(dataFileDir() + "xemapersondata.txt");
    ownpersonfile.open(QFile::ReadOnly);
    QTextStream personinstream1(&ownpersonfile);
    personinstream1.setCodec("ISO 8859-1");

    QFile origpersonfile(":defaultpersons.csv");
    origpersonfile.open(QFile::ReadOnly);
    QTextStream personinstream2(&origpersonfile);
    personinstream2.setCodec("ISO 8859-1");

    QFile exportfile2(exportDir() + "xema_exported_persons.txt");

    exportfile2.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream outstriimi2(&exportfile2);
    outstriimi2.setCodec("ISO 8859-1");

    while (personinstream1.atEnd() == false)
    {
        QString ownPersonLine;
        ownPersonLine = personinstream1.readLine();
        bool matchFound = false;
        personinstream2.seek(0);
        while (personinstream2.atEnd() == false)
        {
            QString origPersonLine;
            origPersonLine = personinstream2.readLine();
            if (ownPersonLine == origPersonLine) {
                matchFound = true;
                break;
            }
        }
        if (matchFound == false) {
            outstriimi2 << ownPersonLine;
            outstriimi2 << "\n";
        }
    }

    // BIRDS

    QFile ownbirdfile(dataFileDir() + "xemabirddata.txt");
    ownbirdfile.open(QFile::ReadOnly);
    QTextStream birdinstream1(&ownbirdfile);
    birdinstream1.setCodec("ISO 8859-1");

    QFile origbirdfile(":specieslist.csv");
    origbirdfile.open(QFile::ReadOnly);
    QTextStream birdinstream2(&origbirdfile);
    birdinstream2.setCodec("ISO 8859-1");

    QFile exportfile3(exportDir() + "xema_exported_birds.txt");

    exportfile3.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream outstriimi3(&exportfile3);
    outstriimi3.setCodec("ISO 8859-1");

    while (birdinstream1.atEnd() == false)
    {
        QString ownBirdLine;
        ownBirdLine = birdinstream1.readLine();
        if (ownBirdLine.isEmpty()) {
            continue;
        }
        bool matchFound = false;
        birdinstream2.seek(0);
        while (birdinstream2.atEnd() == false)
        {
            QString origBirdLine;
            origBirdLine = birdinstream2.readLine();
            if (ownBirdLine == origBirdLine) {
                matchFound = true;
                break;
            }
        }
        if (matchFound == false) {
            outstriimi3 << ownBirdLine;
            outstriimi3 << "\n";
        }
    }

    // STATUSES

    QFile ownstatusfile(dataFileDir() + "xemastatusdata.txt");
    ownstatusfile.open(QFile::ReadOnly);
    QTextStream statusinstream1(&ownstatusfile);
    statusinstream1.setCodec("ISO 8859-1");

    QFile origstatusfile(":defaultstatuses.csv");
    origstatusfile.open(QFile::ReadOnly);
    QTextStream statusinstream2(&origstatusfile);
    statusinstream2.setCodec("ISO 8859-1");

    QFile exportfile4(exportDir() + "xema_exported_statuses.txt");

    exportfile4.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream outstriimi4(&exportfile4);
    outstriimi4.setCodec("ISO 8859-1");

    while (statusinstream1.atEnd() == false)
    {
        QString ownStatusLine;
        ownStatusLine = statusinstream1.readLine();
        if (ownStatusLine.isEmpty()) {
            continue;
        }
        bool matchFound = false;
        statusinstream2.seek(0);
        while (statusinstream2.atEnd() == false)
        {
            QString origBirdLine;
            origBirdLine = statusinstream2.readLine();
            if (ownStatusLine == origBirdLine) {
                matchFound = true;
                break;
            }
        }
        if (matchFound == false) {
            outstriimi4 << ownStatusLine;
            outstriimi4 << "\n";
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

QString ModelDataWriter::formatToTiira(const QString &data, LocationModel *locations, PersonModel *persons, BirdModel *birds)
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
//    qDebug() << "EXPORT, eka 7" << eka;
    eka += QString("######");
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
    return eka;
}

int ModelDataWriter::importHistory(LocationModel *locations,  PersonModel *persons)
{
    int importError = XemaEnums::IMPORT_NOERRORS;
//    qDebug() << "IMPORT";
    QDir dir;
    dir.cd(importDir());
    QStringList filters;
    filters << "*history*.csv" << "*history*.txt";
    QStringList importFiles;
    importFiles = dir.entryList(filters,QDir::Files);
//    qDebug() << "IMPORT" << importFiles;
    for( int fileno = 0; fileno < importFiles.length(); fileno++ ) {
        QFile importfile(importDir() + importFiles.at(fileno));
        importfile.open(QFile::ReadOnly);
        QTextStream importstream(&importfile);
        importstream.setCodec("ISO 8859-1");

        QString delimiter("#");

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
                line.section(delimiter, XemaEnums::TIIRA_ID, XemaEnums::TIIRA_ID) != "Havainto id" )
            {
                importstream.seek(0);
            }
        }
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
                importLine(prevLines, locations, persons, delimiter);
                prevLines.clear();
                prevLines.append(currentLine);
                previousNotHandled = false;
            }
        }

        importLine(prevLines, locations, persons, delimiter);

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

void ModelDataWriter::importLine(const QStringList &lines, LocationModel *locations, PersonModel *persons, const QString &delimiter)
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
    QString speciesAbbrev = readLine.section(delimiter,XemaEnums::TIIRA_SPECIES_ABBR, XemaEnums::TIIRA_SPECIES_ABBR);
    readyLine.append(speciesAbbrev);

    QString date1 = readLine.section(delimiter, XemaEnums::TIIRA_DATE1, XemaEnums::TIIRA_DATE1);
    QString date2 = readLine.section(delimiter, XemaEnums::TIIRA_DATE2, XemaEnums::TIIRA_DATE2);

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
    readyLine += readLine.section(delimiter, XemaEnums::TIIRA_TIME1, XemaEnums::TIIRA_TIME1);
    readyLine.append("#");
    readyLine += readLine.section(delimiter, XemaEnums::TIIRA_TIME2, XemaEnums::TIIRA_TIME2);
    readyLine.append("#");

    QString town = readLine.section(delimiter, XemaEnums::TIIRA_TOWN, XemaEnums::TIIRA_TOWN);
    if (town.isEmpty()) {
        faultyLine++;
        int firstMark = readLine.indexOf(delimiter);
        readLine.remove(firstMark,1);
        town = readLine.section(delimiter, XemaEnums::TIIRA_TOWN, XemaEnums::TIIRA_TOWN);
    }
    readyLine += town;
    readyLine.append("#");

    QString location = readLine.section(delimiter, XemaEnums::TIIRA_LOCATION, XemaEnums::TIIRA_LOCATION);
    readyLine += location;
    readyLine.append("#");

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
            x = readLine.section(delimiter, XemaEnums::TIIRA_XCOORD, XemaEnums::TIIRA_XCOORD);
            y = readLine.section(delimiter, XemaEnums::TIIRA_YCOORD, XemaEnums::TIIRA_YCOORD);
            ykj.append(x);
            ykj.append(":");
            ykj.append(y);
            if (x.isEmpty() == false && y.isEmpty() == false)
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
            }

            // TODO add coordinates
            tmp.setYKJCoordinate(ykj);
            tmp.setWGSCoordinate(wgs);
            tmp.setCustom(true);
            locations->addItem(tmp);
        }
    }

    readyLine += readLine.section(delimiter, XemaEnums::TIIRA_XCOORD, XemaEnums::TIIRA_XCOORD);
    readyLine.append("#");

    readyLine += readLine.section(delimiter, XemaEnums::TIIRA_YCOORD, XemaEnums::TIIRA_YCOORD);
    readyLine.append("#");

    readyLine += readLine.section(delimiter, XemaEnums::TIIRA_ACCURACY, XemaEnums::TIIRA_ACCURACY);
    readyLine.append("#");

    readyLine += readLine.section(delimiter, XemaEnums::TIIRA_BIRD_XCOORD, XemaEnums::TIIRA_BIRD_XCOORD);
    readyLine.append("#");

    readyLine += readLine.section(delimiter, XemaEnums::TIIRA_BIRD_YCOORD, XemaEnums::TIIRA_BIRD_YCOORD);
    readyLine.append("#");

    readyLine += readLine.section(delimiter, XemaEnums::TIIRA_BIRD_ACCURACY, XemaEnums::TIIRA_BIRD_ACCURACY);
    readyLine.append("#");

    readyLine += readLine.section(delimiter, XemaEnums::TIIRA_PAIKANNETTU, XemaEnums::TIIRA_PAIKANNETTU);
    readyLine.append("#");

    readyLine += readLine.section(delimiter, XemaEnums::TIIRA_INFO, XemaEnums::TIIRA_INFO);
    readyLine.append("#");

    readyLine += readLine.section(delimiter, XemaEnums::TIIRA_ATLAS, XemaEnums::TIIRA_ATLAS);
    readyLine.append("#");

    QString saver = readLine.section(delimiter, XemaEnums::TIIRA_SAVER, XemaEnums::TIIRA_SAVER);
    readyLine += saver;
    readyLine.append("#");

    readyLine += readLine.section(delimiter, XemaEnums::TIIRA_SAVETIME, XemaEnums::TIIRA_SAVETIME);
    readyLine.append("#");

    QString observers = readLine.section(delimiter, XemaEnums::TIIRA_PERSONS, XemaEnums::TIIRA_PERSONS);
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
    if(QString::compare(readLine.section(delimiter, XemaEnums::TIIRA_HIDDEN, XemaEnums::TIIRA_HIDDEN),"X", Qt::CaseInsensitive)==0)
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

    QString weather = readLine.section(delimiter,XemaEnums::TIIRA_EXTRA1,XemaEnums::TIIRA_EXTRA1); // WEATHER ON INDIRECTIN PAIKALLA
    for(int i = 0;i < lines.length();i++)
    {
        QString row;
        row = lines.at(i).section(delimiter, XemaEnums::TIIRA_BIRDCOUNT, XemaEnums::TIIRA_INDIRECT);
        for ( int faults = faultyLine; faults > 0; faults--) {
            int firstMark = row.indexOf(delimiter);
            row.remove(firstMark,1);
        }

//        qDebug() << "alarivi" << row;
        readyLine += row.section(delimiter,0,0);
        readyLine.append("#");
        readyLine += row.section(delimiter,1,1);
        readyLine.append("#");
        readyLine += row.section(delimiter,2,2);
        if (row.section(delimiter,3,3) == "k")
        {
            readyLine.append("#koiras#");
        }
        else if (row.section(delimiter,3,3) == "n")
        {
            readyLine.append("#naaras#");
        }
        else if (row.section(delimiter,3,3) == "p")
        {
            readyLine.append("#pariutuneet#");
        }
        else
        {
            readyLine.append("#");
            readyLine += row.section(delimiter,3,3);
            readyLine.append("#");
        }
        readyLine += row.section(delimiter,4,4);
        readyLine.append("#");
        readyLine += row.section(delimiter,5,5);
        readyLine.append("#");
        readyLine += row.section(delimiter,6,6);
        readyLine.append("#");
        readyLine += row.section(delimiter,7,7);
        readyLine.append("#");
        readyLine += row.section(delimiter,8,8);
        readyLine.append("#");
        if(QString::compare(row.section(delimiter,9,9),"X", Qt::CaseInsensitive)==0)
        {
            readyLine.append("true#");
        }
        else
        {
            readyLine.append("false#");
        }
        if(QString::compare(row.section(delimiter,10,10),"X", Qt::CaseInsensitive)==0)
        {
            readyLine.append("true#");
        }
        else
        {
            readyLine.append("false#");
        }

        readyLine += row.section(delimiter,11,11);
        readyLine.append("#");
    }
    readyLine.append(weather);
    // exported to true
    readyLine.append("#true#");

//    qDebug() << "VALMIS RIVI" << readyLine;

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

        if (importstream.atEnd() == false) {
            QString line = importstream.readLine();
            if (line.count(delimiter) != 4 && line.count(delimiter) != 8) {
//                qDebug() << "SETTING DELIMITER TO ;" << line.count(";");
                delimiter = ";";
            }
            if (line.count(delimiter) != 4 && line.count(delimiter) != 8) {
//                qDebug() << "INVALID LOCATION FILE";
                if (!(importError&XemaEnums::IMPORT_LOCATIONERROR)) {
                    importError += XemaEnums::IMPORT_LOCATIONERROR;
                }
                continue;
            }
            importstream.seek(0);
        }
        while (importstream.atEnd() == false) {
            QString locationLine;
            locationLine = importstream.readLine();
            Location location(locationLine.section(';', XemaEnums::LOCATION_TOWN, XemaEnums::LOCATION_TOWN),
                              locationLine.section(';', XemaEnums::LOCATION_PLACE, XemaEnums::LOCATION_PLACE),
                              locationLine.section(';', XemaEnums::LOCATION_WGS, XemaEnums::LOCATION_WGS),
                              locationLine.section(';', XemaEnums::LOCATION_YKJ, XemaEnums::LOCATION_YKJ));
            location.setSweTown(locationLine.section(';', XemaEnums::LOCATION_SWETOWN, XemaEnums::LOCATION_SWETOWN));
            location.setEngTown(locationLine.section(';', XemaEnums::LOCATION_ENGTOWN, XemaEnums::LOCATION_ENGTOWN));
            location.setSwePlace(locationLine.section(';', XemaEnums::LOCATION_SWEPLACE, XemaEnums::LOCATION_SWEPLACE));
            location.setEngPlace(locationLine.section(';', XemaEnums::LOCATION_ENGPLACE, XemaEnums::LOCATION_ENGPLACE));
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
    filters << "*person*.csv" << "*person*.txt" << "*people*.csv" << "*people*.txt";
    importFiles.clear();

    importFiles = dir.entryList(filters,QDir::Files);
//    qDebug() << "IMPORT PEOPLE" << importFiles;
    for( int fileno = 0; fileno < importFiles.length(); fileno++ ) {
        QFile importfile(importDir() + importFiles.at(fileno));
        importfile.open(QFile::ReadOnly);
        QTextStream importstream(&importfile);
        importstream.setCodec("ISO 8859-1");

        QString delimiter("#");

        if (importstream.atEnd() == false) {
            QString line = importstream.readLine();
            if (line.count(delimiter) != 4) {
//                qDebug() << "SETTING DELIMITER TO ;" << line.count(";");
                delimiter = ";";
            }
            if (line.count(delimiter) != 4) {
//                qDebug() << "INVALID PERSON FILE";
                if (!(importError&XemaEnums::IMPORT_PERSONERROR)) {
                    importError += XemaEnums::IMPORT_PERSONERROR;
                }
                continue;
            }
            importstream.seek(0);
        }
        while (importstream.atEnd() == false) {
            QString importLine;
            importLine = importstream.readLine();
            bool registered = false;
            if (QString::compare(importLine.section(';', XemaEnums::PERSON_REGISTERED, XemaEnums::PERSON_REGISTERED), "true", Qt::CaseInsensitive) == 0) {
                registered = true;
            }
            Person person(importLine.section(';', XemaEnums::PERSON_FIRSTNAME, XemaEnums::PERSON_FIRSTNAME),
                              importLine.section(';', XemaEnums::PERSON_SURNAME, XemaEnums::PERSON_SURNAME),
                              registered, false );
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

        if (importstream.atEnd() == false) {
            QString line = importstream.readLine();
            if (line.count(delimiter) != 9 && line.count(delimiter) != 11) {
//                qDebug() << "SETTING DELIMITER TO ;" << line.count(";");
                delimiter = ";";
            }
            if (line.count(delimiter) != 9 && line.count(delimiter) != 11) {
//                qDebug() << "INVALID BIRD FILE";
                if (!(importError&XemaEnums::IMPORT_BIRDERROR)) {
                    importError += XemaEnums::IMPORT_BIRDERROR;
                }
                continue;
            }
            importstream.seek(0);
        }
        while (importstream.atEnd() == false) {
            QString importLine;
            importLine = importstream.readLine();

            Bird bird(importLine.section(';', XemaEnums::BIRD_ID, XemaEnums::BIRD_ID).toInt(),
                       importLine.section(';', XemaEnums::BIRD_FIN_GROUP, XemaEnums::BIRD_FIN_GROUP),
                       importLine.section(';', XemaEnums::BIRD_SWE_GROUP, XemaEnums::BIRD_SWE_GROUP),
                       importLine.section(';', XemaEnums::BIRD_LATIN_GROUP, XemaEnums::BIRD_LATIN_GROUP),
                       importLine.section(';', XemaEnums::BIRD_FIN_NAME, XemaEnums::BIRD_FIN_NAME),
                       importLine.section(';', XemaEnums::BIRD_SWE_NAME, XemaEnums::BIRD_SWE_NAME),
                       importLine.section(';', XemaEnums::BIRD_ABBREV, XemaEnums::BIRD_ABBREV),
                       importLine.section(';', XemaEnums::BIRD_LATIN_NAME, XemaEnums::BIRD_LATIN_NAME),
                       importLine.section(';', XemaEnums::BIRD_CATEGORY, XemaEnums::BIRD_CATEGORY));
            // TODO LOC
            bird.setEngName(importLine.section(';', XemaEnums::BIRD_ENG_NAME, XemaEnums::BIRD_ENG_NAME));
            bird.setEngGroup(importLine.section(';', XemaEnums::BIRD_ENG_GROUP, XemaEnums::BIRD_ENG_GROUP));
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

        if (importstream.atEnd() == false) {
            QString line = importstream.readLine();
            if (line.count(delimiter) != 4) {
//                qDebug() << "SETTING DELIMITER TO ;" << line.count(";");
                delimiter = ";";
            }
            if (line.count(delimiter) != 4) {
//                qDebug() << "INVALID STATUS FILE";
                if (!(importError&XemaEnums::IMPORT_STATUSERROR)) {
                    importError += XemaEnums::IMPORT_STATUSERROR;
                }
                continue;
            }
            importstream.seek(0);
        }
        while (importstream.atEnd() == false) {
            QString importLine;
            importLine = importstream.readLine();

            Status status(importLine.section(';', XemaEnums::STATUS_FINNAME, XemaEnums::STATUS_FINNAME),
                       importLine.section(';', XemaEnums::STATUS_FINABBREV, XemaEnums::STATUS_FINABBREV),
                       importLine.section(';', XemaEnums::STATUS_SWENAME, XemaEnums::STATUS_SWENAME),
                       importLine.section(';', XemaEnums::STATUS_ENGNAME, XemaEnums::STATUS_ENGNAME));

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
