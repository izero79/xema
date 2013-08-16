#ifdef USE_MOBILITY
#include <QSystemNetworkInfo>
QTM_USE_NAMESPACE
#endif
#include "networkcontroller.h"
#include "xemaconstants.h"

NetworkController::NetworkController(QObject *parent) :
    QObject(parent),
    mNetworkConfigManager( 0 ),
    mNetworkSession( 0 ),
    mPreviousConnectionState( QNetworkSession::Invalid ),
    mCurrentConnectionState( QNetworkSession::Invalid ),
    mConfigsUpdated( false ),
    mOnlineConfigsUpdated( false ),
    mOpenRequested( false ),
    mOnlineConfigsRequested( false ),
    mCurrentIP(),
    mConnectionType(ANY)
{
    mNetworkConfigManager = new QNetworkConfigurationManager( this );
    connect(mNetworkConfigManager,SIGNAL(updateCompleted()),this,SLOT(configsUpdated()));
    long capas = mNetworkConfigManager->capabilities();
    QString capasNum;
    capasNum.setNum( capas );
    qDebug() << "Network Capabilities:" << capasNum;
    mNetworkConfigManager->updateConfigurations();
}

NetworkController::~NetworkController()
{
    qDebug() << "NetworkController::~NetworkController()";

    if( mNetworkSession != 0 )
    {
        mNetworkSession->close();
    }
}

bool NetworkController::closeConnection()
{
    qDebug() << "bool NetworkController::closeConnection()";
    mConfigsUpdated = false;
    mOpenRequested = false;
    mOnlineConfigsRequested = false;
    if( mNetworkSession != 0 )
    {
        mNetworkSession->disconnect();
        mNetworkSession->close();
        mPreviousConnectionState = QNetworkSession::Invalid;
        mCurrentConnectionState = QNetworkSession::Invalid;
        return true;
    }
    mPreviousConnectionState = QNetworkSession::Invalid;
    mCurrentConnectionState = QNetworkSession::Invalid;
    return false;
}

bool NetworkController::isConnected()
{
    qDebug() << "bool NetworkController::isConnected()";
    bool connected = activeConnectionExists();
    emit connectionReady();
    return connected;
}


void NetworkController::listConfigurations()
{
    QList<QNetworkConfiguration> netList = mNetworkConfigManager->allConfigurations();
    qDebug() << "void NetworkController::listConfigurations()" << "\n-----\n\n";
    qDebug() << "void NetworkController::listConfigurations()" << "Listing all configurations:";
    foreach( QNetworkConfiguration conf, netList )
    {
        qDebug() << "void NetworkController::listConfigurations()" << "Name:" << conf.name() << ", bearerType: " << conf.bearerType()  << ", typeName: " << conf.bearerTypeName() << ", state" << conf.state() << ", type: " << conf.type();

        if( conf.type() == QNetworkConfiguration::ServiceNetwork )
        {
            QList<QNetworkConfiguration> iaps = conf.children();
            qDebug() << "void NetworkController::listConfigurations()" << "Is SNAP, children count:" << iaps.size();
            foreach( QNetworkConfiguration iap, iaps )
            {
                qDebug() << "void NetworkController::listConfigurations()" << "Children: Name:" << iap.name() << ", bearerType: " << iap.bearerType()  << ", typeName: " << iap.bearerTypeName() << ", state" << iap.state() << ", type: " << iap.type();
            }
        }
        if( ( conf.state() & QNetworkConfiguration::Active ) == QNetworkConfiguration::Active &&
            conf.bearerType() == QNetworkConfiguration::BearerWLAN )
        {
            qDebug() << "void NetworkController::listConfigurations()" << "Name:" << conf.name() << "is Active WLAN!";
        }
    }
    qDebug() << "void NetworkController::listConfigurations()" << "\n-----\n\n";
}

