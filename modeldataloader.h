#ifndef MODELDATALOADER_H
#define MODELDATALOADER_H

#include <QObject>

class BirdModel;
class PersonModel;
class LocationModel;
class StatusModel;
class HistoryModel;
class AtlasIndexModel;
class AgeModel;
class DressModel;
class SexModel;
class DirectionModel;
class AccuracyModel;

class ModelDataLoader : public QObject
{
    Q_OBJECT
public:
    static ModelDataLoader* instance();

    void loadInitialBirdData(BirdModel *model, bool finOnly=false);
    void reloadInitialBirdData(BirdModel *model, bool finOnly=false);
    void loadDefaultBirdData(BirdModel *model, bool finOnly=false);

    void loadPersonData(PersonModel *model);

    void loadInitialLocationData(LocationModel *model);
    void loadDefaultLocationData(LocationModel *model);

    void loadInitialStatusData(StatusModel *model);
    void loadDefaultStatusData(StatusModel *model);

    void loadAtlasData(AtlasIndexModel *model);
    void loadHistoryData(HistoryModel *model, const QString &date = QString(), const QString &place = QString());
    void loadHistoryDateData(HistoryModel *model);
    void loadHistoryPlaceData(HistoryModel *model, const QString &date);
    QString loadObservation(qlonglong id);
    void loadSexData(SexModel *model);
    void loadDressData(DressModel *model);
    void loadAgeData(AgeModel *model);
    void loadDirectionData(DirectionModel *model);
    void loadAccuracyData(AccuracyModel *model);
    void loadBirdAccuracyData(AccuracyModel *model);
signals:

public slots:

private:
    void loadOnlyModifiedBirdData(BirdModel *model, bool finOnly);
    void loadLocationData(LocationModel *model, bool defaultOnly);
    void loadOnlyModifiedLocationData(LocationModel *model);
    void loadStatusData(StatusModel *model, bool defaultOnly);
    void loadOnlyModifiedStatusData(StatusModel *model);
    void loadBirdData(BirdModel *model, bool defaultOnly, bool finOnly);

    explicit ModelDataLoader(QObject *parent = 0);
    ~ModelDataLoader();

    QString dataFileDir();
    QString readBird(const QString &bird);
    QString readLocation(const QString &location);
    BirdModel *mBirdModel;
    LocationModel *mLocationModel;
    static ModelDataLoader *mDataLoader;


};

#endif // MODELDATALOADER_H
