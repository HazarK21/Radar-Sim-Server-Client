#ifndef RADARFALSEPLOTSPAWNER_H
#define RADARFALSEPLOTSPAWNER_H

#include "qtmetamacros.h"
#include <QObject>
#include <QTimer>
#include <QRandomGenerator>

class RadarFalsePlotSpawner : public QObject
{
    Q_OBJECT
public:
    explicit RadarFalsePlotSpawner(QObject *parent = nullptr);
    double distance;
    double degree;
    bool isRunning;
    double emitterDegree;
    int rpm1;
    int spawnRate;

signals:
    void newPlot(double distance, double degree);
    void startSpawnPlot();

public slots:
    void start(int spawnRate);
    void setRate(int newRate);
    void stop();
    double generateRandomDistance();
    void spawnPlot();

private slots:

private:
    QTimer *spawnTimer;
    RadarFalsePlotSpawner *spawner;
    double generateRandomDegree();
};
#endif
