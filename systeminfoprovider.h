#ifndef SYSTEMINFOPROVIDER_H
#define SYSTEMINFOPROVIDER_H

#include <QObject>

#ifdef USE_MOBILITY
#include <QSystemDeviceInfo>
QTM_USE_NAMESPACE
#endif

class SystemInfoProvider : public QObject
{
    Q_OBJECT
public:
    explicit SystemInfoProvider(QObject *parent = 0);
    ~SystemInfoProvider();

signals:

public slots:

    static QString versionInfo();
    static bool imeiAccepted();
    static bool compassSupported();
};

#endif // SYSTEMINFOPROVIDER_H
