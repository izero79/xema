#include <QDebug>
#include <QDir>
#include <QNetworkInterface>
#include "systeminfoprovider.h"

#ifdef USE_MOBILITY
#include <QSystemInfo>
#endif

SystemInfoProvider::SystemInfoProvider(QObject *parent) :
    QObject(parent),
#ifdef USE_MOBILITY
    mDeviceInfo( 0 ),
    mNetworkInfo( 0 ),
#endif
    mClockTimer( 0 )
{
#ifdef USE_MOBILITY
    mDeviceInfo = new QSystemDeviceInfo( this );
    connect( mDeviceInfo, SIGNAL(batteryLevelChanged(int)),
            this, SLOT(batteryLevelChange(int)));
    connect( mDeviceInfo, SIGNAL(powerStateChanged(QSystemDeviceInfo::PowerState)),
            this, SLOT(powerStateChanged(QSystemDeviceInfo::PowerState)));
    connect( mDeviceInfo, SIGNAL(bluetoothStateChanged(bool)),
            this,SLOT(btStateChanged(bool)));
    connect( mDeviceInfo, SIGNAL(currentProfileChanged(QSystemDeviceInfo::Profile)),
            this,SLOT(currentProfileChanged(QSystemDeviceInfo::Profile)));
#if !defined( Q_OS_SYMBIAN ) || defined( BUILDFORSIGNED )

    mNetworkInfo = new QSystemNetworkInfo( this );

    connect( mNetworkInfo, SIGNAL(networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode,int)),
             this, SLOT(networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode,int)));

    connect( mNetworkInfo, SIGNAL(networkStatusChanged(QSystemNetworkInfo::NetworkMode,QSystemNetworkInfo::NetworkStatus)),
             this,SLOT(networkStatusChanged(QSystemNetworkInfo::NetworkMode,QSystemNetworkInfo::NetworkStatus)));

    connect( mNetworkInfo, SIGNAL(networkNameChanged(QSystemNetworkInfo::NetworkMode,QString)),
             this,SLOT(networkNameChanged(QSystemNetworkInfo::NetworkMode,QString)));
#endif
#endif
    mClockTimer = new QTimer( this );
    mClockTimer->setInterval( 1000 );
    connect( mClockTimer, SIGNAL(timeout()), this, SLOT(timeOut()));
    mClockTimer->start();
}

SystemInfoProvider::~SystemInfoProvider()
{
    mClockTimer->stop();
}
int SystemInfoProvider::batteryLevel()
{
    int level = -1;
#ifdef USE_MOBILITY
    int batStatus = mDeviceInfo->batteryStatus();
    if( batStatus == QSystemDeviceInfo::NoBatteryLevel )
    {
        return -1;
    }
    level = mDeviceInfo->batteryLevel();
#else
    return level;
#endif
    int newLevel = ( level / 10 );
    int correctedLevel;
    switch( newLevel )
    {
    case 10:
    case 9:
        correctedLevel = 100;
        break;
    case 8:
        correctedLevel = 90;
        break;
    case 7:
        correctedLevel = 80;
        break;
    case 6:
        correctedLevel = 70;
        break;
    case 5:
        correctedLevel = 60;
        break;
    case 4:
        correctedLevel = 50;
        break;
    case 3:
        correctedLevel = 40;
        break;
    case 2:
        correctedLevel = 30;
        break;
    case 1:
        correctedLevel = 20;
        break;
    case 0:
        if( ( level % 10 ) != 0 )
        {
            correctedLevel = 10;
        }
        else
        {
            correctedLevel = 0;
        }
        break;
    default:
        correctedLevel = 100;
    }
    return correctedLevel;
}