void NetworkController::configsUpdated()
{
    qDebug() << "void NetworkController::configsUpdated()";
    if( mOnlineConfigsRequested == true )
    {
        qDebug() << "void NetworkController::configsUpdated() - check connected state again";
        mOnlineConfigsUpdated = true;
        connectionStateChanged( QNetworkSession::Connected );
        return;
    }
    mConfigsUpdated = true;
    if( mOpenRequested == true )
    {
        qDebug() << "void NetworkController::configsUpdated() - open connection";
        if (mConnectionType == ANY) {
            int rv = openAnyConnection();
            if( rv == NO_AVAILABLE_CONNECTIONS )
            {
                emit noConnectionsAvailable();
            }
        } else if (mConnectionType == WLAN) {
            int rv = openWlanConnection();
            if( rv == NO_AVAILABLE_WLANS )
            {
                emit noWLANsAvailable();
            }
        }
    }
}

void NetworkController::connectionStateChanged( QNetworkSession::State newState )
{
    qDebug() << "void NetworkController::connectionStateChanged( QNetworkSession::State newState ) - newState: " << newState;
    if( newState == QNetworkSession::Connected && mOnlineConfigsUpdated == false )
    {
        qDebug() << "void NetworkController::connectionStateChanged( QNetworkSession::State newState ) - update configs";
        mOnlineConfigsRequested = true;
        mNetworkConfigManager->updateConfigurations();
#ifdef Q_WS_SIMULATOR
        configsUpdated();
#endif
        return; // Not actually needed
    }
    mPreviousConnectionState = mCurrentConnectionState;
    mCurrentConnectionState = newState;

    if( mCurrentConnectionState == mPreviousConnectionState )
    {
        return;
    }


    switch( newState )
    {
        case QNetworkSession::Connecting:
        {
            qDebug() << "void NetworkController::connectionStateChanged( QNetworkSession::State newState ) - Connecting with config" << mCurrentNetworkConfig.name();
            break;
        }

        case QNetworkSession::Connected:
        {
            qDebug() << "void NetworkController::connectionStateChanged( QNetworkSession::State newState ) - Connected with config" << mCurrentNetworkConfig.name();
            bool ipOk = readCurrentIP();
            if (mConnectionType == WLAN) {
                bool wlanActive = activeConfigIsWLAN();
                if( wlanActive == true && ipOk == true )
                {
                    emit wlanConnectionReady();
                }
                else
                {
                    emit nonSupportedConnectionReady();
                    closeConnection();
                }
            } else if (mConnectionType == ANY) {
                if( ipOk == true )
                {
                    qDebug() << "void NetworkController::connectionStateChanged( QNetworkSession::State newState ) - Connection Ready";
                    emit connectionReady();
                }
            }
            break;
        }
        case QNetworkSession::Closing:
        {
            qDebug() << "void NetworkController::connectionStateChanged( QNetworkSession::State newState ) - Closing";
            emit disconnecting();
            closeConnection();
            break;
        }
        case QNetworkSession::Disconnected:
        {
            qDebug() << "void NetworkController::connectionStateChanged( QNetworkSession::State newState ) - Disconnected";
            if (mConnectionType == WLAN) {
                emit wlanConnectionLost();
            } else {
                emit connectionLost();
            }
            closeConnection();
            break;
        }
        default:
        {
            qDebug() << "void NetworkController::connectionStateChanged( QNetworkSession::State newState ) - State is other:" << mNetworkSession->state();
            if( mPreviousConnectionState == QNetworkSession::Connected )
            {
                qDebug() << "void NetworkController::connectionStateChanged( QNetworkSession::State newState )  - State is other - connectionLost";
                if (mConnectionType == WLAN) {
                    emit wlanConnectionLost();
                } else {
                    emit connectionLost();
                }
                closeConnection();
            }
            break;
        }
    }
}

QNetworkConfiguration NetworkController::currentConfiguration()
{
    qDebug() << "QNetworkConfiguration NetworkController::currentConfiguration() - mCurrentNetworkConfig is valid:" << mCurrentNetworkConfig.isValid();
    return mCurrentNetworkConfig;
}

