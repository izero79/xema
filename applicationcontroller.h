#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>

class QMLWindow;

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

signals:

public slots:

private:
    QMLWindow *mQMLWin;

};

#endif // APPLICATIONCONTROLLER_H
