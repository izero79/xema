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

signals:

public slots:
    void quit();

private:
    QMLWindow *mQMLWin;
    BirdModel *mBirdModel;
    PersonModel *mPersonModel;
    LocationModel *mLocationModel;
    StatusModel *mStatusModel;
    HistoryModel *mHistoryModel;
    AtlasIndexModel *mAtlasModel;
    ModelDataLoader *mModelLoader;
    ModelDataWriter *mModelWriter;
};

#endif // APPLICATIONCONTROLLER_H
