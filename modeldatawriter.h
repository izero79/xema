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
    explicit ModelDataWriter(QObject *parent = 0);

    void writeNewObservation(const QString &data, const LocationModel &locations, const PersonModel &persons);
    void replaceObservation(qlonglong id, const QString &data);
    QString loadObservation(qlonglong id);
    void deleteObservation(qlonglong id);
    void writePersonData(PersonModel *model);
    void writeLocationData(LocationModel *model);
    void writeBirdData(BirdModel *model);
    void exportHistory(bool onlyNew, LocationModel *locations, PersonModel *persons, BirdModel *birds);

    void removeCustomSpecies();
    void removeCustomLocations();
    void removeCustomObservers();
    void importHistory(LocationModel *locations, PersonModel *persons, BirdModel *birds);
    void importLine(const QStringList &lines, LocationModel *locations, PersonModel *persons, BirdModel *birds);

private:
    qlonglong getNewId();
    QString dataFileDir();
    QString exportDir();
    QString formatToTiira(const QString &data, LocationModel *locations, PersonModel *persons, BirdModel *birds);

signals:

public slots:

private:
    CoordinateConverter *mCoordinates;
};

#endif // MODELDATAWRITER_H
