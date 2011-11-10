#ifndef MODELDATALOADER_H
#define MODELDATALOADER_H

#include <QObject>

class BirdModel;
class PersonModel;
class LocationModel;

class ModelDataLoader : public QObject
{
    Q_OBJECT
public:
    explicit ModelDataLoader(QObject *parent = 0);

    static void loadBirdData( BirdModel *model );
    static void loadPersonData( PersonModel *model );
    static void loadLocationData( LocationModel *model );
signals:

public slots:

};

#endif // MODELDATALOADER_H
