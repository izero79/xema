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
#include "xemaenums.h"
#include "xemaconstants.h"
#include "dressmodel.h"
#include "agemodel.h"
#include "sexmodel.h"
#include <QLocale>

ModelDataLoader* ModelDataLoader::mDataLoader = 0;
ModelDataLoader* ModelDataLoader::instance() {
    if (!mDataLoader) {
        mDataLoader = new ModelDataLoader();
    }
    return mDataLoader;
}

ModelDataLoader::ModelDataLoader(QObject *parent) :
    QObject(parent),
    mBirdModel(0),
    mLocationModel(0)
{
}

void ModelDataLoader::loadBirdData(BirdModel *model)
{
    mBirdModel = model;
    if (QFile::exists(dataFileDir() + "lokkitestibirds.txt"))
    {
        QFile oldFile(dataFileDir() + "lokkitestibirds.txt");
        oldFile.rename(dataFileDir() + "xemabirddata.txt");
    }
    QFile tiedosto(dataFileDir() + "xemabirddata.txt");
    if (tiedosto.exists() == false)
    {
        tiedosto.setFileName(":specieslist.csv");
    }
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if (striimi.atEnd() == false)
    {
        striimi.readLine();
    }
    while (striimi.atEnd() == false)
    {
        QString birdLine;
        birdLine = striimi.readLine();
        Bird bird(birdLine.section(';', XemaEnums::BIRD_ID, XemaEnums::BIRD_ID).toInt(),
                   birdLine.section(';', XemaEnums::BIRD_FIN_GROUP, XemaEnums::BIRD_FIN_GROUP),
                   birdLine.section(';', XemaEnums::BIRD_SWE_GROUP, XemaEnums::BIRD_SWE_GROUP),
                   birdLine.section(';', XemaEnums::BIRD_LATIN_GROUP, XemaEnums::BIRD_LATIN_GROUP),
                   birdLine.section(';', XemaEnums::BIRD_FIN_NAME, XemaEnums::BIRD_FIN_NAME),
                   birdLine.section(';', XemaEnums::BIRD_SWE_NAME, XemaEnums::BIRD_SWE_NAME),
                   birdLine.section(';', XemaEnums::BIRD_ABBREV, XemaEnums::BIRD_ABBREV),
                   birdLine.section(';', XemaEnums::BIRD_LATIN_NAME, XemaEnums::BIRD_LATIN_NAME),
                   birdLine.section(';', XemaEnums::BIRD_CATEGORY, XemaEnums::BIRD_CATEGORY));
        // TODO LOC
        bird.setEngName(birdLine.section(';', XemaEnums::BIRD_ENG_NAME, XemaEnums::BIRD_ENG_NAME));
        bird.setEngGroup(birdLine.section(';', XemaEnums::BIRD_ENG_GROUP, XemaEnums::BIRD_ENG_GROUP));
        model->addItem(bird);
    }
}

void ModelDataLoader::loadLocationData(LocationModel *model)
{
    mLocationModel = model;
    if (QFile::exists(dataFileDir() + "lokkitestilocation.txt"))
    {
        QFile oldFile(dataFileDir() + "lokkitestilocation.txt");
        oldFile.rename(dataFileDir() + "xemalocationdata.txt");
    }

    QFile tiedosto(dataFileDir() + "xemalocationdata.txt");
    if (tiedosto.exists() == false)
    {
        tiedosto.setFileName(":defaultlocations.csv");
    }
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if (striimi.atEnd() == false)
    {
        striimi.readLine();
    }
    while (striimi.atEnd() == false)
    {
        QString locationLine;
        locationLine = striimi.readLine();
        Location location(locationLine.section(';', XemaEnums::LOCATION_TOWN, XemaEnums::LOCATION_TOWN),
                          locationLine.section(';', XemaEnums::LOCATION_PLACE, XemaEnums::LOCATION_PLACE),
                          locationLine.section(';', XemaEnums::LOCATION_WGS, XemaEnums::LOCATION_WGS),
                          locationLine.section(';', XemaEnums::LOCATION_YKJ, XemaEnums::LOCATION_YKJ));
        location.setSweTown(locationLine.section(';', XemaEnums::LOCATION_SWETOWN, XemaEnums::LOCATION_SWETOWN));
        location.setEngTown(locationLine.section(';', XemaEnums::LOCATION_ENGTOWN, XemaEnums::LOCATION_ENGTOWN));
        location.setSwePlace(locationLine.section(';', XemaEnums::LOCATION_SWEPLACE, XemaEnums::LOCATION_SWEPLACE));
        location.setEngPlace(locationLine.section(';', XemaEnums::LOCATION_ENGPLACE, XemaEnums::LOCATION_ENGPLACE));
        model->addItem(location);
    }
}

