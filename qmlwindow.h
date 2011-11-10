#ifndef QMLWINDOW_H
#define QMLWINDOW_H

#include <QDeclarativeView>
#include <QMainWindow>

class FilterModel;
class BirdModel;
class PersonModel;
class LocationModel;

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

public slots:
    void orientationChanged();
    void init();
    void setBirdModel( BirdModel *model );
    void setPersonModel( PersonModel *model );
    void setLocationModel( LocationModel *model );

private:
#ifdef Q_OS_SYMBIAN
    QDeclarativeView *mView;
#endif
    QDeclarativeContext *mRootContext;
    QObject *mRootObject;
    FilterModel *mFilteredPersonModel;
    FilterModel *mFilteredBirdModel;
    FilterModel *mFilteredLocationModel;
};

#endif // QMLWINDOW_H