int SystemInfoProvider::networkLevel()
{
    int level = -1;
#ifdef USE_MOBILITY
    if( mNetworkInfo == 0 )
    {
        return -1;
    }
    level = mNetworkInfo->networkSignalStrength( QSystemNetworkInfo::GsmMode );
#else
    return level;
#endif
    int newLevel = ( level / 10 );
    int correctedLevel;
    switch( newLevel )
    {
    case 10:
    case 9:
    case 8:
        correctedLevel = 100;
        break;
    case 7:
    case 6:
        correctedLevel = 80;
        break;
    case 5:
    case 4:
        correctedLevel = 60;
        break;
    case 3:
    case 2:
        correctedLevel = 40;
        break;
    case 1:
        correctedLevel = 20;
        break;
    case 0:
        correctedLevel = 0;
        break;
    default:
        correctedLevel = 100;
        break;
    }
    return correctedLevel;
}

int SystemInfoProvider::wlanLevel()
{
    int level = -1;
#ifdef USE_MOBILITY
    if( mNetworkInfo == 0 )
    {
        return 100;
    }
    level = mNetworkInfo->networkSignalStrength( QSystemNetworkInfo::WlanMode );
#else
    return level;
#endif
    int newLevel = ( level / 10 );
    int correctedLevel;
    switch( newLevel )
    {
    case 10:
    case 9:
    case 8:
    case 7:
        correctedLevel = 100;
        break;
    case 6:
    case 5:
    case 4:
        correctedLevel = 60;
        break;
    case 3:
    case 2:
    case 1:
        correctedLevel = 30;
        break;
    case 0:
        correctedLevel = 0;
        break;
    default:
        correctedLevel = 100;
        break;
    }
    return correctedLevel;
}

bool SystemInfoProvider::simAvailable()
{
    int simStatus =  1;
#ifdef USE_MOBILITY
    simStatus = mDeviceInfo->simStatus();
#endif
    if( simStatus == 0 )
    {
        return false;
    }
    return true;
}

bool SystemInfoProvider::wlanActive()
{
#ifdef USE_MOBILITY
    if( mNetworkInfo == 0 )
    {
        return false;
    }
    QNetworkInterface wlanIf = mNetworkInfo->interfaceForMode( QSystemNetworkInfo::WlanMode );
    if( wlanIf.isValid() == true )
    {
        int flags = wlanIf.flags();
        if( ( flags & 1 ) == 1 )
        {
            return true;
        }
    }
    else
    {
        return false;
    }
#endif
    return false;
}

