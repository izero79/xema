#ifndef SYSTEMINFOPROVIDER_H
#define SYSTEMINFOPROVIDER_H

#include <QObject>
#include <QVariant>
#include <QTimer>
#include <QTime>
#include <QDateTime>

#ifdef USE_MOBILITY
#include <QSystemDeviceInfo>
#include <QSystemNetworkInfo>
QTM_USE_NAMESPACE
#endif

class SystemInfoProvider : public QObject
{
    Q_OBJECT
public:
    explicit SystemInfoProvider(QObject *parent = 0);
    ~SystemInfoProvider();

signals:

    void batteryLevelChanged( QVariant level );
    void networkLevelChanged( QVariant level );
    void wlanLevelChanged( QVariant level );
    void simStatusChanged( QVariant available );
    void wlanStatusChanged( QVariant active );
    void netNameChanged( QVariant active );
    void timeChanged( QVariant time );
    void batteryChargingChanged( QVariant charging );
    void bluetoothStateChanged( QVariant active );
    void profileChanged( QVariant profile );

public slots:

    void batteryLevelChange( int level );
    void btStateChanged( bool btOn );
#ifdef USE_MOBILITY
    void powerStateChanged( QSystemDeviceInfo::PowerState state );
    void networkSignalStrengthChanged( QSystemNetworkInfo::NetworkMode mode, int level );
    void networkStatusChanged(QSystemNetworkInfo::NetworkMode mode, QSystemNetworkInfo::NetworkStatus status );
    void networkNameChanged( QSystemNetworkInfo::NetworkMode mode, const QString &name );
    void currentProfileChanged( QSystemDeviceInfo::Profile profile );
#endif

    int batteryLevel();
    int networkLevel();
    int wlanLevel();
    bool simAvailable();
    bool wlanActive();
    bool btActive();
    QString currentTime();
    QString networkName();
    static QString versionInfo();
    static bool touchSupported();
    static bool landscapeDevice();
    bool batteryCharging();
    int currentProfile();
    static QString currentLanguage();
    static QString osVersion();
    static bool qtVersion474();
    static bool imeiAccepted();

    void timeOut();

private:

#ifdef USE_MOBILITY
    QSystemDeviceInfo *mDeviceInfo;
    QSystemNetworkInfo *mNetworkInfo;
#endif
    QTimer *mClockTimer;
};

#endif // SYSTEMINFOPROVIDER_H
