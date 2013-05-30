#ifndef KINETICSCROLLER_H
#define KINETICSCROLLER_H

#include <QObject>
#include <QVariant>
#include <QTimer>
#include <QTime>
#include <QPointF>

class KineticScroller: public QObject
{
Q_OBJECT
public:
    KineticScroller();
public slots:
    void mousePressed();
    void mouseMoved(QVariant deltaX,QVariant deltaY);
    void mouseReleased();
    void kineticTimerEvent();
signals:
    void kineticPan(QVariant deltaX, QVariant deltaY);
private:
 void panFloatWrapper(const QPointF& delta);
private:
    bool        m_panActive;
    QTimer*     m_kineticTimer;
    QTime       m_lastMoveTime;
    // Fractional pan, used by panFloatWrapper
    QPointF     m_remainingPan;
    // current kinetic panning speed, in pixel/msec
    QPointF     m_kineticPanSpeed;
    bool        m_panDecellerate;
    // An entry in the mouse history. first=speed, second=time
    typedef QPair<QPointF, QTime> MouseHistoryEntry;
    // A history of the last (currently 5) mouse move events is stored in order to smooth out movement detection for kinetic panning
    QList<MouseHistoryEntry> m_mouseHistory;
};



#endif // KINETICSCROLLER_H
