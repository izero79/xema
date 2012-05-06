#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>

class QMLWindow;
class BirdModel;
class PersonModel;
class LocationModel;
class StatusModel;
class HistoryModel;
class AtlasIndexModel;
class ModelDataLoader;
class ModelDataWriter;
class AgeModel;
class SexModel;
class DressModel;

class ApplicationController : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationController(QObject *parent = 0);
    ~ApplicationController();

private slots:

#ifdef Q_WS_MAEMO_5
    void screenChange(const QDBusMessage &message);
#endif

    void initGUI();
    void initObjects();
    void reloadHistory();
    void loadHistoryWithDate(const QString &date);
    void loadHistoryWithDateAndPlace(const QString &date, const QString &place);


signals:

public slots:
    void quit();

private slots:
    void restoreSpecies();
    void restoreLocations();
    void restoreObservers();
    void saveLocations();
    void importData();

private:
    QMLWindow *mQMLWin;
    BirdModel *mBirdModel;
    PersonModel *mPersonModel;
    LocationModel *mLocationModel;
    StatusModel *mStatusModel;
    HistoryModel *mHistoryModel;
    HistoryModel *mHistoryDateModel;
    HistoryModel *mHistoryPlaceModel;
    AtlasIndexModel *mAtlasModel;
    ModelDataLoader *mModelLoader;
    ModelDataWriter *mModelWriter;
    AgeModel *mAgeModel;
    DressModel *mDressModel;
    SexModel *mSexModel;
};

#endif // APPLICATIONCONTROLLER_H
