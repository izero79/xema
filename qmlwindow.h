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
class ModelDataLoader;
class AgeModel;
class SexModel;
class DressModel;
class DirectionModel;
class CoordinateConverter;
class KineticScroller;
class NetworkController;
class AccuracyModel;
class TiiraExporter;
class AssociationModel;

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
    void clearHistory();
    void restoreSpecies();
    void restoreStatuses();
    void restoreLocations();
    void restoreObservers();
    void saveLocations();
    void savePersons();
    void saveBirds();
    void reloadBirds();
    void reloadLocations();

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
    void setSexModel(SexModel *model);
    void setDirectionModel(DirectionModel *model);
    void setDressModel(DressModel *model);
    void setAgeModel(AgeModel *model);
    void setProcessing(bool processing);
    void setProcessingFalse();
    void setAccuracyModel(AccuracyModel *model);
    void setBirdAccuracyModel(AccuracyModel *model);
    void setAssociationModel(AssociationModel *model);

private slots:
    void writeNewObservation(const QString &data);
    void loadObservation(const QString &id);
    void deleteObservation(const QString &id, const QString &date, const QString &place);
    void saveDetailLevel(int level);
    void saveSystematicSorting(bool systematic);
    void saveWPSpecies(bool wpspecies);
    void saveOnlyDefaultCountry(bool onlydefault);
    void saveDefaultCountry(const QString &defaultCountry);
    void saveOnlyDefaultAssociation(bool onlydefault);
    void saveDefaultAssociation(const QString &defaultAssociation);
    void saveAlwaysShowOwn(bool show);
    void saveExportWgs(bool exportWgs);
    void exportData(bool onlyNew, bool allCountries, const QString &date, const QString &place, const QString &delimiter);
    void exportOwnData();
    void importOwnData();
    void openBrowser( const QString &url );
    void importData();
    void exportObsToTiira(const QString &id);
    void exportToTiira(const QString &date, const QString &place);

    void saveUseTiira(bool useTiira);
    void saveTiiraUsername(const QString &username);
    void saveTiiraPwdHash(const QString &pwdHash);
    void saveTiiraAutosave(bool autosave);
    void saveTiiraServer(int server);
    void saveTiiraServerUsername(const QString &username);
    void saveTiiraServerPassword(const QString &password);
    void saveTiiraLoginOk(bool ok);

    void tiiraLogin();
    void tiiraLoginOk(const QString &name);
    void tiiraWrongCredientals();
    void tiiraNoUploadRights();
    void tiiraServerLoginFailed();
    void tiiraExportDone();
    void tiiraLoginFailUnknown();

    void createTiiraExporter();
    void adReadyToShow(const QString &iconUrl, const QString &url);
    void tiiraClosed();
    void loadAd();

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
    StatusModel *mStatusModel;
    FilterModel *mFilteredLocationModel;
    FilterModel *mFilteredStatusModel;
    FilterModel *mFilteredHistoryModel;
    FilterModel *mFilteredHistoryDateModel;
    FilterModel *mFilteredHistoryPlaceModel;
    FilterModel *mFilteredAtlasModel;
    FilterModel *mFilteredAgeModel;
    FilterModel *mFilteredDressModel;
    FilterModel *mFilteredSexModel;
    FilterModel *mFilteredDirectionModel;
    Settings *mSettings;
    ModelDataWriter *mDataWriter;
    ModelDataLoader *mDataLoader;
    CoordinateConverter *mCoordinateConverter;
    KineticScroller *mKineticScroller;
    NetworkController *mNetworkController;
    FilterModel *mFilteredAccuracyModel;
    FilterModel *mFilteredBirdAccuracyModel;
    AccuracyModel *mLocationAccuracyModel;
    AccuracyModel *mBirdAccuracyModel;
    TiiraExporter *mTiiraExporter;
    FilterModel *mFilteredAssociationModel;
    AssociationModel *mAssociationModel;
};

#endif // QMLWINDOW_H
