#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include <QObject>
#include <QNetworkSession>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>

class NetworkController : public QObject
{
    Q_OBJECT
public:
    explicit NetworkController(QObject *parent = 0);
    ~NetworkController();

signals:

    void connectingToWlan();
    void connectingToAny();
    void wlanConnectionReady();
    void connectionReady();
    void nonSupportedConnectionReady();
    void wlanConnectionLost();
    void connectionLost();
    void disconnecting();
    void noWLANsAvailable();
    void noConnectionsAvailable();

public slots:

    int openWlanConnection();
    int openAnyConnection();
    bool closeConnection();
    bool isConnected();
    QNetworkConfiguration currentConfiguration();
    QHostAddress currentAddress();

private slots:

    void listConfigurations();

    void configsUpdated();
    void connectionStateChanged( QNetworkSession::State state );
    void connectionOpened();
    void connectionError( QNetworkSession::SessionError error );
    bool readCurrentIP();
    bool activeWLANexists();
    bool activeConnectionExists();
    bool userChoiceIsDefault();
    bool wlansAvailableForUse();
    bool anyWlansAvailableForUse();
    bool connectionsAvailableForUse();
    void openSession();
    bool activeConfigIsWLAN();

private:
    QNetworkConfigurationManager *mNetworkConfigManager;
    QNetworkSession *mNetworkSession;
    QNetworkSession::State mPreviousConnectionState;
    QNetworkSession::State mCurrentConnectionState;
    bool mConfigsUpdated;
    bool mOnlineConfigsUpdated;
    bool mOpenRequested;
    bool mOnlineConfigsRequested;
    QHostAddress mCurrentIP;
    QNetworkConfiguration mCurrentNetworkConfig;
    int mConnectionType;
};

#endif // NETWORKCONTROLLER_H