void ModelDataLoader::loadPersonData(PersonModel *model)
{
    if (QFile::exists(dataFileDir() + "lokkitestiperson.txt"))
    {
        QFile oldFile(dataFileDir() + "lokkitestiperson.txt");
        oldFile.rename(dataFileDir() + "xemapersondata.txt");
    }

    QFile tiedosto(dataFileDir() + "xemapersondata.txt");
    if (tiedosto.exists() == false)
    {
        tiedosto.setFileName(":defaultpersons.csv");
    }
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    while (striimi.atEnd() == false)
    {
        QString personLine;
        personLine = striimi.readLine();
        bool registered = false;
        if (personLine.section(';', XemaEnums::PERSON_REGISTERED, XemaEnums::PERSON_REGISTERED) == "true")
        {
            registered = true;
        }
        bool defaultName = false;
        if (personLine.section(';', XemaEnums::PERSON_DEFAULT, XemaEnums::PERSON_DEFAULT) == "true")
        {
            defaultName = true;
        }
        if (personLine.isEmpty() == false && personLine.contains(";"))
        {
            Person person(personLine.section(';', XemaEnums::PERSON_FIRSTNAME, XemaEnums::PERSON_FIRSTNAME),
                          personLine.section(';', XemaEnums::PERSON_SURNAME, XemaEnums::PERSON_SURNAME),
                          registered, defaultName);
            model->addItem(person);
        }
    }
    tiedosto.close();
}

void ModelDataLoader::loadStatusData(StatusModel *model)
{
    QFile tiedosto(dataFileDir() + "xemastatusdata.txt");
    if (tiedosto.exists() == false)
    {
        tiedosto.setFileName(":defaultstatuses.csv");
    }

    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if (striimi.atEnd() == false)
    {
        striimi.readLine();
    }
    while (striimi.atEnd() == false)
    {
        QString readLine;
        readLine = striimi.readLine();
        if (readLine.isEmpty() == false)
        {
            Status status(readLine.section(';', XemaEnums::STATUS_FINNAME, XemaEnums::STATUS_FINNAME),
                          readLine.section(';', XemaEnums::STATUS_FINABBREV, XemaEnums::STATUS_FINABBREV),
                          readLine.section(';', XemaEnums::STATUS_SWENAME, XemaEnums::STATUS_SWENAME),
                          readLine.section(';', XemaEnums::STATUS_ENGNAME, XemaEnums::STATUS_ENGNAME));
            model->addItem(status);
        }
    }
}

void ModelDataLoader::loadHistoryData(HistoryModel *model, const QString &date, const QString &place)
{
    QFile tiedosto(dataFileDir() + "xemadata.txt");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if (striimi.atEnd() == false)
    {
        striimi.readLine();
    }
    while (striimi.atEnd() == false)
    {
        QString line;
        line = striimi.readLine();
        QString readPlace = line.section('#', XemaEnums::OBS_TOWN, XemaEnums::OBS_LOCATION);
        readPlace.replace("#", ", ");
        readPlace = readLocation(readPlace);
        QString readDate = line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1);
        QString readTime = line.section('#', XemaEnums::OBS_TIME1, XemaEnums::OBS_TIME1);
        int rowCount = line.section('#', XemaEnums::OBS_ROWCOUNT, XemaEnums::OBS_ROWCOUNT).toInt();
//        qDebug() << "Riveja" << rowCount;
        QString readCount = 0;
        if (rowCount == 1)
        {
            readCount = line.section('#', XemaEnums::OBS_BIRDCOUNT, XemaEnums::OBS_BIRDCOUNT);
        }
        else
        {
            int count = 0;
            for(int i = 0; i < rowCount;i++)
            {
                count += line.section('#', XemaEnums::OBS_BIRDCOUNT+(i*XemaEnums::OBS_SUBFIELDCOUNT),XemaEnums::OBS_BIRDCOUNT+(i*XemaEnums::OBS_SUBFIELDCOUNT)).toInt();
            }
            readCount.setNum(count);
        }

//        qDebug() << "Kokonais lajin maara" << readCount;
        if (date.isEmpty() == true && place.isEmpty() == true)
        {
            HistoryItem item(line.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong(),
                             readPlace,
                             line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1));
            QString bird = readBird(line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES));
            item.setSpecies(bird);
            item.addSpeciesCount(bird, readCount.toInt());
            item.setTime(readTime);

            model->addItemAtBeginning(item);
            continue;
        }
        if (date.isEmpty() == false && date == readDate && place.isEmpty() == false && place == readPlace)
        {
            HistoryItem item(line.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong(),
                             readPlace, line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1));
            QString bird = readBird(line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES));
            item.setSpecies(bird);
            item.addSpeciesCount(bird, readCount.toInt());
            item.setTime(readTime);
            model->addItemAtBeginning(item);
            continue;
        }
        // TODO other combinations
    }
}

