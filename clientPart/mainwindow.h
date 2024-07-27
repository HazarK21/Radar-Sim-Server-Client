#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsEllipseItem>
#include <QChart>
#include "radarppiscope.h"
#include "clientStuff.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void setStatus(bool newStatus);
    void receivedSomething(QString msg);
    void gotError(QAbstractSocket::SocketError err);

signals:
    void newPlot(double distance, double degree);


private slots:
    void on_pushButton_connect_clicked();
    void on_pushButton_disconnect_clicked();
    void updateRpm(int value);
    void updateSpawnRate(int newRate);
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void updateDialRotation(int angle);
    void updateTimerDisplay(int seconds);
    void updateAngleDisplay(double degrees);
    void updatePlot(double distance, double degree);



private:
    Ui::MainWindow *ui;
    ClientStuff *client;
    QGraphicsScene *Scene;
    QGraphicsEllipseItem *ItemEllipse;
    RadarPPIScope *scope;
};

#endif // MAINWINDOW_H
