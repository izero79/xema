#include "kineticscroller.h"

#include <QTimer>
#include <cmath>
#include <QGraphicsSceneMouseEvent>
#include <QGestureEvent>
#include <qDebug>

// TODO: Some of these could be exposed in a GUI and should probably be put elsewhere in that case (and made non-const)
#if defined(Q_OS_SYMBIAN) || defined(Q_OS_WINCE_WM) || defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
// Devices
static const bool enableKineticPanning = true;
static const qreal kineticPanningHalflife = 200.0; // time until kinetic panning speed slows down to 50%, in msec
static const qreal panSpeedNormal = 0.3; // keyboard panning speed without modifiers, in pixels/msec
static const qreal panSpeedFast = 1.0; // keyboard panning speed with shift, in pixels/msec
static const qreal kineticPanSpeedThreshold = 0.02; // minimum panning speed, in pixels/msec
static const int kineticPanningResolution = 75; // temporal resolution. Smaller values take more CPU but improve visual quality
static const int holdTimeThreshold = 200; // maximum time between last mouse move and mouse release for kinetic panning to kick in
#else
// Desktop
static const bool enableKineticPanning = true;
static const qreal kineticPanningHalflife = 300.0; // time until kinetic panning speed slows down to 50%, in msec
static const qreal panSpeedNormal = 0.3; // keyboard panning speed without modifiers, in pixels/msec
static const qreal panSpeedFast = 1.0; // keyboard panning speed with shift, in pixels/msec
static const qreal kineticPanSpeedThreshold = 0.005; // minimum panning speed, in pixels/msec
static const int kineticPanningResolution = 30; // temporal resolution. Smaller values take more CPU but improve visual quality
static const int holdTimeThreshold = 100; // maximum time between last mouse move and mouse release for kinetic panning to kick in
#endif


KineticScroller::KineticScroller()
{
    m_kineticTimer = new QTimer(),

    connect(m_kineticTimer, SIGNAL(timeout()), this, SLOT(kineticTimerEvent()));
    m_kineticTimer->setInterval(kineticPanningResolution);
}



void KineticScroller::kineticTimerEvent()
{
    QTime currentTime = QTime::currentTime();
    int deltaTime = m_lastMoveTime.msecsTo(currentTime);
    m_lastMoveTime = currentTime;

    if (m_panDecellerate)
        m_kineticPanSpeed *= pow(qreal(0.5), qreal(deltaTime / kineticPanningHalflife));

    QPointF scaledSpeed = m_kineticPanSpeed * deltaTime;
    if (m_kineticPanSpeed.manhattanLength() < kineticPanSpeedThreshold) {
        // Kinetic panning is almost halted -> stop it.
        m_kineticTimer->stop();
        return;
    }
    panFloatWrapper(scaledSpeed);
}

// Wraps the pan(int, int) method to achieve floating point accuracy, which is needed to scroll smoothly.
void KineticScroller::panFloatWrapper(const QPointF& delta)
{
    // Add to previously stored panning distance
    m_remainingPan += delta;

    // Convert to integers
    QPoint move = m_remainingPan.toPoint();

    // Commit mouse movement
    emit kineticPan(QVariant(move.x()),QVariant(move.y()));


    // Store committed mouse movement
    m_remainingPan -= move;

}

void KineticScroller::mousePressed()
{
    m_panActive = true;
    // When pressing, stop the timer and stop all current kinetic panning
    m_kineticTimer->stop();
    m_kineticPanSpeed = QPointF();
    m_lastMoveTime = QTime::currentTime();
}

void KineticScroller::mouseMoved(QVariant deltaX,QVariant deltaY)
{
    if (m_panActive) {

        QTime currentTime = QTime::currentTime();
        int deltaTime = m_lastMoveTime.msecsTo(currentTime);
        m_lastMoveTime = currentTime;

        // Calculate position delta
        QPointF delta(deltaX.toInt(),deltaY.toInt());

        // Calculate and set speed
        if (deltaTime > 0) {
            m_kineticPanSpeed = delta / deltaTime;
            m_mouseHistory.push_back(MouseHistoryEntry(m_kineticPanSpeed, currentTime));

            if(m_mouseHistory.count() > 5){
                m_mouseHistory.pop_front();
            }
        }

        // Pan map
        panFloatWrapper(delta);
    }
}

void KineticScroller::mouseReleased()
{
    if (m_panActive) {
        m_panActive = false;

        if (!enableKineticPanning || m_lastMoveTime.msecsTo(QTime::currentTime()) > holdTimeThreshold) {
            return;
        }

        m_kineticPanSpeed = QPointF();
        int entries_considered = 0;

        QTime currentTime = QTime::currentTime();
        foreach(MouseHistoryEntry entry, m_mouseHistory) {
            // first=speed, second=time
            int deltaTime = entry.second.msecsTo(currentTime);
            if (deltaTime < holdTimeThreshold) {
                m_kineticPanSpeed += entry.first;
                entries_considered++;
            }
        }
        if (entries_considered > 0){
            m_kineticPanSpeed /= entries_considered;
        }

        m_lastMoveTime = currentTime;

        // When releasing the mouse button/finger while moving, start the kinetic panning timer
        m_kineticTimer->start();
        m_panDecellerate = true;
    }

}