void ModelDataLoader::loadHistoryDateData(HistoryModel *model)
{
    QFile tiedosto(dataFileDir() + "xemadata.txt");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if (striimi.atEnd() == false)
    {
        striimi.readLine();
    }
    while (striimi.atEnd() == false)
    {
        QString line;
        line = striimi.readLine();
        QString date = line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1);
        QString readTime = line.section('#', XemaEnums::OBS_TIME1, XemaEnums::OBS_TIME1);
        QString species = readBird(line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES));
        int rowCount = line.section('#', XemaEnums::OBS_ROWCOUNT, XemaEnums::OBS_ROWCOUNT).toInt();
//        qDebug() << "Riveja" << rowCount;
        QString readCount = 0;
        if (rowCount == 1)
        {
            readCount = line.section('#', XemaEnums::OBS_BIRDCOUNT, XemaEnums::OBS_BIRDCOUNT);
        }
        else
        {
            int count = 0;
            for(int i = 0; i < rowCount;i++)
            {
                count += line.section('#', XemaEnums::OBS_BIRDCOUNT+(i*XemaEnums::OBS_SUBFIELDCOUNT),
                                      XemaEnums::OBS_BIRDCOUNT+(i*XemaEnums::OBS_SUBFIELDCOUNT)).toInt();
            }
            readCount.setNum(count);
        }

//        qDebug() << "Kokonais lajin maara" << readCount;

        int modelRowCount = model->rowCount();
        bool sameDateFound = false;
        for(int i = 0; i < modelRowCount; i++)
        {
//            qDebug() << "tarkistetaan" << date;
            if (model->getItem(i).date() == date)
            {
//                qDebug() << "sama pvm" << date;
                HistoryItem tmp = model->getItem(i);
                tmp.increaseDateCount();
                tmp.addSpecies(species);
                tmp.addSpeciesCount(species, readCount.toInt());
                model->replaceItem(i, tmp);
                sameDateFound = true;
            }
        }
        if (sameDateFound == false)
        {
            QString readPlace = line.section('#', XemaEnums::OBS_TOWN, XemaEnums::OBS_LOCATION);
            readPlace.replace("#", ", ");
            readPlace = readLocation(readPlace);

            HistoryItem item(line.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong(),
                             readPlace, line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1));
            item.setSpecies(species);
            item.addSpeciesCount(species, readCount.toInt());
            item.setTime(readTime);

            model->addItemAtBeginning(item);
        }
    }
}

void ModelDataLoader::loadHistoryPlaceData(HistoryModel *model, const QString &date)
{
    QFile tiedosto(dataFileDir() + "xemadata.txt");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if (striimi.atEnd() == false)
    {
        striimi.readLine();
    }
    while (striimi.atEnd() == false)
    {
        QString line;
        line = striimi.readLine();
        QString readDate = line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1);
        QString readPlace = line.section('#', XemaEnums::OBS_TOWN, XemaEnums::OBS_LOCATION);
        readPlace.replace("#", ", ");
        readPlace = readLocation(readPlace);
        QString species = readBird(line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES));

        int rowCount = line.section('#', XemaEnums::OBS_ROWCOUNT, XemaEnums::OBS_ROWCOUNT).toInt();
