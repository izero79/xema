#ifndef MODELDATALOADER_H
#define MODELDATALOADER_H

#include <QObject>

class BirdModel;
class PersonModel;
class LocationModel;
class StatusModel;
class HistoryModel;
class AtlasIndexModel;

class ModelDataLoader : public QObject
{
    Q_OBJECT
public:
    static ModelDataLoader* instance();

    void loadBirdData(BirdModel *model);
    void loadPersonData(PersonModel *model);
    void loadLocationData(LocationModel *model);
    void loadStatusData(StatusModel *model);
    void loadAtlasData(AtlasIndexModel *model);
    void loadHistoryData(HistoryModel *model, const QString &date = QString(), const QString &place = QString());
    void loadHistoryDateData(HistoryModel *model);
    void loadHistoryPlaceData(HistoryModel *model, const QString &date = QString());
    QString loadObservation(qlonglong id);
signals:

public slots:

private:
    explicit ModelDataLoader(QObject *parent = 0);

    QString dataFileDir();
    QString readBird(const QString &bird);
    BirdModel *mBirdModel;
    static ModelDataLoader *mDataLoader;


};

#endif // MODELDATALOADER_H
