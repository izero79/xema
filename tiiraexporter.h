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
    bool exportRecord(long id);
    
signals:
    
public slots:

private slots:
    void uploadOk(long id, const QString &csvId);
    void rowUploadOk(long id, int row);

    QMap<QString, QString> getFirstRowMap(const QString &data);
    QMap<QString, QString> getRowMap(const QString &data, int row);

private:

    TiiraServiceHelper *mTiiraServiceHelper;
    LocationModel *mLocations;
    PersonModel *mPersons;
    BirdModel *mSpecies;
    CoordinateConverter *mCoordinates;
    QMap<long, QString> mSentRecords;

};

#endif // TIIRAEXPORTER_H
