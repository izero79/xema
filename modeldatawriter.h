#ifndef MODELDATAWRITER_H
#define MODELDATAWRITER_H

#include <QObject>

class BirdModel;
class PersonModel;
class LocationModel;
class StatusModel;
class HistoryModel;
class CoordinateConverter;

class ModelDataWriter : public QObject
{
    Q_OBJECT
public:
    static ModelDataWriter* instance();


    void writeNewObservation(const QString &data);
    void replaceObservation(qlonglong id, const QString &data);
    void deleteObservation(qlonglong id);
    void writePersonData(PersonModel *model);
    void writeLocationData(LocationModel *model);
    void writeStatusData(StatusModel *model);
    void writeBirdData(BirdModel *model);
    void exportHistory(bool onlyNew, LocationModel *locations, PersonModel *persons, BirdModel *birds);
    void exportOwnData(LocationModel *lModel, BirdModel *bModel, StatusModel *sModel, PersonModel *pModel);

    void removeCustomSpecies();
    void removeCustomLocations();
    void removeCustomStatuses();
    void removeCustomObservers();
    void removeHistory();
    int importHistory(LocationModel *locations, PersonModel *persons);
    void importLine(const QStringList &lines, LocationModel *locations, PersonModel *persons, const QString &delimiter);
    int importOwnData( LocationModel *locations, PersonModel *persons, BirdModel *birds, StatusModel *statuses);

private:
    explicit ModelDataWriter(QObject *parent = 0);
    ~ModelDataWriter();
    qlonglong getNewId();
    QString dataFileDir();
    QString exportDir();
    QString importDir();
    QString importedDir();
    QString formatToTiira(const QString &data, LocationModel *locations, PersonModel *persons, BirdModel *birds);
    void checkAndCreateDirs();

signals:

public slots:

private:
    CoordinateConverter *mCoordinates;
    static ModelDataWriter *mDataWriter;

};

#endif // MODELDATAWRITER_H
