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

ModelDataLoader::ModelDataLoader(QObject *parent) :
    QObject(parent)
{
}

void ModelDataLoader::loadBirdData(BirdModel *model)
{
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
        model->addItem(bird);
    }
}

void ModelDataLoader::loadLocationData(LocationModel *model)
{
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
    while (striimi.atEnd() == false)
    {
        QString locationLine;
        locationLine = striimi.readLine();
        Location location(locationLine.section(';', XemaEnums::LOCATION_TOWN, XemaEnums::LOCATION_TOWN),
                          locationLine.section(';', XemaEnums::LOCATION_PLACE, XemaEnums::LOCATION_PLACE),
                          locationLine.section(';', XemaEnums::LOCATION_WGS, XemaEnums::LOCATION_WGS),
                          locationLine.section(';', XemaEnums::LOCATION_YKJ, XemaEnums::LOCATION_YKJ));
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
    QFile tiedosto(":defaultstatuses.csv");
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
        if (birdLine.isEmpty() == false)
        {
            Status status(birdLine.section(';', XemaEnums::STATUS_NAME, XemaEnums::STATUS_NAME),
                          birdLine.section(';', XemaEnums::STATUS_ABBREV, XemaEnums::STATUS_ABBREV));
            model->addItem(status);
        }
    }
}

void ModelDataLoader::loadHistoryData(HistoryModel *model, const QString &date, const QString &place)
{
//    QFile tiedosto(dataFileDir() + "xemadata.txt");
    QFile tiedosto(dataFileDir() + "lokkitesti.txt");
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
        QString readPlace = line.section('#', XemaEnums::OBS_PLACE, XemaEnums::OBS_PLACE);
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
                count += line.section('#', XemaEnums::OBS_BIRDCOUNT+(i*11),XemaEnums::OBS_BIRDCOUNT+(i*11)).toInt();
            }
            readCount.setNum(count);
        }

//        qDebug() << "Kokonais lajin maara" << readCount;
        if (date.isEmpty() == true && place.isEmpty() == true)
        {
            HistoryItem item(line.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong(),
                             line.section('#', XemaEnums::OBS_PLACE, XemaEnums::OBS_PLACE),
                             line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1));
            item.setSpecies(line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES));
            item.addSpeciesCount(line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES), readCount.toInt());
            item.setTime(readTime);

            model->addItemAtBeginning(item);
            continue;
        }
        if (date.isEmpty() == false && date == readDate && place.isEmpty() == false && place == readPlace)
        {
            HistoryItem item(line.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong(),
                             line.section('#', XemaEnums::OBS_PLACE, XemaEnums::OBS_PLACE), line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1));
            item.setSpecies(line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES));
            item.addSpeciesCount(line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES), readCount.toInt());
            item.setTime(readTime);
            model->addItemAtBeginning(item);
            continue;
        }
        // TODO other combinations
    }
}

void ModelDataLoader::loadHistoryDateData(HistoryModel *model)
{
    //    QFile tiedosto(dataFileDir() + "xemadata.txt");
        QFile tiedosto(dataFileDir() + "lokkitesti.txt");
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
        QString species = line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES);
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
            HistoryItem item(line.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong(), line.section('#', XemaEnums::OBS_PLACE, XemaEnums::OBS_PLACE), line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1));
            item.setSpecies(line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES));
            item.addSpeciesCount(species, readCount.toInt());
            item.setTime(readTime);

            model->addItemAtBeginning(item);
        }
    }
}

void ModelDataLoader::loadHistoryPlaceData(HistoryModel *model, const QString &date)
{
    //    QFile tiedosto(dataFileDir() + "xemadata.txt");
        QFile tiedosto(dataFileDir() + "lokkitesti.txt");
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
        QString place = line.section('#', XemaEnums::OBS_PLACE, XemaEnums::OBS_PLACE);
        QString species = line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES);

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
                count += line.section('#', XemaEnums::OBS_BIRDCOUNT+(i*11),XemaEnums::OBS_BIRDCOUNT+(i*11)).toInt();
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
            if (sameDateFound && model->getItem(i).place() == place)
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
                                 line.section('#', XemaEnums::OBS_PLACE, XemaEnums::OBS_PLACE),
                                 line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1));
                item.setSpecies(line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES));
                item.addSpeciesCount(species, readCount.toInt());
                model->addItemAtBeginning(item);
            }
            continue;
        }
        else if (samePlaceFound == false)
        {
            HistoryItem item(line.section('#', XemaEnums::OBS_ID, XemaEnums::OBS_ID).toLongLong(),
                             line.section('#', XemaEnums::OBS_PLACE, XemaEnums::OBS_PLACE),
                             line.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1));
            item.setSpecies(line.section('#', XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES));
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
            AtlasIndex index(line.section(';', XemaEnums::ATLAS_VALUE, XemaEnums::ATLAS_VALUE).toInt());
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
