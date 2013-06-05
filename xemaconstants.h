#ifndef XEMACONSTANTS_H
#define XEMACONSTANTS_H

#include <QString>
/*
#ifdef Q_OS_SYMBIAN
static const QString filePath("c:/data/");
#else
//static const QString filePath("/home/tsiirone/");
static const QString filePath("C:/");
#endif
*/
//static const int OBS_SUBFIELDCOUNT = 12;
enum NetworkConnectionState
{
    CONNECTING = 0,
    CONNECTING_CANCELED,
    NO_AVAILABLE_WLANS,
    NO_AVAILABLE_CONNECTIONS
};

enum NetworkConnectionType
{
    ANY = 0,
    WLAN,
    CELLULAR
};

#endif // XEMACONSTANTS_H
