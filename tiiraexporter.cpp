#include <QDebug>
#include <QFile>

#include "tiiraexporter.h"
#include "tiiraservicehelper.h"
#include "xemautils.h"
#include "xemaenums.h"
#include "birdmodel.h"
#include "locationmodel.h"
#include "personmodel.h"
#include "coordinateconverter.h"

TiiraExporter::TiiraExporter(const QNetworkConfiguration &config, LocationModel *locations, PersonModel *persons, BirdModel *birds, QObject *parent) :
    QObject(parent),
    mTiiraServiceHelper(0),
    mLocations(locations),
    mPersons(persons),
    mSpecies(birds),
    mCoordinates(0)
{
    mCoordinates = new CoordinateConverter(this);
    mTiiraServiceHelper = new TiiraServiceHelper("iZero", "37e0a47e842a33948d804f26ece2aa8f", this);
    mTiiraServiceHelper->init(config);
    connect(mTiiraServiceHelper,SIGNAL(uploadOk(long,QString)),this,SLOT(uploadOk(long,QString)));
    connect(mTiiraServiceHelper,SIGNAL(rowUploadOk(long,int)),this,SLOT(rowUploadOk(long,int)));
}

bool TiiraExporter::exportRecord(long id) {
    QMap<QString,QString> map;

    QFile tiedosto(XemaUtils::dataFileDir() + "xemadata.txt");
    QFile tmptiedosto(XemaUtils::dataFileDir() + "xemadata.tmp.txt");

    tiedosto.open(QFile::ReadOnly);
    tmptiedosto.open(QFile::ReadWrite|QFile::Truncate);
    QTextStream instriimi(&tiedosto);
    QTextStream tmp_stream(&tmptiedosto);
    instriimi.setCodec("ISO 8859-1");
    tmp_stream.setCodec("ISO 8859-1");

    QString obsLine;
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
        if (obsLine.section("#",XemaEnums::OBS_ID,XemaEnums::OBS_ID).toLongLong() == id) {
            qDebug() << "ObsRivi tiira exportiin" << obsLine;
            int xemaRows = obsLine.section("#", XemaEnums::OBS_ROWCOUNT, XemaEnums::OBS_ROWCOUNT).toInt();
            qDebug() << Q_FUNC_INFO << "XEMAROWS" << xemaRows;
            int exportPos = XemaEnums::OBS_EXPORTED + ((xemaRows-1) * XemaEnums::OBS_SUBFIELDCOUNT);
            QString exported = obsLine.section('#', exportPos, exportPos);
            bool doNotExport = false;/*
            if (date.isEmpty() == false) {
                qDebug() << Q_FUNC_INFO << "vain pvm" << date;
                QString obsDate = obsLine.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1);
                if (QString::compare(obsDate, date, Qt::CaseInsensitive) != 0) {
                    qDebug() << "do not export!!!";
                    doNotExport = true;
                }
            }
            if (place.isEmpty() == false) {
                qDebug() << Q_FUNC_INFO << "vain place" << place;
                QString location = obsLine.section('#', XemaEnums::OBS_TOWN, XemaEnums::OBS_LOCATION);
                location = location.replace("#", ", ");
                if (QString::compare(location, place, Qt::CaseInsensitive) != 0) {
                    qDebug() << "do not export!!!";
                    doNotExport = true;
                }
            }*/
/*
            if (onlyNew && QString::compare(exported, "true", Qt::CaseInsensitive) == 0) {
                doNotExport = true;
            }
*/
            qDebug() << Q_FUNC_INFO << "EXPORTED" << exported;
            int pos = -1;
            int i = 0;
            do
            {
                pos = obsLine.indexOf("#",pos+1);
                //qDebug() << "pos" << pos << i << exportPos;
                if (i == exportPos - 1)
                {
                    //qDebug() << "pos" << pos << "paikka" << i;

                    break;
                }
                i++;
            }
            while (pos>0);

            bool markAsExported = false;
            if (doNotExport == false)
            {
                map = getFirstRowMap(obsLine);
                qDebug() << "MAP" << map;
                mTiiraServiceHelper->uploadRecord(map, id);
                mSentRecords.insert(id, obsLine);

                //qDebug() << "VALMIS EXPORT RIVI" << formatToTiira(obsLine,locations,persons,birds,delimiter,allCountries);
                /*
                QString formatted = formatToTiira(obsLine,locations,persons,birds,delimiter,allCountries);
                if (formatted.isEmpty() == false) {
                    export_stream << formatted;
                    export_stream << "\n";
                    markAsExported = true;
                }*/
            }
            if (obsLine.length() > 20)
            {
                //qDebug() << "rivi ennen export settia" << obsLine;
                QString newLine;
                newLine = obsLine;

                QString start = newLine.mid(0, pos);
                qDebug() << "start" << start;

                if (markAsExported == true) {
                    start.append("#true#\n");
                } else {
                    start.append("#" + exported + "#\n");
                }
                //qDebug() << "uus rivi datassa export setin jalkeen" << start;
                tmp_stream << obsLine;
                tmp_stream << "\n";
            }
        } else {
            qDebug() << "Ei vietava rivi, tallennetaan takaisin";
            tmp_stream << obsLine;
            tmp_stream << "\n";
        }
    }
    tiedosto.close();
    tmptiedosto.close();

    QFile::remove(XemaUtils::dataFileDir() + "xemadata.backup");
    tiedosto.rename(XemaUtils::dataFileDir() + "xemadata.backup");
    tmptiedosto.rename(XemaUtils::dataFileDir() + "xemadata.txt");


    qDebug() << map;
    return true;

}

