#ifndef QMLWINDOW_H
#define QMLWINDOW_H

#include <QDeclarativeView>
#include <QMainWindow>

class FilterModel;
class BirdModel;
class PersonModel;
class LocationModel;
class StatusModel;
class HistoryModel;
class AtlasIndexModel;
class Settings;
class ModelDataWriter;

#if defined(Q_OS_SYMBIAN) && !defined(SYMBIAN3)
class QMLWindow : public QMainWindow
#else
class QMLWindow : public QDeclarativeView
#endif
{
    Q_OBJECT
public:
    explicit QMLWindow(QWidget *parent = 0);
    ~QMLWindow();

#if defined(Q_OS_SYMBIAN) && !defined(SYMBIAN3)
    void resizeEvent(QResizeEvent *event);
#endif

signals:
    void reloadHistory();
    void quit();
    void loadHistoryWithDate(const QString &date);
    void loadHistoryWithDateAndPlace(const QString &date, const QString &place);
    void restoreSpecies();
    void restoreLocations();
    void restoreObservers();
    void saveLocations();
    void importData();

public slots:
    void orientationChanged();
    void init();
    void setBirdModel(BirdModel *model);
    void setPersonModel(PersonModel *model);
    void setLocationModel(LocationModel *model);
    void setStatusModel(StatusModel *model);
    void setHistoryModel(HistoryModel *model);
    void setHistoryDateModel(HistoryModel *model);
    void setHistoryPlaceModel(HistoryModel *model);
    void setAtlasModel(AtlasIndexModel *model);

private slots:
    void writeNewObservation(const QString &data);
    void loadObservation(const QString &id);
    void deleteObservation(const QString &id, const QString &date, const QString &place);
    void saveDetailLevel(int level);
    void saveSystematicSorting(bool systematic);
    void exportData(bool onlyNew);

private:
#if defined(Q_OS_SYMBIAN) && !defined(SYMBIAN3)
    QDeclarativeView *mView;
#endif
    QDeclarativeContext *mRootContext;
    QObject *mRootObject;
    PersonModel *mPersonModel;
    FilterModel *mFilteredPersonModel;
    BirdModel *mBirdModel;
    FilterModel *mFilteredBirdModel;
    LocationModel *mLocationModel;
    FilterModel *mFilteredLocationModel;
    FilterModel *mFilteredStatusModel;
    FilterModel *mFilteredHistoryModel;
    FilterModel *mFilteredHistoryDateModel;
    FilterModel *mFilteredHistoryPlaceModel;
    FilterModel *mFilteredAtlasModel;
    Settings *mSettings;
    ModelDataWriter *mDataWriter;
};

#endif // QMLWINDOW_H
