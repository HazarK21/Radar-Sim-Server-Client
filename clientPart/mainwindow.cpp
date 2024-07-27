#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)    , scope(new RadarPPIScope)
{
    ui->setupUi(this);

    ui->pushButton_disconnect->setVisible(false);

    client = new ClientStuff("localhost", 6547);

    setStatus(client->getStatus());

    connect(client, &ClientStuff::hasReadSome, this, &MainWindow::receivedSomething);
    connect(client, &ClientStuff::statusChanged, this, &MainWindow::setStatus);
    connect(client->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(gotError(QAbstractSocket::SocketError)));



    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::updateRpm);
    connect(ui->spawnRateSlider, &QSlider::valueChanged, this, &MainWindow::updateSpawnRate);
    ui->verticalLayout_2->insertWidget(0,scope->getChartView());
    connect(client, &ClientStuff::handleTimerValueSignal, this, &MainWindow::updateTimerDisplay);
    connect(client, &ClientStuff::handleAngleValueSignal, this, &MainWindow::updateAngleDisplay);
    connect(client, &ClientStuff::handleAngleValueSignal, this, &MainWindow::updateDialRotation);

    connect(client, &ClientStuff::handlePlotValuesSignal, this, &MainWindow::updatePlot);
    connect(this, &MainWindow::newPlot, scope, &RadarPPIScope::getNewPlot);
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

void MainWindow::setStatus(bool newStatus)
{
    if(newStatus)
    {
        ui->label_status->setText(
                    tr("<font color=\"green\">CONNECTED</font>"));
        ui->pushButton_connect->setVisible(false);
        ui->pushButton_disconnect->setVisible(true);
    }
    else
    {
        ui->label_status->setText(
                tr("<font color=\"red\">DISCONNECTED</font>"));
        ui->pushButton_connect->setVisible(true);
        ui->pushButton_disconnect->setVisible(false);
    }
}

void MainWindow::receivedSomething(QString msg)
{
    ui->textEdit_log->append(msg);

}

void MainWindow::gotError(QAbstractSocket::SocketError err)
{
    QString strError = "unknown";
    switch (err)
    {
        case 0:
            strError = "Connection was refused";
            break;
        case 1:
            strError = "Remote host closed the connection";
            break;
        case 2:
            strError = "Host address was not found";
            break;
        case 5:
            strError = "Connection timed out";
            break;
        default:
            strError = "Unknown error";
    }

    ui->textEdit_log->append(strError);
}

void MainWindow::on_pushButton_connect_clicked()
{
    client->connect2host();
}

void MainWindow::on_pushButton_disconnect_clicked()
{
    client->closeConnection();
}

void MainWindow::updateRpm(int value)
{
    ui->rpmLabel->setText(QString("RPM: %1").arg(value));
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0);
    out << QString("SliderValue");
    out << value;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    client->tcpSocket->write(arrBlock);
}

void MainWindow::updateSpawnRate(int newRate)
{
    ui->spawnRateLabel->setText(QString("spawn per sec: %1").arg(newRate));    
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0);
        out << QString("rateSlider");

    out << newRate;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    client->tcpSocket->write(arrBlock);
}

void MainWindow::on_startButton_clicked()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0);
    out << QString("StartButtonClicked");
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    client->tcpSocket->write(arrBlock);
}

void MainWindow::on_stopButton_clicked()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0);
    out << QString("StopButtonClicked");
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    client->tcpSocket->write(arrBlock);
}

void MainWindow::updateDialRotation(int angle)
{
    scope->stick->setRotation(angle-90);
    ui->degreeLabel->setText(QString("%1°").arg(angle % 360));
}

void MainWindow::updateTimerDisplay(int seconds)
{
    ui->sayacLabel->setText(QString("Timer: %1 seconds").arg(seconds));
}

void MainWindow::updateAngleDisplay(double degrees)
{
    ui->degreeLabel->setText(QString("%1°").arg(degrees));
}
void MainWindow::updatePlot(double distance, double degree)
{

    qDebug() << "distance:" << distance << "degree:" << degree;
    emit newPlot(distance, degree);
}