void TiiraExporter::uploadOk(long id, const QString &csvId) {
    qDebug() << Q_FUNC_INFO << id << csvId;
    QString record = mSentRecords.value(id);
    int rows = record.section("#",XemaEnums::OBS_ROWCOUNT,XemaEnums::OBS_ROWCOUNT).toInt();
    if (rows> 1) {
        for( int i = 2; i <= rows; i++) {
            QMap<QString,QString> map;
            map = getRowMap(record, i);
            map.insert("havaintotunnus", csvId);
            mTiiraServiceHelper->uploadRecordRow(map, id);
        }
        // TODO, csvid
        qDebug() << Q_FUNC_INFO << "TODO add csvid to record";
    } else {
        // TODO, mark record as tiira exported and add csvid
        qDebug() << Q_FUNC_INFO << "TODO mark record as tiira exported";
        mSentRecords.remove(id);
    }
}

void TiiraExporter::rowUploadOk(long id, int row) {
    qDebug() << Q_FUNC_INFO << id << row;
    QString record = mSentRecords.value(id);
    int rows = record.section("#",XemaEnums::OBS_ROWCOUNT,XemaEnums::OBS_ROWCOUNT).toInt();
    if (row == rows) {
        qDebug() << Q_FUNC_INFO << "TODO mark record as tiira exported";
        mSentRecords.remove(id);
    } else {
        qDebug() << Q_FUNC_INFO << "more rows";
    }

}


