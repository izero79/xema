#include <QDebug>
#include <QDir>
#include <QNetworkInterface>
#include "systeminfoprovider.h"

#ifdef USE_MOBILITY
#include <QSystemInfo>
#endif


SystemInfoProvider::SystemInfoProvider(QObject *parent) :
    QObject(parent)
{

}

SystemInfoProvider::~SystemInfoProvider()
{
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

bool SystemInfoProvider::compassSupported()
{
    bool isSupported = true;
#ifdef USE_MOBILITY
    QSystemDeviceInfo devInfo;
    if (QString::compare(devInfo.model(),"X7", Qt::CaseInsensitive) == 0) {
        isSupported = false;
    }
    if (QString::compare(devInfo.model(),"X7-00", Qt::CaseInsensitive) == 0) {
        isSupported = false;
    }
    if (QString::compare(devInfo.productName(),"RM-707", Qt::CaseInsensitive) == 0) {
        isSupported = false;
    }
#endif
    return isSupported;
}
