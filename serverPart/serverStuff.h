#ifndef SERVERSTUFF_H
#define SERVERSTUFF_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QList>

class ServerStuff : public QObject
{
    Q_OBJECT

public:
    ServerStuff(QObject *pwgt);
    QTcpServer *tcpServer;
    QList<QTcpSocket *> getClients();
    int getRpmValue() const { return rpmValue; }
    int getSpawnRate() const { return spawnRate; }
    QTcpSocket *clientSocket;

public slots:
    virtual void newConnection();
    void readClient();
    void gotDisconnection();
    qint64 sendToClient(QTcpSocket *socket, const QString &str);
    qint64 sendAngleToClient(QTcpSocket* socket, double angle);
    qint64 sendTimerToClient(QTcpSocket* socket, int timer);
    qint64 sendPlotToClient(QTcpSocket* socket, double distance, double degree);


signals:
    void gotNewMesssage(QString msg);
    void smbDisconnected();
    void startButtonClicked(int spawnRate);
    void stopButtonClicked();
    void rpmReceived();
    void spawnRateReceived(int newRate);


private:
    quint16 m_nNextBlockSize;
    QList<QTcpSocket*> clients;
    int rpmValue;
    int spawnRate;
};

#endif // SERVERSTUFF_H