QHostAddress NetworkController::currentAddress()
{
    qDebug() << "QHostAddress NetworkController::currentAddress() - " << mCurrentIP;
    return mCurrentIP;
}

void NetworkController::connectionOpened()
{
    qDebug() << "void NetworkController::connectionOpened()";
    connectionStateChanged( QNetworkSession::Connected );
}

void NetworkController::connectionError( QNetworkSession::SessionError error )
{
    qDebug() << "void NetworkController::connectionError( QNetworkSession::SessionError error )" << "connectionError:" << error;
}

bool NetworkController::readCurrentIP()
{
    QSystemNetworkInfo networkInfo;
    QNetworkInterface wlanIf = networkInfo.interfaceForMode( QSystemNetworkInfo::WlanMode );
    QNetworkInterface activeIf = mNetworkSession->interface();
    qDebug() << "void NetworkController::readCurrentIP()";
    if( wlanIf.isValid() == true )
    {
        qDebug() << "void NetworkController::readCurrentIP(), wlanIf:" << wlanIf.name();
        QList<QNetworkAddressEntry> addrs = wlanIf.addressEntries();
        foreach( QNetworkAddressEntry addr, addrs )
        {
            if( addr.ip().protocol() != QAbstractSocket::IPv6Protocol &&
                addr.ip() != QHostAddress::LocalHost &&
                !(addr.ip().toString().startsWith( "169" ))    )
            {
                qDebug() << "void NetworkController::readCurrentIP(), wlanIf IP" << wlanIf.name() << addr.ip().toString();
                if( addr.ip().isNull() == false )
                {
                    mCurrentIP = addr.ip();
                    return true;
                }
            }
        }
    }
    if( activeIf.isValid() == true )
    {
        qDebug() << "void NetworkController::readCurrentIP(), activeIf:" << activeIf.name();
        QList<QNetworkAddressEntry> addrs = activeIf.addressEntries();
        foreach( QNetworkAddressEntry addr, addrs )
        {
            if( addr.ip().protocol() != QAbstractSocket::IPv6Protocol &&
                addr.ip() != QHostAddress::LocalHost &&
                !(addr.ip().toString().startsWith( "169" ))    )
            {
                if( addr.ip().toString().startsWith( "192.168" ) ||
                    addr.ip().toString().startsWith( "172." ) ||
                    addr.ip().toString().startsWith( "10." ) )
                {
                    qDebug() << "void NetworkController::readCurrentIP(), using activeIf" << activeIf.name() << addr.ip().toString();
                    if( addr.ip().isNull() == false )
                    {
                        mCurrentIP = addr.ip();
                        return true;
                    }
                }
            }
        }
        qDebug() << "void NetworkController::readCurrentIP(), Now any IP will do";
        foreach( QNetworkAddressEntry addr, addrs )
        {
            if( addr.ip().protocol() != QAbstractSocket::IPv6Protocol &&
                addr.ip() != QHostAddress::LocalHost &&
                !(addr.ip().toString().startsWith( "169" ))    )
            {
                qDebug() << "void NetworkController::readCurrentIP(), using activeIf" << activeIf.name() << addr.ip().toString();
                if( addr.ip().isNull() == false )
                {
                    mCurrentIP = addr.ip();
                    return true;
                }
            }
        }
    }
    qDebug() << "void NetworkController::readCurrentIP(), no suitable IP address";
    return false;
}

