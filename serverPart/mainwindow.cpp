#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    , controller(new RadarBeamController(this))
    , spawner(new RadarFalsePlotSpawner(this))
{
    ui->setupUi(this);

    server = new ServerStuff(this);
    connect(server, &ServerStuff::gotNewMesssage,
            this, &MainWindow::gotNewMesssage);
    connect(server->tcpServer, &QTcpServer::newConnection,
            this, &MainWindow::smbConnectedToServer);
    connect(server, &ServerStuff::smbDisconnected,
            this, &MainWindow::smbDisconnectedFromServer);
    connect(server, &ServerStuff::startButtonClicked, this, &MainWindow::startRotation);
    connect(server, &ServerStuff::stopButtonClicked, this, &MainWindow::stopRotation);
    connect(server, &ServerStuff::rpmReceived, this, &MainWindow::updateServerRPM);
    connect(server, &ServerStuff::startButtonClicked, spawner, &RadarFalsePlotSpawner::start);
    connect(server, &ServerStuff::stopButtonClicked, spawner, &RadarFalsePlotSpawner::stop);
    connect(controller, &RadarBeamController::degreeUpdated, this , &MainWindow::updateRotation);
    connect(controller, &RadarBeamController::timerUpdated, this , &MainWindow::updateTimer);
    connect(server, &ServerStuff::startButtonClicked, this, &MainWindow::startRotation);
    connect(controller, &RadarBeamController::degreeUpdated, this, &MainWindow::fromControllerToSpawner);
    connect(server, &ServerStuff::spawnRateReceived, this, &MainWindow::updateServerSpawnRate);

    connect(spawner, &RadarFalsePlotSpawner::newPlot, this , &MainWindow::updatePlot);

}

MainWindow::~MainWindow()
{
    delete server;
    delete ui;
}

void MainWindow::on_pushButton_startServer_clicked()
{
    if (!server->tcpServer->listen(QHostAddress::Any, 6547))
    {
        ui->textEdit_log->append(tr("<font color=\"red\"><b>Error!</b> The port is taken by some other service.</font>"));
        return;
    }
    connect(server->tcpServer, &QTcpServer::newConnection, server, &ServerStuff::newConnection);
    ui->textEdit_log->append(tr("<font color=\"green\"><b>Server started</b>, port is openned.</font>"));
}

void MainWindow::on_pushButton_stopServer_clicked()
{
    if(server->tcpServer->isListening())
    {
        disconnect(server->tcpServer, &QTcpServer::newConnection, server, &ServerStuff::newConnection);

        QList<QTcpSocket *> clients = server->getClients();
        for(int i = 0; i < clients.count(); i++)
        {
            server->sendToClient(clients.at(i), "0");
        }

        server->tcpServer->close();
        ui->textEdit_log->append(tr("<b>Server stopped</b>, post is closed"));
    }
    else
    {
        ui->textEdit_log->append(tr("<b>Error!</b> Server was not running"));
    }
}

void MainWindow::on_pushButton_testConn_clicked()
{
    if(server->tcpServer->isListening())
    {
        ui->textEdit_log->append(
                    QString("%1 %2")
                    .arg("Server is listening, number of connected clients:")
                    .arg(QString::number(server->getClients().count()))
                    );
    }
    else
    {
        ui->textEdit_log->append(
                    QString("%1 %2")
                    .arg("Server is not listening, number of connected clients:")
                    .arg(QString::number(server->getClients().count()))
                    );
    }
}

void MainWindow::smbConnectedToServer()
{
    ui->textEdit_log->append(tr("Somebody has connected"));
}

void MainWindow::smbDisconnectedFromServer()
{
    ui->textEdit_log->append(tr("Somebody has disconnected"));
}

void MainWindow::gotNewMesssage(QString msg)
{
    ui->textEdit_log->append(QString("New message: %1").arg(msg));
}


void MainWindow::startRotation()
{
    controller->start();
}

void MainWindow::stopRotation()
{
    controller->stop();
}

void MainWindow::updateServerRPM()
{
    controller->setRpm(server->getRpmValue());
}

void MainWindow::updateServerSpawnRate(int newRate)
{
    spawner->setRate(server->getSpawnRate());

}

void MainWindow::updateRotation()
{
    server->sendAngleToClient(server->clientSocket, controller->degreeCounter);
}

void MainWindow::updateTimer()
{
    server->sendTimerToClient(server->clientSocket, controller->timerSeconds);
}

void MainWindow::fromControllerToSpawner()
{
    spawner->emitterDegree = controller->degreeCounter;
    spawner->rpm1 = controller->rpm;
}


void MainWindow::updatePlot()
{
    server->sendPlotToClient(server->clientSocket, spawner->distance, spawner->degree);
}
