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
#include "settings.h"

TiiraExporter::TiiraExporter(const QNetworkConfiguration &config, LocationModel *locations, PersonModel *persons, BirdModel *birds, QObject *parent) :
    QObject(parent),
    mTiiraServiceHelper(0),
    mLocations(locations),
    mPersons(persons),
    mSpecies(birds),
    mCoordinates(0),
    mExportInProgress(false)
{
    mCoordinates = new CoordinateConverter(this);
    QString userName = Settings::tiiraUsername();
    QString pwdHash = Settings::tiiraPwdHash();
    mTiiraServiceHelper = new TiiraServiceHelper(userName, pwdHash, this);
    connect(mTiiraServiceHelper,SIGNAL(uploadOk(long,QString)),this,SLOT(uploadOk(long,QString)));
    connect(mTiiraServiceHelper,SIGNAL(rowUploadOk(long,int)),this,SLOT(rowUploadOk(long,int)));
    connect(mTiiraServiceHelper,SIGNAL(loginOk(QString)),this,SIGNAL(loginOk(QString)));
    connect(mTiiraServiceHelper,SIGNAL(wrongCredientals()),this,SIGNAL(wrongCredientals()));
    connect(mTiiraServiceHelper,SIGNAL(noUploadRights()),this,SIGNAL(noUploadRights()));
    connect(mTiiraServiceHelper,SIGNAL(serverLoginFailed()),this,SIGNAL(serverLoginFailed()));
    connect(mTiiraServiceHelper,SIGNAL(loginFailUnknown()),this,SIGNAL(loginFailUnknown()));
    connect(mTiiraServiceHelper,SIGNAL(adLoaded(QString,QString)),this,SIGNAL(adLoaded(QString,QString)));
    connect(mTiiraServiceHelper,SIGNAL(tiiraClosed()),this,SIGNAL(tiiraClosed()));
    mTiiraServiceHelper->init(config);
}

void TiiraExporter::resetServer() {
    QString userName = Settings::tiiraUsername();
    QString pwdHash = Settings::tiiraPwdHash();
    mTiiraServiceHelper->resetServer(userName, pwdHash);
}

bool TiiraExporter::exportOneRecord(long id) {
    mExportInProgress = true;
    exportRecord(id);
    mExportInProgress = false;
    return true;
}

bool TiiraExporter::exportAllRecords(const QString &date, const QString &place) {
    qDebug() << Q_FUNC_INFO << date << place;
    mExportInProgress = true;
    exportRecords(date, place);
    mExportInProgress = false;
    return true;
}

void TiiraExporter::getAd() {
    mTiiraServiceHelper->getAd();
}