//        qDebug() << "Riveja" << rowCount;
        QString readCount = 0;
        if (rowCount == 1)
        {
            readCount = line.section('#', XemaEnums::OBS_BIRDCOUNT, XemaEnums::OBS_BIRDCOUNT);
        }
        else
        {
            int count = 0;
            for(int i = 0; i < rowCount;i++)
            {
                count += line.section('#', XemaEnums::OBS_BIRDCOUNT+(i*XemaEnums::OBS_SUBFIELDCOUNT),XemaEnums::OBS_BIRDCOUNT+(i*XemaEnums::OBS_SUBFIELDCOUNT)).toInt();
            }
            readCount.setNum(count);
        }

//        qDebug() << "Kokonais lajin maara" << readCount;

        bool sameDateFound = false;
        bool samePlaceFound = false;
//        qDebug() << "\nuusi itemi" << readDate << place;
        for(int i = 0; i < model->rowCount(); i++)
        {
//            qDebug() << "tarkistetaan kohta" << i;
            sameDateFound = false;
            samePlaceFound = false;
            if (model->getItem(i).date() == readDate)
            {
//                qDebug() << "sama date" <<  model->getItem(i).date();
                sameDateFound = true;
            }
            else
            {
//                qDebug() << "eri date  " <<  model->getItem(i).date() << "vs" << readDate;
                continue;
            }
            if (sameDateFound && model->getItem(i).place() == readPlace)
            {
//                qDebug() << "kohdassa" << i << "sama place ja date" << readDate << place;
                HistoryItem tmp = model->getItem(i);
                tmp.increasePlaceCount();
                tmp.increaseDateCount();
                tmp.addSpecies(species);
                tmp.addSpeciesCount(species, readCount.toInt());
                model->replaceItem(i, tmp);
                samePlaceFound = true;
                break;
            }
        }
        if (date.isEmpty() == false)
        {
            if (readDate == date && samePlaceFound == false)
            {
//                qDebug() << "adding with date" << readDate << place;
                HistoryItem item(line.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong(),
                                 readPlace,
                                 line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1));
                item.setSpecies(species);
                item.addSpeciesCount(species, readCount.toInt());
                model->addItemAtBeginning(item);
            }
            continue;
        }
        else if (samePlaceFound == false)
        {
            HistoryItem item(line.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong(),
                             readPlace,
                             line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1));
            item.setSpecies(species);
            item.addSpeciesCount(species, readCount.toInt());
            model->addItemAtBeginning(item);
        }
    }
}

void ModelDataLoader::loadAtlasData(AtlasIndexModel *model)
{
    QFile tiedosto(":atlasindexes.csv");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if (striimi.atEnd() == false)
    {
        striimi.readLine();
    }
    while (striimi.atEnd() == false)
    {
        QString line;
        line = striimi.readLine();
        if (line.isEmpty() == false)
        {
            AtlasIndex index(line.section(';', XemaEnums::ATLAS_VALUE, XemaEnums::ATLAS_VALUE),
                             line.section(';', XemaEnums::ATLAS_FIN, XemaEnums::ATLAS_FIN),
                             line.section(';', XemaEnums::ATLAS_SWE, XemaEnums::ATLAS_SWE),
                             line.section(';', XemaEnums::ATLAS_ENG, XemaEnums::ATLAS_ENG));
            model->addItem(index);
        }
    }
}

QString ModelDataLoader::dataFileDir()
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

QString ModelDataLoader::readBird(const QString &bird) {
    if (!mBirdModel) {
        return bird;
    }
    for (int i = 0; i < mBirdModel->rowCount(); i++) {
        if (QString::compare(bird,mBirdModel->getItem(i).abbreviation(),Qt::CaseInsensitive)==0) {
            // TODO select correct name based on language
            QString locale = QLocale::system().name();
            QString lang = locale.section("_",0,0);
            if (lang == "en") {
                return mBirdModel->getItem(i).engName();
            }
            else if (lang == "sv") {
                return mBirdModel->getItem(i).sweName();
            }

            return mBirdModel->getItem(i).finName();
        }
    }
    return bird;
}

