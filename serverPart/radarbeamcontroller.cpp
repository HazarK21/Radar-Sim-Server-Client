#include "RadarBeamController.h"
#include <QDebug>
#include <QGraphicsPolygonItem>

RadarBeamController::RadarBeamController(QObject *parent) : QObject(parent), degreeCounter(0), rpm(0), rotationAngle(0), rotationTimer(new QTimer(this)),timer(new QTimer(this)), timerSeconds(0)
{
    connect(rotationTimer, &QTimer::timeout, this, &RadarBeamController::updateRotation);
    connect(timer, &QTimer::timeout, this, &RadarBeamController::updateTimer);
}

void RadarBeamController::start()
{
    rotationTimer->start(10);
    timer->start(1000);
}

void RadarBeamController::stop()
{
    rotationTimer->stop();
    timer->stop();
}

void RadarBeamController::setRpm(int newRpm)
{
    rpm = newRpm;
}

int RadarBeamController::getTimerValue() const
{
    return timerSeconds;
}

void RadarBeamController::updateRotation()
{
    rotationAngle += (360.0 * rpm * 10 / 60000.0);
    degreeCounter = static_cast<int>(rotationAngle) % 360;
    emit rotationChanged(degreeCounter);
    emit degreeUpdated(degreeCounter);

    degreeCounter = static_cast<int>(degreeCounter);
}

void RadarBeamController::updateTimer()
{
    ++timerSeconds;
    emit timerUpdated(timerSeconds);
}

