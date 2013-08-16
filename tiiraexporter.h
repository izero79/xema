#ifndef TIIRAEXPORTER_H
#define TIIRAEXPORTER_H

#include <QObject>
#include <QMap>

class TiiraServiceHelper;
class QNetworkConfiguration;
class LocationModel;
class PersonModel;
class BirdModel;
class CoordinateConverter;

class TiiraExporter : public QObject
{
    Q_OBJECT
public:
    explicit TiiraExporter(const QNetworkConfiguration &config, LocationModel *locations, PersonModel *persons, BirdModel *birds, QObject *parent = 0);
    bool exportOneRecord(long id);
    bool exportAllRecords(const QString &date, const QString &wantedplace);
    void login();
    void resetServer();
    
signals:
    void wrongCredientals();
    void noUploadRights();
    void loginOk(const QString &name);
    void serverLoginFailed();
    void tiiraExportDone();

public slots:

private slots:
    bool exportRecord(long id);
    bool exportRecords(const QString &date, const QString &place);
    void uploadOk(long id, const QString &csvId);
    void rowUploadOk(long id, int row);

    QMap<QString, QString> getFirstRowMap(const QString &data);
    QMap<QString, QString> getRowMap(const QString &data, int row);

    void addCsvIdsToRecords();

private:

    TiiraServiceHelper *mTiiraServiceHelper;
    LocationModel *mLocations;
    PersonModel *mPersons;
    BirdModel *mSpecies;
    CoordinateConverter *mCoordinates;
    QMap<long, QString> mSentRecords;
    QMap<long, QString> mUploadedRecords;
    bool mExportInProgress;
};

#endif // TIIRAEXPORTER_H