int NetworkController::openWlanConnection()
{
    qDebug() << "int NetworkController::openWlanConnection()";
    mConnectionType = WLAN;
    if( mConfigsUpdated == false )
    {
        qDebug() << "int NetworkController::openWlanConnection() - update configs";
        mOpenRequested = true;
        mNetworkConfigManager->updateConfigurations();
#ifdef Q_WS_SIMULATOR
        configsUpdated();
#endif
        return CONNECTING;
    }
    if( mNetworkSession != 0 )
    {
        qDebug() << "int NetworkController::openWlanConnection() - network session already exists";
        mNetworkSession->disconnect();
        if( mNetworkSession->isOpen() )
        {
            mNetworkSession->close();
        }
        delete mNetworkSession;
        mNetworkSession = 0;
    }

    emit connectingToWlan();

    bool suitableConfigFound = false;
    // Check if wlan configuration already active
    suitableConfigFound = activeWLANexists();

    // Check if default config is user choice
    if( suitableConfigFound == false )
    {
        suitableConfigFound = userChoiceIsDefault();
    }

    // Check if default config includes available wlans
    if( suitableConfigFound == false )
    {
        suitableConfigFound = wlansAvailableForUse();
    }

    // Check if any config includes available wlans
    if( suitableConfigFound == false )
    {
        suitableConfigFound = anyWlansAvailableForUse();
    }

    // No suitable configs found
    if( suitableConfigFound == false )
    {
        qDebug() << "int NetworkController::openWlanConnection() - no available wlans";
        emit noWLANsAvailable();
        return NO_AVAILABLE_WLANS;
    }

    openSession();

    return CONNECTING;
}

int NetworkController::openAnyConnection()
{
    qDebug() << "int NetworkController::openAnyConnection()";
    mConnectionType = ANY;

    if( mConfigsUpdated == false )
    {
        qDebug() << "int NetworkController::openAnyConnection() - update configs";
        mOpenRequested = true;
        mNetworkConfigManager->updateConfigurations();
#ifdef Q_WS_SIMULATOR
        configsUpdated();
#endif
        return CONNECTING;
    }
    if( mNetworkSession != 0 )
    {
        qDebug() << "int NetworkController::openAnyConnection() - network session already exists";
        mNetworkSession->disconnect();
        if( mNetworkSession->isOpen() )
        {
            mNetworkSession->close();
        }
        delete mNetworkSession;
        mNetworkSession = 0;
    }

    emit connectingToAny();

    bool suitableConfigFound = false;
    // Check if any configuration already active
    suitableConfigFound = activeConnectionExists();

    // Check if default config is user choice
    if( suitableConfigFound == false )
    {
        suitableConfigFound = userChoiceIsDefault();
    }

    // Check if default config includes available wlans
    if( suitableConfigFound == false )
    {
        suitableConfigFound = connectionsAvailableForUse();
    }

    // No suitable configs found
    if( suitableConfigFound == false )
    {
        qDebug() << "int NetworkController::openAnyConnection() - no available connection";
        emit noConnectionsAvailable();
        return NO_AVAILABLE_CONNECTIONS;
    }

    openSession();

    return CONNECTING;
}

bool NetworkController::activeWLANexists()
{
    qDebug() << "bool NetworkController::activeWLANexists()";

    QNetworkConfiguration activeWLANConfig;

    QList<QNetworkConfiguration> netList = mNetworkConfigManager->allConfigurations();

    foreach( QNetworkConfiguration conf, netList )
    {
        if( ( conf.state() & QNetworkConfiguration::Active ) == QNetworkConfiguration::Active &&
                ( conf.bearerType() == QNetworkConfiguration::BearerWLAN ||
            ( QString::compare( conf.bearerTypeName(), "WLAN", Qt::CaseInsensitive ) == 0 ) )
          )
        {
            activeWLANConfig = conf;
        }
    }

    if( activeWLANConfig.isValid() == true )
    {
        qDebug() << "bool NetworkController::activeWLANexists() - active config" << activeWLANConfig.name();
        mCurrentNetworkConfig = activeWLANConfig;
        return true;
    }
    qDebug() << "bool NetworkController::activeWLANexists() - no active wlans";
    return false;
}

bool NetworkController::activeConnectionExists()
{
    qDebug() << "bool NetworkController::activeConnectionExists()";

    QNetworkConfiguration activeConfig;

    QList<QNetworkConfiguration> netList = mNetworkConfigManager->allConfigurations();

    foreach( QNetworkConfiguration conf, netList )
    {
        if (( conf.state() & QNetworkConfiguration::Active ) == QNetworkConfiguration::Active)
        {
            activeConfig = conf;
        }
    }

    if( activeConfig.isValid() == true )
    {
        qDebug() << "bool NetworkController::activeConnectionExists() - active config" << activeConfig.name();
        mCurrentNetworkConfig = activeConfig;
        return true;
    }
    qDebug() << "bool NetworkController::activeConnectionExists() - no active configs";
    return false;
}

