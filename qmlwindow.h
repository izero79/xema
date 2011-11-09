#ifndef QMLWINDOW_H
#define QMLWINDOW_H

#include <QDeclarativeView>

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

private:
#ifdef Q_OS_SYMBIAN
    QDeclarativeView *mView;
#endif
    QDeclarativeContext *mRootContext;
    QObject *mRootObject;

};

#endif // QMLWINDOW_H
