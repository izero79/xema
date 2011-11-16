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
class Settings;
class ModelDataWriter;

#ifdef Q_OS_SYMBIAN
class QMLWindow : public QMainWindow
#else
class QMLWindow : public QDeclarativeView
#endif
{
    Q_OBJECT
public:
    explicit QMLWindow(QWidget *parent = 0);
    ~QMLWindow();

#ifdef Q_OS_SYMBIAN
    void resizeEvent( QResizeEvent *event );
#endif

signals:
    void reloadHistory();

public slots:
    void orientationChanged();
    void init();
    void setBirdModel( BirdModel *model );
    void setPersonModel( PersonModel *model );
    void setLocationModel( LocationModel *model );
    void setStatusModel( StatusModel *model );
    void setHistoryModel( HistoryModel *model );

private slots:
    void writeNewObservation( const QString &data );
    void loadObservation( const QString &id );
    void saveDetailLevel( int level );
    void saveSystematicSorting( bool systematic );

private:
#ifdef Q_OS_SYMBIAN
    QDeclarativeView *mView;
#endif
    QDeclarativeContext *mRootContext;
    QObject *mRootObject;
    FilterModel *mFilteredPersonModel;
    FilterModel *mFilteredBirdModel;
    FilterModel *mFilteredLocationModel;
    FilterModel *mFilteredStatusModel;
    FilterModel *mFilteredHistoryModel;
    Settings *mSettings;
    ModelDataWriter *mDataWriter;
};

#endif // QMLWINDOW_H