bool SystemInfoProvider::btActive()
{
#ifdef USE_MOBILITY
    #ifndef Q_WS_MAEMO_5
    return mDeviceInfo->currentBluetoothPowerState();
    #else
    // For some reason Maemo5's Mobility API doest have method implemented.
    // So lets read the state from config file
    QString filePath = "/var/lib/bluetooth";
    QDir dir( filePath );
    QStringList nameFilter;
    nameFilter << "*:*:*:*:*:*";
    QStringList files = dir.entryList( nameFilter );
    if( files.length() > 0 )
    {
        filePath.append( "/" );
        filePath.append( files.at( 0 ) );
        filePath.append( "/config" );
        QFile file( filePath );
        if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
            return false;
        }
        QByteArray line;
        QString mode;
        do
        {
            line = file.readLine();
            if( line.contains( "mode ") && !line.contains( "onmode ") )
            {
                mode = line;
                mode.remove( 0, 5 );
                if( !mode.contains( "off" ) )
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        while( line.length() > 0 );
        return true;
    }
    return false;

    #endif
#endif
    return false;
}

void SystemInfoProvider::batteryLevelChange( int level )
{
    int newLevel = ( level / 10 );
    int correctedLevel;
    switch( newLevel )
    {
    case 10:
    case 9:
        correctedLevel = 100;
        break;
    case 8:
        correctedLevel = 90;
        break;
    case 7:
        correctedLevel = 80;
        break;
    case 6:
        correctedLevel = 70;
        break;
    case 5:
        correctedLevel = 60;
        break;
    case 4:
        correctedLevel = 50;
        break;
    case 3:
        correctedLevel = 40;
        break;
    case 2:
        correctedLevel = 30;
        break;
    case 1:
        correctedLevel = 20;
        break;
    case 0:
        if( ( level % 10 ) != 0 )
        {
            correctedLevel = 10;
        }
        else
        {
            correctedLevel = 0;
        }
        break;
    default:
        correctedLevel = 100;
    }

    emit batteryLevelChanged( QVariant( correctedLevel ) );
}

void SystemInfoProvider::btStateChanged( bool btOn )
{
    emit bluetoothStateChanged( QVariant( btOn ) );
}

#ifdef USE_MOBILITY
void SystemInfoProvider::powerStateChanged( QSystemDeviceInfo::PowerState state )
{
    if( state == QSystemDeviceInfo::WallPowerChargingBattery )
    {
        emit batteryChargingChanged( QVariant( true ) );
        return;
    }
    emit batteryChargingChanged( QVariant( false ) );
}

void SystemInfoProvider::networkSignalStrengthChanged( QSystemNetworkInfo::NetworkMode mode, int level )
{
    int newLevel = ( level / 10 );
    if( mode == QSystemNetworkInfo::UnknownMode ||
        mode == QSystemNetworkInfo::GsmMode ||
        mode == QSystemNetworkInfo::CdmaMode ||
        mode == QSystemNetworkInfo::WcdmaMode ||
        mode == QSystemNetworkInfo::WimaxMode )
    {
        int correctedLevel;
        switch( newLevel )
        {
        case 10:
        case 9:
        case 8:
            correctedLevel = 100;
            break;
        case 7:
        case 6:
            correctedLevel = 80;
            break;
        case 5:
        case 4:
            correctedLevel = 60;
            break;
        case 3:
        case 2:
            correctedLevel = 40;
            break;
        case 1:
            correctedLevel = 20;
            break;
        case 0:
            correctedLevel = 0;
            break;
        default:
            correctedLevel = 100;
            break;
        }
        emit networkLevelChanged( QVariant( correctedLevel ) );
    }
    else if( mode == QSystemNetworkInfo::WlanMode )
    {
        int correctedLevel;
        switch( newLevel )
        {
        case 10:
        case 9:
        case 8:
        case 7:
        case 6:
            correctedLevel = 100;
            break;
        case 5:
        case 4:
        case 3:
            correctedLevel = 60;
            break;
        case 2:
        case 1:
            correctedLevel = 30;
            break;
        case 0:
            correctedLevel = 0;
            break;
        default:
            correctedLevel = 100;
            break;
        }
        emit wlanLevelChanged( QVariant( correctedLevel ) );
    }
}

void SystemInfoProvider::networkStatusChanged(QSystemNetworkInfo::NetworkMode mode, QSystemNetworkInfo::NetworkStatus status )
{
    if( mode == QSystemNetworkInfo::WlanMode )
    {
        switch( status )
        {
        case QSystemNetworkInfo::Searching:
        case QSystemNetworkInfo::Connected:
            emit wlanStatusChanged( QVariant( true ) );
            break;
        default:
            emit wlanStatusChanged( QVariant( false ) );
            break;
        }
    }
}

void SystemInfoProvider::networkNameChanged( QSystemNetworkInfo::NetworkMode mode, const QString &name )
{
    if( mode == QSystemNetworkInfo::GsmMode ||
        mode == QSystemNetworkInfo::CdmaMode ||
        mode == QSystemNetworkInfo::WcdmaMode ||
        mode == QSystemNetworkInfo::WimaxMode )
    {
        emit netNameChanged( name );
    }
}

void SystemInfoProvider::currentProfileChanged( QSystemDeviceInfo::Profile profile )
{
    emit profileChanged( QVariant( profile ) );
}

#endif

QString SystemInfoProvider::currentTime()
{
    // Not sure that this works with all locales, check
    QLocale local;
    QString format = local.timeFormat( QLocale::ShortFormat );
    QTime current = QTime::currentTime();
    int posOfSecs = format.indexOf( "ss" ) - 1;
    if( posOfSecs >= 0 )
    {
        format = format.remove( posOfSecs, 3 );
    }
    posOfSecs = format.indexOf( "s" ) - 1;
    if( posOfSecs >= 0 )
    {
        format = format.remove( posOfSecs, 2 );
    }
    QString timeString = current.toString( format );
    return timeString;
}

QString SystemInfoProvider::networkName()
{
#ifdef USE_MOBILITY
    if( mNetworkInfo == 0 )
    {
        return QString();
    }
    QString name = mNetworkInfo->networkName( QSystemNetworkInfo::GsmMode );
    return name;
#endif
    return QString();
}

void SystemInfoProvider::timeOut()
{
    emit timeChanged( currentTime() );
}

QString SystemInfoProvider::versionInfo()
{
    QString info;
#ifdef USE_MOBILITY
    QSystemDeviceInfo devInfo;
    info.append( "Manufacturer: ");
    info.append( devInfo.manufacturer() );
    info.append( "\n");
    info.append( "Model: ");
    info.append( devInfo.model() );
    info.append( "\n");
    info.append( "ProductName: ");
    info.append( devInfo.productName() );
    info.append( "\n");
    info.append( "InputMethodTypes: ");
    long types = devInfo.inputMethodType();
    QString num;
    num.setNum( types );
    info.append( num );
    info.append( "\n\n");
    QSystemInfo sysInfo;
    info.append( "SysInfo version QSystemInfo::Os: ");
    info.append( sysInfo.version( QSystemInfo::Os ) );
    info.append( "\n");
    info.append( "SysInfo version QSystemInfo::QtCore: ");
    info.append( sysInfo.version( QSystemInfo::QtCore ) );
    info.append( "\n");
    info.append( "SysInfo version QSystemInfo::Firmware: ");
    info.append( sysInfo.version( QSystemInfo::Firmware ) );
    info.append( "\n");
#ifndef Q_WS_MAEMO_5
    info.append( "SysInfo version QSystemInfo::QtMobility: ");
    info.append( sysInfo.version( QSystemInfo::QtMobility ) );
    info.append( "\n\n");
#endif

#ifdef LITEVERSION
    info.append( QString( "Xema Lite version: " ) );
#else
    info.append( QString( "Xema version: " ) );
#endif
    QString majorVersion;
    majorVersion.setNum( MAJORVERSION );
    QString minorVersion;
    minorVersion.setNum( MINORVERSION );
    QString patchVersion;
    patchVersion.setNum( PATCHVERSION );
    QString version( majorVersion + "." + minorVersion + "." + patchVersion );
    info.append( version );
    info.append( "\n\n");


    if( sysInfo.hasFeatureSupported( QSystemInfo::BluetoothFeature ) )
        info.append( "SysInfo feature supported, QSystemInfo::BluetoothFeature:");
    info.append( "\n");
    if( sysInfo.hasFeatureSupported( QSystemInfo::MemcardFeature ) )
        info.append( "SysInfo feature supported, QSystemInfo::MemcardFeature:");
    info.append( "\n");
    if( sysInfo.hasFeatureSupported( QSystemInfo::VibFeature ) )
        info.append( "SysInfo feature supported, QSystemInfo::VibFeature:");
    info.append( "\n");
    if( sysInfo.hasFeatureSupported( QSystemInfo::WlanFeature ) )
        info.append( "SysInfo feature supported, QSystemInfo::WlanFeature:");
    info.append( "\n");
    if( sysInfo.hasFeatureSupported( QSystemInfo::SimFeature ) )
        info.append( "SysInfo feature supported, QSystemInfo::SimFeature:");
    info.append( "\n");
    if( sysInfo.hasFeatureSupported( QSystemInfo::HapticsFeature ) )
        info.append( "SysInfo feature supported, QSystemInfo::HapticsFeature:");
#endif
    info.append( "\n\n");
    return info;
}

bool SystemInfoProvider::touchSupported()
{
#ifdef USE_MOBILITY
    QSystemDeviceInfo devInfo;

    long types = devInfo.inputMethodType();
    if( ( types & QSystemDeviceInfo::MultiTouch ) == QSystemDeviceInfo::MultiTouch )
    {
        return true;
    }
    if( ( types & QSystemDeviceInfo::SingleTouch ) == QSystemDeviceInfo::SingleTouch )
    {
        return true;
    }
    else
    {
        return false;
    }
#else
    return true;
#endif

}

bool SystemInfoProvider::landscapeDevice()
{
#ifdef USE_MOBILITY
    QSystemDeviceInfo devInfo;
    QString productType = devInfo.productName();

    // Is there any better way to do this?
    if( productType == "RM-462" ||
    productType == "RM-627" ||
    productType == "RM-648" ||
    productType == "RM-658" ||
    productType == "RM-632" ||
    productType == "RM-634" ||
    productType == "RM-699" ||
    productType == "RM-529" ||
    productType == "RM-530" ||
    productType == "RM-584" ||
    productType == "RM-346" ||
    productType == "RM-407" ||
    productType == "RM-357" ||
    productType == "RM-493" ||
    productType == "RM-437" ||
    productType == "RM-450" ||
    productType == "RM-449" ||
    productType == "RM-600" )
    {
        return true;
    }
#endif
    return false;
}

bool SystemInfoProvider::batteryCharging()
{
#ifdef USE_MOBILITY
    QSystemDeviceInfo devInfo;

    int powerState = devInfo.currentPowerState();
    if( powerState == QSystemDeviceInfo::WallPowerChargingBattery )
    {
        return true;
    }
#endif
    return false;
}

int SystemInfoProvider::currentProfile()
{
    int profile =  0;
#ifdef USE_MOBILITY
    profile = mDeviceInfo->currentProfile();
#endif
    return profile;
}

QString SystemInfoProvider::currentLanguage()
{
    QString lang;
#ifdef USE_MOBILITY
    QString country;
    QSystemInfo sysInfo;
    lang = sysInfo.currentLanguage();
    country = sysInfo.currentCountryCode();
    if( lang.isEmpty() == false && country.isEmpty() == false )
    {
        lang.append( "_" );
        lang.append( country );
    }
#endif
    return lang;
}

QString SystemInfoProvider::osVersion()
{
    QString ver;
#ifdef USE_MOBILITY
    QSystemInfo sysInfo;
    ver = sysInfo.version( QSystemInfo::Os );
#endif
    return ver;
}

bool SystemInfoProvider::imeiAccepted()
{
#ifdef USE_MOBILITY
    QSystemDeviceInfo sysInfo;
    QString imei = sysInfo.imei();
    imei.remove( "-" );
    QString restrictedToImei;
    restrictedToImei.setNum( ONLYFORIMEI );
    if( restrictedToImei.compare( "0" ) != 0 )
    {
        qDebug() << "bool SystemInfoProvider::imeiAccepted(), restricted to IMEI" << restrictedToImei;
    }
    if( restrictedToImei.compare( "0" ) != 0 && imei.compare( restrictedToImei, Qt::CaseInsensitive ) != 0 )
    {
        qDebug() << "Wrong IMEI";
        return false;
    }
    else
    {
        return true;
    }
#endif
    return true;
}

bool SystemInfoProvider::qtVersion474()
{
    bool qt474 = false;
#ifdef USE_MOBILITY
    QString qtVersion;
    QSystemInfo sysInfo;
    qtVersion = sysInfo.version( QSystemInfo::QtCore );

    QString qtMajor = qtVersion.left( qtVersion.indexOf( "." ) );
    QString qtMinor = qtVersion.mid( qtVersion.indexOf( "." ) + 1, 1 );
    QString qtPatch = qtVersion.mid( qtVersion.indexOf( ".", 2 ) + 1 );
    qDebug() << "QT VER" << qtMajor << qtMinor << qtPatch;
    bool ok;
    bool ok2;
    bool ok3;
    int majorNum = qtMajor.toInt( &ok );
    int minorNum = qtMinor.toInt( &ok2 );
    int patchNum = qtPatch.toInt( &ok3 );
    if( ok == true && ok2 == true && ok3 == true )
    {
        if( majorNum > 4 )
        {
            qt474 = true;
        }
        else if( majorNum == 4 && minorNum > 7 )
        {
            qt474 = true;
        }
        else if( majorNum == 4 && minorNum == 7 && patchNum >= 4 )
        {
            qt474 = true;
        }
    }


#endif
    return qt474;
}