bool NetworkController::userChoiceIsDefault()
{
    qDebug() << "bool NetworkController::userChoiceIsDefault()";
    QNetworkConfiguration defaultConfig = mNetworkConfigManager->defaultConfiguration();

    if( defaultConfig.type() == QNetworkConfiguration::UserChoice )
    {
        qDebug() << "bool NetworkController::userChoiceIsDefault() - true";
        mCurrentNetworkConfig = defaultConfig;
        return true;
    }
    qDebug() << "bool NetworkController::userChoiceIsDefault() - false";
    return false;
}

bool NetworkController::wlansAvailableForUse()
{
    qDebug() << "bool NetworkController::wlansAvailableForUse()";
    QNetworkConfiguration defaultConfig = mNetworkConfigManager->defaultConfiguration();

#ifdef Q_WS_SIMULATOR
    qDebug() << "bool NetworkController::wlansAvailableForUse() - Simulator uses defaultConfig";
    mCurrentNetworkConfig = defaultConfig;
    return true;
#endif

    if( defaultConfig.type() == QNetworkConfiguration::InternetAccessPoint )
    {
        qDebug() << "bool NetworkController::wlansAvailableForUse() - default is a IAP" << defaultConfig.bearerType() << defaultConfig.bearerTypeName();
        if( defaultConfig.bearerType() == QNetworkConfiguration::BearerWLAN ||
                ( QString::compare( defaultConfig.bearerTypeName(), "WLAN", Qt::CaseInsensitive ) == 0 ) )
        {
            qDebug() << "bool NetworkController::wlansAvailableForUse() - IAP type is WLAN";
            if( ( ( defaultConfig.state() & QNetworkConfiguration::Active ) == QNetworkConfiguration::Active ) ||
                ( ( defaultConfig.state() & QNetworkConfiguration::Discovered ) == QNetworkConfiguration::Discovered ) )
            {
                qDebug() << "bool NetworkController::wlansAvailableForUse() - IAP is Active or Discovered";
                mCurrentNetworkConfig = defaultConfig;
                return true;
            }
            else
            {
                qDebug() << "bool NetworkController::wlansAvailableForUse() - IAP is not available";
                return false;
            }

        }
        else
        {
            qDebug() << "bool NetworkController::wlansAvailableForUse() - IAP type is not WLAN";
        }
        return false;
    }
    else if( defaultConfig.type() == QNetworkConfiguration::ServiceNetwork )
    {
        qDebug() << "bool NetworkController::wlansAvailableForUse() - default is a SNAP";

        // Check the IAPs in priority order, first discovered is now only interesting as this is going to be used
        QList<QNetworkConfiguration> iaps = defaultConfig.children();

        foreach( QNetworkConfiguration iap, iaps )
        {

            qDebug() << "bool NetworkController::wlansAvailableForUse() - IAP Name:" << iap.name() << ", bearerType: " << iap.bearerType()  << ", typeName: " << iap.bearerTypeName() << ", state" << iap.state();

            if( ( iap.state() & QNetworkConfiguration::Active ) == QNetworkConfiguration::Active ||
                ( iap.state() & QNetworkConfiguration::Discovered ) == QNetworkConfiguration::Discovered )
            {
                qDebug() << "bool NetworkController::wlansAvailableForUse() - Active or Discovered IAP";
                if( iap.bearerType() == QNetworkConfiguration::BearerWLAN ||
                        ( QString::compare( iap.bearerTypeName(), "WLAN", Qt::CaseInsensitive ) == 0 ) )
                {
                    qDebug() << "bool NetworkController::wlansAvailableForUse() - Active or Discovered IAP type is WLAN, using" << iap.name();
                    mCurrentNetworkConfig = defaultConfig;
                    return true;
                }
                else
                {
                    qDebug() << "bool NetworkController::wlansAvailableForUse() - Active or Discovered IAP type is not WLAN, cannot use" << iap.name();
                    break;
                }
            }
        }

        qDebug() << "bool NetworkController::wlansAvailableForUse() - default is a SNAP, but had non-WLAN prioritized, check again";

        // Check the IAPs in again priority order, now check if there is any available wlan that could be used
        foreach( QNetworkConfiguration iap, iaps )
        {

            qDebug() << "bool NetworkController::wlansAvailableForUse() - IAP Name:" << iap.name() << ", bearerType: " << iap.bearerType()  << ", typeName: " << iap.bearerTypeName() << ", state" << iap.state();

            if( ( iap.state() & QNetworkConfiguration::Active ) == QNetworkConfiguration::Active ||
                ( iap.state() & QNetworkConfiguration::Discovered ) == QNetworkConfiguration::Discovered )
            {
                qDebug() << "bool NetworkController::wlansAvailableForUse() - Active or Discovered IAP";
                if( iap.bearerType() == QNetworkConfiguration::BearerWLAN ||
                        ( QString::compare( iap.bearerTypeName(), "WLAN", Qt::CaseInsensitive ) == 0 ) )
                {
                    qDebug() << "bool NetworkController::wlansAvailableForUse() - Active or Discovered IAP type is WLAN, using" << iap.name();
                    mCurrentNetworkConfig = iap;
                    return true;
                }
            }
        }
        qDebug() << "bool NetworkController::wlansAvailableForUse() - not suitable config found from SNAP";
        return false;
    }
    else
    {
        qDebug() << "bool NetworkController::wlansAvailableForUse() - not suitable config found, defaultConfig not valid";
        return false;
    }
}