bool TiiraExporter::exportRecord(long id) {
    QMap<QString,QString> map;

    QFile tiedosto(XemaUtils::dataFileDir() + "xemadata.txt");

    tiedosto.open(QFile::ReadOnly);
    QTextStream instriimi(&tiedosto);
    instriimi.setCodec("ISO 8859-1");

    QString obsLine;
    while (instriimi.atEnd() == false)
    {
        obsLine = instriimi.readLine();
        if (obsLine.section("#",XemaEnums::OBS_ID,XemaEnums::OBS_ID) == "Id" ||
            obsLine.section("#",XemaEnums::OBS_ID,XemaEnums::OBS_ID) == "Rivi-ID")
        {
            continue;
        }
        if (obsLine.section("#",XemaEnums::OBS_ID,XemaEnums::OBS_ID).toLongLong() == id) {
            qDebug() << "ObsRivi tiira exportiin" << obsLine;
            int xemaRows = obsLine.section("#", XemaEnums::OBS_ROWCOUNT, XemaEnums::OBS_ROWCOUNT).toInt();
            qDebug() << Q_FUNC_INFO << "XEMAROWS" << xemaRows;
            int exportPos = XemaEnums::OBS_EXPORTED + ((xemaRows-1) * XemaEnums::OBS_SUBFIELDCOUNT);
            QString tiiraexported = obsLine.section('#', exportPos+1, exportPos+1);
            QString notiiraexp = obsLine.section('#', exportPos+2, exportPos+2);
            QString town = obsLine.section("#", XemaEnums::OBS_TOWN, XemaEnums::OBS_TOWN);
            QString place = obsLine.section("#", XemaEnums::OBS_LOCATION, XemaEnums::OBS_LOCATION);


            int locationRowCount = mLocations->rowCount();

            // add location coordinates if found
            QString country = "";

            for(int i = 0; i < locationRowCount; i++)
            {
                if (mLocations->getItem(i).town() == town && mLocations->getItem(i).place() == place)
                {
                    country =  mLocations->getItem(i).localizedCountry();
                    break;
                }
            }
            bool doNotExport = false;

            if ((QString::compare(country, "suomi", Qt::CaseInsensitive) != 0 &&
                QString::compare(country, "finland", Qt::CaseInsensitive) != 0) &&
                !country.isEmpty()) {
                doNotExport = true;
            }

            if (!doNotExport && QString::compare(tiiraexported, "true", Qt::CaseInsensitive) == 0) {
                qDebug() << "already tiira export";
                doNotExport = true;
            }

            if (!doNotExport && QString::compare(notiiraexp, "true", Qt::CaseInsensitive) == 0) {
                doNotExport = true;
            }

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

            if (doNotExport == false)
            {
                map = getFirstRowMap(obsLine);
                //qDebug() << "MAP" << map;
                mSentRecords.insert(id, obsLine);
                mTiiraServiceHelper->uploadRecord(map, id);
            }

        } else {
            //qDebug() << "Ei vietava rivi";
        }
    }
    tiedosto.close();

    qDebug() << map;
    return true;

}

bool TiiraExporter::exportRecords(const QString &date, const QString &wantedplace) {
    QMap<QString,QString> map;

    QFile tiedosto(XemaUtils::dataFileDir() + "xemadata.txt");

    tiedosto.open(QFile::ReadOnly);
    QTextStream instriimi(&tiedosto);
    instriimi.setCodec("ISO 8859-1");

    QString obsLine;
    while (instriimi.atEnd() == false)
    {
        obsLine = instriimi.readLine();
        if (obsLine.section("#",XemaEnums::OBS_ID,XemaEnums::OBS_ID) == "Id" ||
            obsLine.section("#",XemaEnums::OBS_ID,XemaEnums::OBS_ID) == "Rivi-ID")
        {
            continue;
        }
        long id = obsLine.section("#",XemaEnums::OBS_ID,XemaEnums::OBS_ID).toLong();
        qDebug() << "ObsRivi tiira exportiin" << id;
        int xemaRows = obsLine.section("#", XemaEnums::OBS_ROWCOUNT, XemaEnums::OBS_ROWCOUNT).toInt();
        qDebug() << Q_FUNC_INFO << "XEMAROWS" << xemaRows;
        int exportPos = XemaEnums::OBS_EXPORTED + ((xemaRows-1) * XemaEnums::OBS_SUBFIELDCOUNT);
        QString tiiraexported = obsLine.section('#', exportPos+1, exportPos+1);
        QString notiiraexp = obsLine.section('#', exportPos+2, exportPos+2);
        QString town = obsLine.section("#", XemaEnums::OBS_TOWN, XemaEnums::OBS_TOWN);
        QString location = obsLine.section("#", XemaEnums::OBS_LOCATION, XemaEnums::OBS_LOCATION);
        QString place = town + ", " + location;


        qDebug() << Q_FUNC_INFO << "locationRowCount";
        int locationRowCount = mLocations->rowCount();
        qDebug() << Q_FUNC_INFO << "locationRowCount - done";

        QString country = "";

        for(int i = 0; i < locationRowCount; i++)
        {
            if (mLocations->getItem(i).town() == town && mLocations->getItem(i).place() == location)
            {
                country =  mLocations->getItem(i).localizedCountry();
                break;
            }
        }
        bool doNotExport = false;

        if ((QString::compare(country, "suomi", Qt::CaseInsensitive) != 0 &&
            QString::compare(country, "finland", Qt::CaseInsensitive) != 0) &&
            !country.isEmpty()) {
            qDebug() << "not in finland";
            doNotExport = true;
        }

        if (!doNotExport && QString::compare(tiiraexported, "true", Qt::CaseInsensitive) == 0) {
            qDebug() << "already tiira export";
            doNotExport = true;
        }

        if (!doNotExport && QString::compare(notiiraexp, "true", Qt::CaseInsensitive) == 0) {
            qDebug() << "no tiira export";
            doNotExport = true;
        }


        if (!doNotExport && date.isEmpty() == false) {
            qDebug() << Q_FUNC_INFO << "vain pvm" << date;
            QString obsDate = obsLine.section('#', XemaEnums::OBS_DATE1, XemaEnums::OBS_DATE1);
            if (QString::compare(obsDate, date, Qt::CaseInsensitive) != 0) {
                qDebug() << "wrong date, do not export!!!";
                doNotExport = true;
            }
        }
        if (!doNotExport && wantedplace.isEmpty() == false) {
            qDebug() << Q_FUNC_INFO << "vain place" << wantedplace;
            if (QString::compare(place, wantedplace, Qt::CaseInsensitive) != 0) {
                qDebug() << "wrong place, do not export!!!";
                doNotExport = true;
            }
        }
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

        if (doNotExport == false)
        {
            map = getFirstRowMap(obsLine);
            qDebug() << "MAP" << map;
            mSentRecords.insert(id, obsLine);
            mTiiraServiceHelper->uploadRecord(map, id);
        }
    }
    tiedosto.close();
    return true;

}

