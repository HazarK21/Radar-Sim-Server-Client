#ifndef CLIENTSTUFF_H
#define CLIENTSTUFF_H

#include <QString>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>

class ClientStuff : public QObject
{
    Q_OBJECT

public:
    ClientStuff(const QString hostAddress, int portVal, QObject *parent = 0);

    QTcpSocket *tcpSocket;
    bool getStatus();
    double angle;
    double getClientAngle() const { return angle; }

public slots:
    void closeConnection();
    void connect2host();
    void handleAngleValue(double angle);
    void handleTimerValue(int timer);
    void handlePlotValues(double distance, double degree);

signals:
    void statusChanged(bool);
    void hasReadSome(QString msg);
    void handleAngleValueSignal(double angle);
    void handleTimerValueSignal(int timer);
    void handlePlotValuesSignal(double distance, double degree);

private slots:
    void readyRead();
    void connected();
    void connectionTimeout();

private:
    QString host;
    int port;
    bool status;
    quint16 m_nNextBlockSize;
    QTimer *timeoutTimer;
};

#endif // CLIENTSTUFF_H