bool NetworkController::connectionsAvailableForUse()
{
    qDebug() << "bool NetworkController::connectionsAvailableForUse()";
    QNetworkConfiguration defaultConfig = mNetworkConfigManager->defaultConfiguration();

#ifdef Q_WS_SIMULATOR
    qDebug() << "bool NetworkController::connectionsAvailableForUse() - Simulator uses defaultConfig";
    mCurrentNetworkConfig = defaultConfig;
    return true;
#endif

    if( defaultConfig.type() == QNetworkConfiguration::InternetAccessPoint )
    {
        qDebug() << "bool NetworkController::connectionsAvailableForUse() - default is a IAP" << defaultConfig.bearerType() << defaultConfig.bearerTypeName();
        if( ( ( defaultConfig.state() & QNetworkConfiguration::Active ) == QNetworkConfiguration::Active ) ||
            ( ( defaultConfig.state() & QNetworkConfiguration::Discovered ) == QNetworkConfiguration::Discovered ) )
        {
            qDebug() << "bool NetworkController::connectionsAvailableForUse() - IAP is Active or Discovered";
            mCurrentNetworkConfig = defaultConfig;
            return true;
        }
        else
        {
            qDebug() << "bool NetworkController::connectionsAvailableForUse() - IAP is not available";
            return false;
        }
    }
    else if( defaultConfig.type() == QNetworkConfiguration::ServiceNetwork )
    {
        qDebug() << "bool NetworkController::connectionsAvailableForUse() - default is a SNAP";

        // Check the IAPs in priority order, first discovered is now only interesting as this is going to be used
        QList<QNetworkConfiguration> iaps = defaultConfig.children();

        foreach( QNetworkConfiguration iap, iaps )
        {

            qDebug() << "bool NetworkController::connectionsAvailableForUse() - IAP Name:" << iap.name() << ", bearerType: " << iap.bearerType()  << ", typeName: " << iap.bearerTypeName() << ", state" << iap.state();

            if( ( iap.state() & QNetworkConfiguration::Active ) == QNetworkConfiguration::Active ||
                ( iap.state() & QNetworkConfiguration::Discovered ) == QNetworkConfiguration::Discovered )
            {
                qDebug() << "bool NetworkController::connectionsAvailableForUse() - Active or Discovered IAP, using" << iap.name();
                mCurrentNetworkConfig = defaultConfig;
                return true;
            }
        }

        qDebug() << "bool NetworkController::connectionsAvailableForUse() - default is a SNAP, but had non-WLAN prioritized, check again";

        // Check the IAPs in again priority order, now check if there is any available wlan that could be used
        foreach( QNetworkConfiguration iap, iaps )
        {

            qDebug() << "bool NetworkController::connectionsAvailableForUse() - IAP Name:" << iap.name() << ", bearerType: " << iap.bearerType()  << ", typeName: " << iap.bearerTypeName() << ", state" << iap.state();

            if( ( iap.state() & QNetworkConfiguration::Active ) == QNetworkConfiguration::Active ||
                ( iap.state() & QNetworkConfiguration::Discovered ) == QNetworkConfiguration::Discovered )
            {
                qDebug() << "bool NetworkController::connectionsAvailableForUse() - Active or Discovered IAP, using" << iap.name();
                mCurrentNetworkConfig = iap;
                return true;
            }
        }
        qDebug() << "bool NetworkController::connectionsAvailableForUse() - not suitable config found from SNAP";
        return false;
    }
    else
    {
        qDebug() << "bool NetworkController::connectionsAvailableForUse() - not suitable config found, defaultConfig not valid";
        return false;
    }
}

