#ifndef XEMAUTILS_H
#define XEMAUTILS_H

#include <QString>

class XemaUtils
{
public:
    XemaUtils();

    static QString dataFileDir();
    static QString exportDir();
    static QString importDir();
    static QString importedDir();
    static void checkAndCreateDirs();

};

#endif // XEMAUTILS_H