void TiiraExporter::uploadOk(long id, const QString &csvId) {
    qDebug() << Q_FUNC_INFO << id << csvId;
    QString record = mSentRecords.value(id);
    int rows = record.section("#",XemaEnums::OBS_ROWCOUNT,XemaEnums::OBS_ROWCOUNT).toInt();
    mUploadedRecords.insert(id, csvId);
    if (rows> 1) {
        for( int i = 2; i <= rows; i++) {
            QMap<QString,QString> map;
            map = getRowMap(record, i);
            map.insert("havaintotunnus", csvId);
            mTiiraServiceHelper->uploadRecordRow(map, id);
        }
    } else {
        mSentRecords.remove(id);
        if(mExportInProgress == false) {
            addCsvIdsToRecords();
        }
    }
}

void TiiraExporter::rowUploadOk(long id, int row) {
    qDebug() << Q_FUNC_INFO << id << row;
    QString record = mSentRecords.value(id);
    int rows = record.section("#",XemaEnums::OBS_ROWCOUNT,XemaEnums::OBS_ROWCOUNT).toInt();
    if (row == rows) {
        mSentRecords.remove(id);
        mTiiraServiceHelper->removeId(id);
        if(mExportInProgress == false) {
            addCsvIdsToRecords();
        }
    } else {
        qDebug() << Q_FUNC_INFO << "more rows" << rows;
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
    QString town = data.section("#", XemaEnums::OBS_TOWN, XemaEnums::OBS_TOWN);
    QString place = data.section("#", XemaEnums::OBS_LOCATION, XemaEnums::OBS_LOCATION);
    QString birdX = data.section("#", XemaEnums::OBS_BIRD_XCOORD, XemaEnums::OBS_BIRD_XCOORD);
    QString birdY = data.section("#", XemaEnums::OBS_BIRD_YCOORD, XemaEnums::OBS_BIRD_YCOORD);
    QString locationAccuracy = data.section("#", XemaEnums::OBS_ACCURACY, XemaEnums::OBS_ACCURACY);
    QString birdAccuracy = data.section("#", XemaEnums::OBS_BIRD_ACCURACY, XemaEnums::OBS_BIRD_ACCURACY);

    int rowCount = mSpecies->rowCount();

    // convert bird coords if needed
    QString x = birdX;
    QString y = birdY;
    double dy = y.toDouble();
    if (dy > 400) {
        QString ykj = birdY + ":" + birdX;
        QString wgs = mCoordinates->ykjToWgsString(ykj);
        birdY = wgs.section(":", 0, 0);
        birdX = wgs.section(":", 1, 1);
    }
    while (birdY.endsWith('0')) {
        birdY.remove(birdY.length()-1, 1);
    }
    while (birdX.endsWith('0')) {
        birdX.remove(birdX.length()-1, 1);
    }

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
    while (place_y.endsWith('0')) {
        place_y.remove(place_y.length()-1, 1);
    }
    while (place_x.endsWith('0')) {
        place_x.remove(place_x.length()-1, 1);
    }


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

    if (nimet.length() > 1 && nimet.endsWith('#') == false) {
        nimet.replace("#", ", ");
    }
    else {
        nimet.replace("#", "");
    }
    if (nimet.endsWith(", ")) {
        nimet = nimet.mid(0, nimet.length() - 2);
    }

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


    QMap<QString, QString> map;
    map.insert("laji", species.toLatin1());
    map.insert("maara", birdCount.toLatin1() );
    map.insert("pvm1", date1.toLatin1());
    map.insert("pvm2", date2.toLatin1());
    map.insert("kello_hav1", time1.toLatin1());
    map.insert("kello_hav2", time2.toLatin1());
    map.insert("kunta", town.toLatin1());
    map.insert("paikka", place.toLatin1());
    map.insert("x", place_x.toLatin1());
    map.insert("y", place_y.toLatin1());
    map.insert("paikan_tark", locationAccuracy);
    map.insert("x_lintu", birdX.toLatin1());
    map.insert("y_lintu", birdY.toLatin1());
    map.insert("linnun_tark", birdAccuracy);
    map.insert("havainnoijat", nimet.toLatin1());
    map.insert("lisatietoja", info.toLatin1());
    map.insert("tallentaja", saver.toLatin1());
    map.insert("salattu", hidden.toLatin1());
    map.insert("sukupuoli", sukupuoli.toLatin1());
    map.insert("puku", puku.toLatin1());
    map.insert("ika", ika.toLatin1());
    map.insert("tila", tila.toLatin1());
    map.insert("bongattu", bongattu.toLatin1());
    map.insert("pesinta", pesinta.toLatin1());
    map.insert("kello_lintu1", kello_lintu1.toLatin1());
    map.insert("kello_lintu2", kello_lintu2.toLatin1());
    map.insert("lisat", lisat.toLatin1());
    map.insert("atlaskoodi", atlaskoodi.toLatin1());
    map.insert("parvi", parvi.toLatin1());
    map.insert("havaintoriveja", rows.toLatin1());
    map.insert("koord_tyyppi", "WGS84");
    return map;
}

QMap<QString, QString> TiiraExporter::getRowMap(const QString &data, int row)
{
    QString town = data.section("#", XemaEnums::OBS_TOWN, XemaEnums::OBS_TOWN);
    QString place = data.section("#", XemaEnums::OBS_LOCATION, XemaEnums::OBS_LOCATION);
    QString birdX = data.section("#", XemaEnums::OBS_BIRD_XCOORD, XemaEnums::OBS_BIRD_XCOORD);
    QString birdY = data.section("#", XemaEnums::OBS_BIRD_YCOORD, XemaEnums::OBS_BIRD_YCOORD);

    int rowCount = mSpecies->rowCount();

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

    QString rivi = data.section("#", XemaEnums::OBS_BIRDCOUNT+((row-1)*XemaEnums::OBS_SUBFIELDCOUNT), XemaEnums::OBS_INDIRECT+((row-1)*XemaEnums::OBS_SUBFIELDCOUNT));
    qDebug() << "RIVIII " << rivi;
    int offset = XemaEnums::OBS_BIRDCOUNT;
    QString birdCount = rivi.section("#", XemaEnums::OBS_BIRDCOUNT-offset, XemaEnums::OBS_BIRDCOUNT-offset);
    QString sukupuoli = rivi.section('#', XemaEnums::OBS_SEX-offset, XemaEnums::OBS_SEX-offset);
    sukupuoli = sukupuoli.replace("koiras", "k");
    sukupuoli = sukupuoli.replace("naaras", "n");
    QString puku = rivi.section('#', XemaEnums::OBS_DRESS-offset, XemaEnums::OBS_DRESS-offset);
    QString ika = rivi.section('#', XemaEnums::OBS_AGE-offset, XemaEnums::OBS_AGE-offset);
    QString tila = rivi.section('#', XemaEnums::OBS_STATUS-offset, XemaEnums::OBS_STATUS-offset);
    QString bongattu = rivi.section('#', XemaEnums::OBS_BONGAUS-offset, XemaEnums::OBS_BONGAUS-offset);
    QString pesinta = rivi.section('#', XemaEnums::OBS_NEST-offset, XemaEnums::OBS_NEST-offset);
    QString kello_lintu1 = rivi.section('#', XemaEnums::OBS_BIRDTIME1-offset, XemaEnums::OBS_BIRDTIME1-offset);
    QString kello_lintu2 = rivi.section('#', XemaEnums::OBS_BIRDTIME2-offset, XemaEnums::OBS_BIRDTIME2-offset);
    QString lisat = rivi.section('#', XemaEnums::OBS_BIRDINFO-offset, XemaEnums::OBS_BIRDINFO-offset);
    QString parvi = rivi.section('#', XemaEnums::OBS_LOFT-offset, XemaEnums::OBS_LOFT-offset);

    QMap<QString, QString> map;
    map.insert("maara", birdCount.toLatin1() );
    map.insert("sukupuoli", sukupuoli.toLatin1());
    map.insert("puku", puku.toLatin1());
    map.insert("ika", ika.toLatin1());
    map.insert("tila", tila.toLatin1());
    map.insert("bongattu", bongattu.toLatin1());
    map.insert("pesinta", pesinta.toLatin1());
    map.insert("kello_lintu1", kello_lintu1.toLatin1());
    map.insert("kello_lintu2", kello_lintu2.toLatin1());
    map.insert("lisat", lisat.toLatin1());
    map.insert("parvi", parvi.toLatin1());
    return map;
}

void TiiraExporter::login() {
    mTiiraServiceHelper->login();

}

void TiiraExporter::addCsvIdsToRecords() {

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
        long id = obsLine.section("#",XemaEnums::OBS_ID,XemaEnums::OBS_ID).toLongLong();

        if ( mUploadedRecords.contains(id)) {
            int xemaRows = obsLine.section("#", XemaEnums::OBS_ROWCOUNT, XemaEnums::OBS_ROWCOUNT).toInt();
            int exportPos = XemaEnums::OBS_EXPORTED + ((xemaRows-1) * XemaEnums::OBS_SUBFIELDCOUNT);
            QString exported = obsLine.section('#', exportPos, exportPos);
            QString notiiraexp = obsLine.section('#', exportPos+2, exportPos+2);
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

            if (obsLine.length() > 20)
            {
                //qDebug() << "rivi ennen export settia" << obsLine;
                QString newLine;
                newLine = obsLine;

                QString start = newLine.mid(0, pos);
                qDebug() << "start" << start;

                start.append("#" + exported + "#");
                start.append("true#");
                start.append(notiiraexp + "#");
                start.append(mUploadedRecords.value(id) + "#");
                start.append("\n");

                qDebug() << "uus rivi datassa tiira export setin jalkeen" << start;
                tmp_stream << start;
                mUploadedRecords.remove(id);
            }
        } else {
            //qDebug() << "Ei vietava rivi, tallennetaan takaisin";
            tmp_stream << obsLine;
            tmp_stream << "\n";
        }
    }
    tiedosto.close();
    tmptiedosto.close();

    QFile::remove(XemaUtils::dataFileDir() + "xemadata.backup");
    tiedosto.rename(XemaUtils::dataFileDir() + "xemadata.backup");
    tmptiedosto.rename(XemaUtils::dataFileDir() + "xemadata.txt");
    emit tiiraExportDone();
}