bool NetworkController::anyWlansAvailableForUse()
{
    qDebug() << "bool NetworkController::anyWlansAvailableForUse()";
    QList<QNetworkConfiguration> allConfigs = mNetworkConfigManager->allConfigurations();

    qDebug() << "bool NetworkController::anyWlansAvailableForUse() - checking all active IAPs";

    foreach( QNetworkConfiguration config, allConfigs )
    {
        if( config.type() == QNetworkConfiguration::InternetAccessPoint )
        {
            if( config.bearerType() == QNetworkConfiguration::BearerWLAN ||
                    ( QString::compare( config.bearerTypeName(), "WLAN", Qt::CaseInsensitive ) == 0 ) )
            {
                qDebug() << "bool NetworkController::anyWlansAvailableForUse() - WLAN IAP" << config.name() << config.bearerTypeName();
                if( ( ( config.state() & QNetworkConfiguration::Active ) == QNetworkConfiguration::Active ) )
                {
                    qDebug() << "bool NetworkController::anyWlansAvailableForUse() - IAP is Active, using" << config.name();
                    mCurrentNetworkConfig = config;
                    return true;
                }
            }
        }
    }

    qDebug() << "bool NetworkController::anyWlansAvailableForUse() - checking all available IAPs";

    foreach( QNetworkConfiguration config, allConfigs )
    {
        if( config.type() == QNetworkConfiguration::InternetAccessPoint )
        {
            if( config.bearerType() == QNetworkConfiguration::BearerWLAN ||
                    ( QString::compare( config.bearerTypeName(), "WLAN", Qt::CaseInsensitive ) == 0 ) )
            {
                qDebug() << "bool NetworkController::anyWlansAvailableForUse() - WLAN IAP" << config.name() << config.bearerTypeName();
                if( ( ( config.state() & QNetworkConfiguration::Active ) == QNetworkConfiguration::Active ) ||
                    ( ( config.state() & QNetworkConfiguration::Discovered ) == QNetworkConfiguration::Discovered ) )
                {
                    qDebug() << "bool NetworkController::anyWlansAvailableForUse() - IAP is Active or Discovered, using" << config.name();
                    mCurrentNetworkConfig = config;
                    return true;
                }
            }
        }
    }
    qDebug() << "bool NetworkController::anyWlansAvailableForUse() - not suitable config found, no wlans at all";
    return false;
}

