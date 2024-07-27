#include "serverStuff.h"

ServerStuff::ServerStuff(QObject *pwgt) : QObject(pwgt), m_nNextBlockSize(0)
{
    tcpServer = new QTcpServer(this);
}

QList<QTcpSocket *> ServerStuff::getClients()
{
    return clients;
}

void ServerStuff::newConnection()
{
    clientSocket = tcpServer->nextPendingConnection();

    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    connect(clientSocket, &QTcpSocket::readyRead, this, &ServerStuff::readClient);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ServerStuff::gotDisconnection);
    clients << clientSocket;
    sendToClient(clientSocket, "Reply: connection established");
}


void ServerStuff::readClient()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());

    QDataStream in(clientSocket);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (clientSocket->bytesAvailable() < sizeof(quint16)) { break; }
            in >> m_nNextBlockSize;
        }

        if (clientSocket->bytesAvailable() < m_nNextBlockSize) { break; }

        QString identifier;
        in >> identifier;

        if (identifier == "SliderValue") {
            in >> rpmValue;

            qDebug() << "Received RPM value:" << rpmValue;
            emit rpmReceived();
        }

        else if (identifier == "rateSlider") {
            in >> spawnRate;
            qDebug() << "Received Spawn Rate value:" << spawnRate;
            emit spawnRateReceived(spawnRate);
        }

        else if (identifier == "StartButtonClicked") {
            emit startButtonClicked(spawnRate);
        }

        else if (identifier == "StopButtonClicked") {
            emit stopButtonClicked();
        }
        m_nNextBlockSize = 0;
    }
}



void ServerStuff::gotDisconnection()
{
    clients.removeAt(clients.indexOf((QTcpSocket*)sender()));
    emit smbDisconnected();
}

qint64 ServerStuff::sendToClient(QTcpSocket* socket, const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    return socket->write(arrBlock);
}

qint64 ServerStuff::sendAngleToClient(QTcpSocket* socket, double angle)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0) << QString("AngleValue") << angle;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    return socket->write(arrBlock);
}

qint64 ServerStuff::sendTimerToClient(QTcpSocket* socket, int timer)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0) << QString("TimerValue") << timer;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    return socket->write(arrBlock);
}


qint64 ServerStuff::sendPlotToClient(QTcpSocket* socket, double distance, double degree)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0) << QString("PlotValues") << distance << degree;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    return socket->write(arrBlock);
}
