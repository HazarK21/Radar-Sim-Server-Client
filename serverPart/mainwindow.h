#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "radarbeamcontroller.h"
#include "radarfalseplotspawner.h"
#include "serverStuff.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // double degree;

public slots:
    void startRotation();
    void stopRotation();

    
private slots:
    void on_pushButton_stopServer_clicked();
    void on_pushButton_startServer_clicked();
    void on_pushButton_testConn_clicked();
    void smbConnectedToServer();
    void smbDisconnectedFromServer();
    void gotNewMesssage(QString msg);
    void updateServerRPM();
    void updateServerSpawnRate(int newRate);
    void updateRotation();
    void updateTimer();
    void fromControllerToSpawner();
    void updatePlot();

private:
    Ui::MainWindow *ui;
    ServerStuff *server;
    RadarBeamController *controller;
    RadarFalsePlotSpawner *spawner;
};

#endif // MAINWINDOW_H
