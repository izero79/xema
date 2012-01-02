#ifndef LOKKICONSTANTS_H
#define LOKKICONSTANTS_H

#include <QString>

#ifdef Q_OS_SYMBIAN
static const QString filePath( "c:/data/");
#else
static const QString filePath( "/home/tsiirone/");
#endif

#endif // LOKKICONSTANTS_H