QMap<QString, QString> TiiraExporter::getFirstRowMap(const QString &data)
{
    QString species = data.section("#",XemaEnums::OBS_SPECIES, XemaEnums::OBS_SPECIES);
    QString date1 = data.section("#",XemaEnums::OBS_DATE1,XemaEnums::OBS_DATE1);
    QString date2 = data.section("#",XemaEnums::OBS_DATE2,XemaEnums::OBS_DATE2);
    QString time1 = data.section("#",XemaEnums::OBS_TIME1,XemaEnums::OBS_TIME1);
    QString time2 = data.section("#",XemaEnums::OBS_TIME2,XemaEnums::OBS_TIME2);
    QString info = data.section("#",XemaEnums::OBS_INFO,XemaEnums::OBS_INFO);
    QString nimet = data.section("#",XemaEnums::OBS_REGPERSON, XemaEnums::OBS_OTHERPERSON);
    QString hidden_string = data.section("#",XemaEnums::OBS_HIDDEN, XemaEnums::OBS_HIDDEN);
    QString hidden = "0";
    if (QString::compare(hidden_string, "true")) {
        hidden = "1";
    }
    QString rows = data.section("#",XemaEnums::OBS_ROWCOUNT,XemaEnums::OBS_ROWCOUNT);
    int xemaRows = rows.toInt();
    QString town = data.section("#", XemaEnums::OBS_TOWN, XemaEnums::OBS_TOWN);
    QString place = data.section("#", XemaEnums::OBS_LOCATION, XemaEnums::OBS_LOCATION);
    QString birdX = data.section("#", XemaEnums::OBS_BIRD_XCOORD, XemaEnums::OBS_BIRD_XCOORD);
    QString birdY = data.section("#", XemaEnums::OBS_BIRD_YCOORD, XemaEnums::OBS_BIRD_YCOORD);
    QString locationAccuracy = data.section("#", XemaEnums::OBS_ACCURACY, XemaEnums::OBS_ACCURACY);
    QString birdAccuracy = data.section("#", XemaEnums::OBS_BIRD_ACCURACY, XemaEnums::OBS_BIRD_ACCURACY);
    QString saveTime = data.section("#", XemaEnums::OBS_SAVETIME, XemaEnums::OBS_SAVETIME);
//    qDebug() << "EXPORT, birdX " << birdX;
//    qDebug() << "EXPORT, birdY " << birdY;

    QString exportLine = "";
    QString locationString = "#" + town + "#" + place;
//    qDebug() << "EXPORT, exportLine 2" << exportLine;

    QString species_en;
    QString species_sv;
    QString species_sc;
    QString species_fi;
    int rowCount = mSpecies->rowCount();

    bool exportWgs = true;

    // convert bird coords if needed
    QString x = birdX;
    QString y = birdY;
    double dy = y.toDouble();
    if (dy < 400) {
//            qDebug() << "lintu wgs";
    } else {
        QString ykj = birdY + ":" + birdX;
        QString wgs = mCoordinates->ykjToWgsString(ykj);
        birdY = wgs.section(":", 0, 0);
        birdX = wgs.section(":", 1, 1);
    }


    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 3" << exportLine;
    exportLine += species;
//    qDebug() << "EXPORT, exportLine 4" << exportLine;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 5" << exportLine;
//    qDebug() << "EXPORT, datetime" << datetime;
    /*
    if (datetime.section("#",3,3).isEmpty() == true)
    {
//        qDebug() << "EXPORT, exportLine 5.1" << exportLine;
        exportLine += datetime.section("#",0,2);
//        qDebug() << "EXPORT, exportLine 5.2" << exportLine;
        exportLine += "#";
//        qDebug() << "EXPORT, exportLine 5.3" << exportLine;
        exportLine += datetime.section("#",2,2);
//        qDebug() << "EXPORT, exportLine 5.4" << exportLine;
    }
    else
    {*/
//        exportLine += datetime;
//        qDebug() << "EXPORT, exportLine 6" << exportLine;
    //}
    rowCount = mLocations->rowCount();

    // add location coordinates if found
    QString country = "";
    QString place_x = "";
    QString place_y = "";

    for(int i = 0; i < rowCount; i++)
    {
        if (mLocations->getItem(i).town() == town && mLocations->getItem(i).place() == place)
        {
            // save country
            country =  mLocations->getItem(i).localizedCountry();
            QString coordinate = mLocations->getItem(i).wgsCoordinate();

            //qDebug() << "export paikka" << coordinate;
            if (coordinate == "" || coordinate == "0:0" || coordinate == "0") {
                break;
            }

            place_y = coordinate.section(":", 0, 0);
            place_x = coordinate.section(":", 1, 1);

            break;
        }
    }

    exportLine += locationString;
//    qDebug() << "EXPORT, exportLine 7" << exportLine;
    exportLine += "#";
    exportLine += locationAccuracy;
    exportLine += "#";
    exportLine += birdX;
    exportLine += "#";
    exportLine += birdY;
    exportLine += "#";
    exportLine += birdAccuracy;
    exportLine += "#";
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 8" << exportLine;
    exportLine += info;
    exportLine += "#";
    //exportLine += atlas;
//    qDebug() << "EXPORT, exportLine 9" << exportLine;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 10" << exportLine;

    // add data saver (default name if found)
    rowCount = mPersons->rowCount();
    QString saver;
    for(int i = 0; i < rowCount; i++)
    {
        if (mPersons->getItem(i).saver() == true)
        {
            saver = mPersons->getItem(i).firstName() + " " + mPersons->getItem(i).surName();
            break;
        }
    }

    exportLine += saver;
//        qDebug() << "EXPORT, exportLine 11" << exportLine;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 12" << exportLine;
    // tiira saving time
    exportLine += saveTime;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 13" << exportLine;

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
    exportLine += nimet;
//    qDebug() << "EXPORT, exportLine 14" << exportLine;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 15" << exportLine;
    exportLine += hidden;
//    qDebug() << "EXPORT, exportLine 16" << exportLine;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 17" << exportLine;

    // koontihavainto ja kuuluu havaintoon
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 18" << exportLine;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 19" << exportLine;

    // include exported or not (24 vs 25)
    QString loppu = data.section("#", XemaEnums::OBS_WEATHER+((xemaRows-1)*XemaEnums::OBS_SUBFIELDCOUNT),
                                 XemaEnums::OBS_WEATHER+((xemaRows-1)*XemaEnums::OBS_SUBFIELDCOUNT));

    loppu.prepend("#");
    loppu.append("#");
    loppu.append(country);
    if (exportWgs) {
        loppu.append("#WGS84");
    } else {
        loppu.append("#YKJ");
    }
    loppu.append("#");
    loppu.append(species_en);
    loppu.append("#");
    loppu.append(species_sv);
    loppu.append("#");
    loppu.append(species_sc);
    loppu.append("#");
    loppu.append(species_fi);
    //loppu.append("#");

    QString birdCount = data.section("#", XemaEnums::OBS_BIRDCOUNT, XemaEnums::OBS_BIRDCOUNT);
    QString sukupuoli = data.section('#', XemaEnums::OBS_SEX, XemaEnums::OBS_SEX);
    sukupuoli = sukupuoli.replace("koiras", "k");
    sukupuoli = sukupuoli.replace("naaras", "n");
    QString puku = data.section('#', XemaEnums::OBS_DRESS, XemaEnums::OBS_DRESS);
    QString ika = data.section('#', XemaEnums::OBS_AGE, XemaEnums::OBS_AGE);
    QString tila = data.section('#', XemaEnums::OBS_STATUS, XemaEnums::OBS_STATUS);
    QString bongattu = data.section('#', XemaEnums::OBS_BONGAUS, XemaEnums::OBS_BONGAUS);
    QString pesinta = data.section('#', XemaEnums::OBS_NEST, XemaEnums::OBS_NEST);
    QString kello_lintu1 = data.section('#', XemaEnums::OBS_BIRDTIME1, XemaEnums::OBS_BIRDTIME1);
    QString kello_lintu2 = data.section('#', XemaEnums::OBS_BIRDTIME2, XemaEnums::OBS_BIRDTIME2);
    QString lisat = data.section('#', XemaEnums::OBS_BIRDINFO, XemaEnums::OBS_BIRDINFO);
    QString atlaskoodi = data.section('#', XemaEnums::OBS_ATLAS, XemaEnums::OBS_ATLAS);
    QString parvi = data.section('#', XemaEnums::OBS_LOFT, XemaEnums::OBS_LOFT);


    //    qDebug() << "EXPORT, exportLine 20" << exportLine;
    exportLine += loppu;
    //qDebug() << "EXPORT, exportLine 21, delimcount" << exportLine.count("#");
/*
    if (xemaRows > 1) {
        for(int i = 1; i < xemaRows; i++)
        {
            exportLine += "\n";
            //qDebug() << "EXPORT, exportLine 22.1" << exportLine;
            QString rowLine = rowId;
            //exportLine += rowId;
            //exportLine += "#######################";
            rowLine += "#######################";
            //qDebug() << "EXPORT, rowLine " << rowLine;
            QString rivi = data.section("#", XemaEnums::OBS_BIRDCOUNT+(i*XemaEnums::OBS_SUBFIELDCOUNT), XemaEnums::OBS_INDIRECT+(i*XemaEnums::OBS_SUBFIELDCOUNT));
            rivi.replace("#koiras#", "#k#");
            rivi.replace("#naaras#", "#n#");
            //qDebug() << "EXPORT, lisataan rivi" << rivi;
            //exportLine += rivi;
            rowLine += rivi;
//            qDebug() << "EXPORT, exportLine 22.4" << exportLine;
            //qDebug() << "EXPORT, lisataan loput" << "########";
            //exportLine += "########";
            rowLine += "#######";
            //exportLine += loppu;
            //qDebug() << "EXPORT, rowLine, delimcount" << rowLine.count("#");
//            qDebug() << "EXPORT, exportLine 22.5" << exportLine;
            exportLine += rowLine;
        }
    }
*/
//    qDebug() << "exportLine ENNEN MUUTOSTA" << exportLine;
    exportLine.replace("#true","#X", Qt::CaseSensitive);
//    qDebug() << "EXPORT, exportLine 23" << exportLine;
    exportLine.replace("#false","#", Qt::CaseSensitive);
//    qDebug() << "EXPORT, exportLine 24" << exportLine;
//    qDebug() << "exportLine MUUTOKSEN JALKEEN" << exportLine;
/*    if( exportLine.endsWith( "#") == true ) {
        exportLine.remove(exportLine.size()-1,1);
    }*/
    //qDebug() << "FORMAT TO TIIRA JALKEEN" << exportLine;
    //qDebug() << "FORMAT TO TIIRA JALKEEN DELIMS" << exportLine.count(delimiter);
    QMap<QString, QString> map;
    map.insert("laji", species);
    map.insert("maara", birdCount );
    map.insert("pvm1", date1);
    map.insert("pvm2", date2);
    map.insert("kello_hav1", time1);
    map.insert("kello_hav2", time2);
    map.insert("kunta", town);
    map.insert("paikka", place);
    map.insert("x", place_x);
    map.insert("y", place_y);
    map.insert("paikan_tark", locationAccuracy);
    map.insert("x_lintu", birdX);
    map.insert("y_lintu", birdY);
    map.insert("linnun_tark", birdAccuracy);
    map.insert("havainnoijat", nimet);
    map.insert("lisatietoja", info.toLatin1());
    map.insert("tallentaja", saver);
    map.insert("salattu", hidden);
    map.insert("sukupuoli", sukupuoli);
    map.insert("puku", puku);
    map.insert("ika", ika);
    map.insert("tila", tila);
    map.insert("bongattu", bongattu);
    map.insert("pesinta", pesinta);
    map.insert("kello_lintu1", kello_lintu1);
    map.insert("kello_lintu2", kello_lintu2);
    map.insert("lisat", lisat);
    map.insert("atlaskoodi", atlaskoodi);
    map.insert("parvi", parvi);
    map.insert("havaintoriveja", rows);
    map.insert("koord_tyyppi", "WGS84");
    return map;
}