void NetworkController::openSession()
{
    qDebug() << "bool NetworkController::openSession() - using" << mCurrentNetworkConfig.name();
    mNetworkSession = new QNetworkSession( mCurrentNetworkConfig, this );
    connect(mNetworkSession,SIGNAL(stateChanged(QNetworkSession::State)),this,SLOT(connectionStateChanged(QNetworkSession::State)));
    connect(mNetworkSession,SIGNAL(opened()),this,SLOT(connectionOpened()));
    connect(mNetworkSession,SIGNAL(error(QNetworkSession::SessionError)),this,SLOT(connectionError(QNetworkSession::SessionError)));
    mNetworkSession->open();
}

bool NetworkController::activeConfigIsWLAN()
{
    qDebug() << "bool NetworkController::activeConfigIsWLAN()";
    QSystemNetworkInfo networkInfo;
    QNetworkInterface wlanIf = networkInfo.interfaceForMode( QSystemNetworkInfo::WlanMode );
    QNetworkInterface activeIf = mNetworkSession->interface();

    qDebug() << "bool NetworkController::activeConfigIsWLAN(), wlanIf" << wlanIf << "activeIf" << activeIf;

    if( wlanIf.isValid() && wlanIf.name() == activeIf.name() )
    {
        qDebug() << "bool NetworkController::activeConfigIsWLAN(), activeIf and wlanIf match";
        return true;
    }

#ifdef Q_WS_SIMULATOR
    qDebug() << "bool NetworkController::activeConfigIsWLAN() - Simulator";
    return true;
#endif

    QNetworkConfiguration activeConfig = mNetworkSession->configuration();

    if( activeConfig.type() == QNetworkConfiguration::InternetAccessPoint )
    {
        qDebug() << "bool NetworkController::activeConfigIsWLAN() - activeConfig is a IAP" << activeConfig.bearerType() << activeConfig.bearerTypeName();
        if( activeConfig.bearerType() == QNetworkConfiguration::BearerWLAN ||
                ( QString::compare( activeConfig.bearerTypeName(), "WLAN", Qt::CaseInsensitive ) == 0 ) )
        {
            qDebug() << "bool NetworkController::activeConfigIsWLAN() - activeConfig IAP type is WLAN";
            return true;
        }
        else
        {
            qDebug() << "bool NetworkController::activeConfigIsWLAN() - activeConfig IAP type is not WLAN";
            return false;
        }
    }
    else if( activeConfig.type() == QNetworkConfiguration::ServiceNetwork )
    {
        qDebug() << "bool NetworkController::activeConfigIsWLAN() - activeConfig is a SNAP";

        // Check the IAPs in priority order, first discovered is now only interesting as this is going to be used
        QList<QNetworkConfiguration> iaps = activeConfig.children();

        foreach( QNetworkConfiguration iap, iaps )
        {

            qDebug() << "bool NetworkController::activeConfigIsWLAN() - activeConfig IAP Name:" << iap.name() << ", bearerType: " << iap.bearerType()  << ", typeName: " << iap.bearerTypeName() << ", state" << iap.state();

            if( ( iap.state() & QNetworkConfiguration::Active ) == QNetworkConfiguration::Active ||
                ( iap.state() & QNetworkConfiguration::Discovered ) == QNetworkConfiguration::Discovered )
            {
                qDebug() << "bool NetworkController::activeConfigIsWLAN() - Active or Discovered IAP";
                if( iap.bearerType() == QNetworkConfiguration::BearerWLAN ||
                        ( QString::compare( iap.bearerTypeName(), "WLAN", Qt::CaseInsensitive ) == 0 ) )
                {
                    qDebug() << "bool NetworkController::activeConfigIsWLAN() - activeConfig type is WLAN, using" << iap.name();
                    return true;
                }
            }
        }
    }

    else if( activeConfig.type() == QNetworkConfiguration::UserChoice )
    {
        qDebug() << "bool NetworkController::activeConfigIsWLAN() - activeConfig is user choice";
    }

    if( activeIf.name().contains( "wlan", Qt::CaseInsensitive ) == true )
    {
        qDebug() << "bool NetworkController::activeConfigIsWLAN() - activeIf name contains WLAN";
        return true;
    }
    qDebug() << "bool NetworkController::activeConfigIsWLAN() - no active WLAN found";
    return false;
}
