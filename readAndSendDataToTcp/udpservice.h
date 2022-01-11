#ifndef UDPSERVICE_H
#define UDPSERVICE_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include "protocol.h"
class udpService:public QObject
{
    Q_OBJECT
public:
    udpService(QObject *parent = nullptr);

    void startUdpService(QHostAddress localIP, quint16 localPort, QHostAddress serviceIP, quint16 servicePort, dDATA *buf, int time);
    void closeUdpService();

private:
    QTimer *timer;
    dDATA *data;
    QUdpSocket *udpsocket;

    QHostAddress IPV4;
    quint16 Port;
private slots:
    void sendData();
    void recvData();
signals:
    void setPackage(char*buf, unsigned short size);
};

#endif // UDPSERVICE_H