QMap<QString, QString> TiiraExporter::getRowMap(const QString &data, int row)
{
    QString rows = data.section("#",XemaEnums::OBS_ROWCOUNT,XemaEnums::OBS_ROWCOUNT);
    int xemaRows = rows.toInt();
    QString town = data.section("#", XemaEnums::OBS_TOWN, XemaEnums::OBS_TOWN);
    QString place = data.section("#", XemaEnums::OBS_LOCATION, XemaEnums::OBS_LOCATION);
    QString birdX = data.section("#", XemaEnums::OBS_BIRD_XCOORD, XemaEnums::OBS_BIRD_XCOORD);
    QString birdY = data.section("#", XemaEnums::OBS_BIRD_YCOORD, XemaEnums::OBS_BIRD_YCOORD);
    QString locationAccuracy = data.section("#", XemaEnums::OBS_ACCURACY, XemaEnums::OBS_ACCURACY);
    QString birdAccuracy = data.section("#", XemaEnums::OBS_BIRD_ACCURACY, XemaEnums::OBS_BIRD_ACCURACY);
    QString saveTime = data.section("#", XemaEnums::OBS_SAVETIME, XemaEnums::OBS_SAVETIME);
//    qDebug() << "EXPORT, birdX " << birdX;
//    qDebug() << "EXPORT, birdY " << birdY;

    QString exportLine = "";
    QString locationString = "#" + town + "#" + place;
//    qDebug() << "EXPORT, exportLine 2" << exportLine;

    QString species_en;
    QString species_sv;
    QString species_sc;
    QString species_fi;
    int rowCount = mSpecies->rowCount();

    bool exportWgs = true;

    // convert bird coords if needed
    QString x = birdX;
    QString y = birdY;
    double dy = y.toDouble();
    if (dy < 400) {
//            qDebug() << "lintu wgs";
    } else {
        QString ykj = birdY + ":" + birdX;
        QString wgs = mCoordinates->ykjToWgsString(ykj);
        birdY = wgs.section(":", 0, 0);
        birdX = wgs.section(":", 1, 1);
    }


    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 3" << exportLine;
    //exportLine += species;
//    qDebug() << "EXPORT, exportLine 4" << exportLine;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 5" << exportLine;
//    qDebug() << "EXPORT, datetime" << datetime;
    /*
    if (datetime.section("#",3,3).isEmpty() == true)
    {
//        qDebug() << "EXPORT, exportLine 5.1" << exportLine;
        exportLine += datetime.section("#",0,2);
//        qDebug() << "EXPORT, exportLine 5.2" << exportLine;
        exportLine += "#";
//        qDebug() << "EXPORT, exportLine 5.3" << exportLine;
        exportLine += datetime.section("#",2,2);
//        qDebug() << "EXPORT, exportLine 5.4" << exportLine;
    }
    else
    {*/
//        exportLine += datetime;
//        qDebug() << "EXPORT, exportLine 6" << exportLine;
    //}
    rowCount = mLocations->rowCount();

    // add location coordinates if found
    QString country = "";
    QString place_x = "";
    QString place_y = "";

    for(int i = 0; i < rowCount; i++)
    {
        if (mLocations->getItem(i).town() == town && mLocations->getItem(i).place() == place)
        {
            // save country
            country =  mLocations->getItem(i).localizedCountry();
            QString coordinate = mLocations->getItem(i).wgsCoordinate();

            //qDebug() << "export paikka" << coordinate;
            if (coordinate == "" || coordinate == "0:0" || coordinate == "0") {
                break;
            }

            place_x = coordinate.section(":", 0, 0);
            place_y = coordinate.section(":", 1, 1);

            break;
        }
    }

    exportLine += locationString;
//    qDebug() << "EXPORT, exportLine 7" << exportLine;
    exportLine += "#";
    exportLine += locationAccuracy;
    exportLine += "#";
    exportLine += birdX;
    exportLine += "#";
    exportLine += birdY;
    exportLine += "#";
    exportLine += birdAccuracy;
    exportLine += "#";
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 8" << exportLine;
    //exportLine += info;
    exportLine += "#";
    //exportLine += atlas;
//    qDebug() << "EXPORT, exportLine 9" << exportLine;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 10" << exportLine;

    // add data saver (default name if found)
    rowCount = mPersons->rowCount();
    QString saver;
    for(int i = 0; i < rowCount; i++)
    {
        if (mPersons->getItem(i).saver() == true)
        {
            saver = mPersons->getItem(i).firstName() + " " + mPersons->getItem(i).surName();
            break;
        }
    }

    exportLine += saver;
//        qDebug() << "EXPORT, exportLine 11" << exportLine;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 12" << exportLine;
    // tiira saving time
    exportLine += saveTime;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 13" << exportLine;

//    qDebug() << "NIMET ennen muutosta" << nimet;
    /*
    if (nimet.length() > 1 && nimet.endsWith('#') == false) {
        nimet.replace("#", ", ");
    }
    else {
        nimet.replace("#", "");
    }
    if (nimet.endsWith(", ")) {
        nimet = nimet.mid(0, nimet.length() - 2);
    }
    exportLine += nimet;*/
//    qDebug() << "EXPORT, exportLine 14" << exportLine;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 15" << exportLine;
    //exportLine += hidden;
//    qDebug() << "EXPORT, exportLine 16" << exportLine;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 17" << exportLine;

    // koontihavainto ja kuuluu havaintoon
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 18" << exportLine;
    exportLine += "#";
//    qDebug() << "EXPORT, exportLine 19" << exportLine;

    // include exported or not (24 vs 25)
    QString loppu = data.section("#", XemaEnums::OBS_WEATHER+((xemaRows-1)*XemaEnums::OBS_SUBFIELDCOUNT),
                                 XemaEnums::OBS_WEATHER+((xemaRows-1)*XemaEnums::OBS_SUBFIELDCOUNT));

    loppu.prepend("#");
    loppu.append("#");
    loppu.append(country);
    if (exportWgs) {
        loppu.append("#WGS84");
    } else {
        loppu.append("#YKJ");
    }
    loppu.append("#");
    loppu.append(species_en);
    loppu.append("#");
    loppu.append(species_sv);
    loppu.append("#");
    loppu.append(species_sc);
    loppu.append("#");
    loppu.append(species_fi);
    //loppu.append("#");
/*
    QString birdCount = data.section("#", XemaEnums::OBS_BIRDCOUNT, XemaEnums::OBS_BIRDCOUNT);
    QString sukupuoli = data.section('#', XemaEnums::OBS_SEX, XemaEnums::OBS_SEX);
    QString puku = data.section('#', XemaEnums::OBS_DRESS, XemaEnums::OBS_DRESS);
    QString ika = data.section('#', XemaEnums::OBS_AGE, XemaEnums::OBS_AGE);
    QString tila = data.section('#', XemaEnums::OBS_STATUS, XemaEnums::OBS_STATUS);
    QString bongattu = data.section('#', XemaEnums::OBS_BONGAUS, XemaEnums::OBS_BONGAUS);
    QString pesinta = data.section('#', XemaEnums::OBS_NEST, XemaEnums::OBS_NEST);
    QString kello_lintu1 = data.section('#', XemaEnums::OBS_BIRDTIME1, XemaEnums::OBS_BIRDTIME1);
    QString kello_lintu2 = data.section('#', XemaEnums::OBS_BIRDTIME2, XemaEnums::OBS_BIRDTIME2);
    QString lisat = data.section('#', XemaEnums::OBS_BIRDINFO, XemaEnums::OBS_BIRDINFO);
    QString parvi = data.section('#', XemaEnums::OBS_LOFT, XemaEnums::OBS_LOFT);
*/

    //    qDebug() << "EXPORT, exportLine 20" << exportLine;
    exportLine += loppu;
    //qDebug() << "EXPORT, exportLine 21, delimcount" << exportLine.count("#");
/*
    if (xemaRows > 1) {
        for(int i = 1; i < xemaRows; i++)
        {
            exportLine += "\n";
            //qDebug() << "EXPORT, exportLine 22.1" << exportLine;
            QString rowLine = rowId;
            //exportLine += rowId;
            //exportLine += "#######################";
            rowLine += "#######################";
            //qDebug() << "EXPORT, rowLine " << rowLine;*/
            QString rivi = data.section("#", XemaEnums::OBS_BIRDCOUNT+((row-1)*XemaEnums::OBS_SUBFIELDCOUNT), XemaEnums::OBS_INDIRECT+((row-1)*XemaEnums::OBS_SUBFIELDCOUNT));
            qDebug() << "RIVIII " << rivi;
            int offset = XemaEnums::OBS_BIRDCOUNT;
            QString birdCount = rivi.section("#", XemaEnums::OBS_BIRDCOUNT-offset, XemaEnums::OBS_BIRDCOUNT-offset);
            QString sukupuoli = rivi.section('#', XemaEnums::OBS_SEX-offset, XemaEnums::OBS_SEX-offset);
            QString puku = rivi.section('#', XemaEnums::OBS_DRESS-offset, XemaEnums::OBS_DRESS-offset);
            QString ika = rivi.section('#', XemaEnums::OBS_AGE-offset, XemaEnums::OBS_AGE-offset);
            QString tila = rivi.section('#', XemaEnums::OBS_STATUS-offset, XemaEnums::OBS_STATUS-offset);
            QString bongattu = rivi.section('#', XemaEnums::OBS_BONGAUS-offset, XemaEnums::OBS_BONGAUS-offset);
            QString pesinta = rivi.section('#', XemaEnums::OBS_NEST-offset, XemaEnums::OBS_NEST-offset);
            QString kello_lintu1 = rivi.section('#', XemaEnums::OBS_BIRDTIME1-offset, XemaEnums::OBS_BIRDTIME1-offset);
            QString kello_lintu2 = rivi.section('#', XemaEnums::OBS_BIRDTIME2-offset, XemaEnums::OBS_BIRDTIME2-offset);
            QString lisat = rivi.section('#', XemaEnums::OBS_BIRDINFO-offset, XemaEnums::OBS_BIRDINFO-offset);
            QString parvi = rivi.section('#', XemaEnums::OBS_LOFT-offset, XemaEnums::OBS_LOFT-offset);

            //rivi.replace("#koiras#", "#k#");
            //rivi.replace("#naaras#", "#n#");
            //qDebug() << "EXPORT, lisataan rivi" << rivi;
            //exportLine += rivi;
            //rowLine += rivi;
//            qDebug() << "EXPORT, exportLine 22.4" << exportLine;
            //qDebug() << "EXPORT, lisataan loput" << "########";
            //exportLine += "########";
            //rowLine += "#######";
            //exportLine += loppu;
            //qDebug() << "EXPORT, rowLine, delimcount" << rowLine.count("#");
//            qDebug() << "EXPORT, exportLine 22.5" << exportLine;
            //exportLine += rowLine;
//        }
//    }

//    qDebug() << "exportLine ENNEN MUUTOSTA" << exportLine;
    exportLine.replace("#true","#X", Qt::CaseSensitive);
//    qDebug() << "EXPORT, exportLine 23" << exportLine;
    exportLine.replace("#false","#", Qt::CaseSensitive);
//    qDebug() << "EXPORT, exportLine 24" << exportLine;
//    qDebug() << "exportLine MUUTOKSEN JALKEEN" << exportLine;
/*    if( exportLine.endsWith( "#") == true ) {
        exportLine.remove(exportLine.size()-1,1);
    }*/
    //qDebug() << "FORMAT TO TIIRA JALKEEN" << exportLine;
    //qDebug() << "FORMAT TO TIIRA JALKEEN DELIMS" << exportLine.count(delimiter);
    QMap<QString, QString> map;
    map.insert("maara", birdCount );
    map.insert("sukupuoli", sukupuoli);
    map.insert("puku", puku);
    map.insert("ika", ika);
    map.insert("tila", tila);
    map.insert("bongattu", bongattu);
    map.insert("pesinta", pesinta);
    map.insert("kello_lintu1", kello_lintu1);
    map.insert("kello_lintu2", kello_lintu2);
    map.insert("lisat", lisat);
    map.insert("parvi", parvi);
    return map;
}
