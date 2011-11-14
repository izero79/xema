#ifndef DUMMYDEBUGPRINTER_H
#define DUMMYDEBUGPRINTER_H

#include <QtDebug>
#include <QFile>
#include <QTextStream>

void DummyDebugPrinter(QtMsgType type, const char *msg)
{
    Q_UNUSED( type )
    Q_UNUSED( msg )
}

#endif // DUMMYDEBUGPRINTER_H
