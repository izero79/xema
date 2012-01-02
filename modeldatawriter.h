#ifndef MODELDATAWRITER_H
#define MODELDATAWRITER_H

#include <QObject>

class BirdModel;
class PersonModel;
class LocationModel;
class StatusModel;
class HistoryModel;

class ModelDataWriter : public QObject
{
    Q_OBJECT
public:
    explicit ModelDataWriter(QObject *parent = 0);

    void writeNewObservation( const QString &data );
    void replaceObservation( qlonglong id, const QString &data );
    QString loadObservation( qlonglong id );
    void writePersonData(PersonModel *model);
    void writeLocationData(LocationModel *model);
    void writeBirdData(BirdModel *model);

private:
    qlonglong getNewId();

signals:

public slots:

};

#endif // MODELDATAWRITER_H