QString ModelDataLoader::readLocation(const QString &location) {
    if (!mLocationModel) {
        return location;
    }
    for (int i = 0; i < mLocationModel->rowCount(); i++) {
        QString modelLocation = mLocationModel->getItem(i).town() + ", " + mLocationModel->getItem(i).place();
        if (QString::compare(location,modelLocation,Qt::CaseInsensitive)==0) {
            // TODO select correct name based on language
            QString locale = QLocale::system().name();
            QString lang = locale.section("_",0,0);
            if (lang == "en") {
                return mLocationModel->getItem(i).engTown() + ", " + mLocationModel->getItem(i).engPlace();
            }
            else if (lang == "sv") {
                return mLocationModel->getItem(i).sweTown() + ", " + mLocationModel->getItem(i).swePlace();
            }

            return location;
        }
    }
    return location;
}

QString ModelDataLoader::loadObservation(qlonglong id)
{
//    qDebug() << "LUETAAN" << id;
    if (QFile::exists(dataFileDir() + "xemadata.txt") == false)
    {
        return QString();
    }
    QFile tiedosto(dataFileDir() + "xemadata.txt");
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
    QString lineId = obsLine.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID);
    if (lineId == "Id" || lineId.isEmpty() == true)
    {
        return QString();
    }
    QString newLine = obsLine.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID);
    newLine.append("#");
    newLine.append(readBird(obsLine.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES)));
    newLine.append("#");
    newLine.append(obsLine.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_TIME2));
    newLine.append("#");
    QString location;
    location.append(obsLine.section('#', XemaEnums::OBS_TOWN, XemaEnums::OBS_TOWN));
    location.append(", ");
    location.append(obsLine.section('#', XemaEnums::OBS_LOCATION, XemaEnums::OBS_LOCATION));
    location = readLocation( location );
    qDebug() << location;
    newLine.append(location.section(", ", 0, 0));
    newLine.append("#");
    newLine.append(location.section(", ", 1));
//    newLine.append(obsLine.section('#', XemaEnums::OBS_TOWN, XemaEnums::OBS_LOCATION));
    newLine.append("#");
    newLine.append(obsLine.section('#', XemaEnums::OBS_XCOORD));
    return newLine;
}


void ModelDataLoader::loadDressData(DressModel *model)
{
    QFile tiedosto(":dresses.csv");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if (striimi.atEnd() == false)
    {
        striimi.readLine();
    }
    while (striimi.atEnd() == false)
    {
        QString line;
        line = striimi.readLine();
        if (line.isEmpty() == false)
        {
            Dress item(line.section(';', XemaEnums::DRESS_VALUE, XemaEnums::DRESS_VALUE),
                       line.section(';', XemaEnums::DRESS_FIN, XemaEnums::DRESS_FIN),
                       line.section(';', XemaEnums::DRESS_SWE, XemaEnums::DRESS_SWE),
                       line.section(';', XemaEnums::DRESS_ENG, XemaEnums::DRESS_ENG));
            model->addItem(item);
        }
    }
}

void ModelDataLoader::loadAgeData(AgeModel *model)
{
    QFile tiedosto(":ages.csv");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if (striimi.atEnd() == false)
    {
        striimi.readLine();
    }
    while (striimi.atEnd() == false)
    {
        QString line;
        line = striimi.readLine();
        if (line.isEmpty() == false)
        {
            Age item(line.section(';', XemaEnums::AGE_VALUE, XemaEnums::AGE_VALUE),
                       line.section(';', XemaEnums::AGE_FIN, XemaEnums::AGE_FIN),
                       line.section(';', XemaEnums::AGE_SWE, XemaEnums::AGE_SWE),
                       line.section(';', XemaEnums::AGE_ENG, XemaEnums::AGE_ENG));
            model->addItem(item);
        }
    }
}

void ModelDataLoader::loadSexData(SexModel *model)
{
    QFile tiedosto(":sexes.csv");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if (striimi.atEnd() == false)
    {
        striimi.readLine();
    }
    while (striimi.atEnd() == false)
    {
        QString line;
        line = striimi.readLine();
        if (line.isEmpty() == false)
        {
            Sex item(line.section(';', XemaEnums::SEX_FIN, XemaEnums::SEX_FIN),
                       line.section(';', XemaEnums::SEX_SWE, XemaEnums::SEX_SWE),
                       line.section(';', XemaEnums::SEX_ENG, XemaEnums::SEX_ENG));
            model->addItem(item);
        }
    }
}
