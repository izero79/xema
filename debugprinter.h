#ifndef DEBUGPRINTER_H
#define DEBUGPRINTER_H

#include <QtDebug>
#include <QFile>
#include <QTextStream>

void DebugPrinter(QtMsgType type, const char *msg)
{
#ifdef DEBUGONLYTOFILE
    QString txt;
    switch (type) {
    case QtDebugMsg:
            txt = QString("Debug: %1").arg(msg);
            break;

    case QtWarningMsg:
            txt = QString("Warning: %1").arg(msg);
    break;
    case QtCriticalMsg:
            txt = QString("Critical: %1").arg(msg);
    break;
    case QtFatalMsg:
            txt = QString("Fatal: %1").arg(msg);
            abort();
    }

#if defined(Q_OS_SYMBIAN)
    QFile outFile("c:/data/xema-debuglog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
#elif defined(Q_WS_MAEMO_5) || defined(HARMATTAN)
    QFile outFile("/home/user/MyDocs/xema-debuglog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
#endif
#else
    Q_UNUSED(type)
    Q_UNUSED(msg)
#endif
}

#endif // DEBUGPRINTER_H
