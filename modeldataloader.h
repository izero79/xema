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
    explicit ModelDataLoader(QObject *parent = 0);

    void loadBirdData( BirdModel *model );
    void loadPersonData( PersonModel *model );
    void loadLocationData( LocationModel *model );
    void loadStatusData( StatusModel *model );
    void loadAtlasData( AtlasIndexModel *model );
    void loadHistoryData( HistoryModel *model );
signals:

public slots:

};

#endif // MODELDATALOADER_H
