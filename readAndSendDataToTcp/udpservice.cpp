#include "udpservice.h"
#include <QDebug>
#include <QThread>
udpService::udpService(QObject *parent)
    :QObject(parent)
{

    udpsocket = new QUdpSocket(this);
    connect(udpsocket, SIGNAL(readyRead()), this,SLOT(recvData()));

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &udpService::sendData);
}

void udpService::startUdpService(QHostAddress localIP, quint16 localPort, QHostAddress serviceIP, quint16 servicePort, dDATA *buf, int time)
{
    IPV4 = serviceIP;
    Port = servicePort;
    data = buf;
    timer->setInterval(time);
    if(!timer->isActive())
        timer->start();

    if(udpsocket->state())
        udpsocket->close();

    udpsocket->bind(localIP,localPort);


    //qDebug()<<"startUdpService"<<QThread::currentThreadId()<<endl;
}

void udpService::closeUdpService()
{
    if(timer->isActive())
        timer->stop();
    if(udpsocket->state())
        udpsocket->close();
    //qDebug()<<"closeUdpService"<<QThread::currentThreadId()<<endl;
}

void udpService::sendData()
{
    int size = data->size;
    for(int i = 0; i < data->size; i=i+1024)
    {
        if(size>1024)
            bool ok = udpsocket->writeDatagram((char*)(&data->data[i]),1024,IPV4,Port);
        else
            udpsocket->writeDatagram((char*)(&data->data[i]),size,IPV4,Port);
        size-=1024;
    }

    //qDebug()<<"sendData()"<<QThread::currentThreadId()<<"  "<<endl;

}

void udpService::recvData()
{
    QHostAddress host;
    quint16 port;
    char *buf;
    int Size = 0;
    while(udpsocket->hasPendingDatagrams())			//(a)
    {
       // qDebug()<<"recvData()"<<QThread::currentThreadId()<<"  "<<endl;
        QByteArray datagram;
        int size = udpsocket->pendingDatagramSize();

        char *buff = new char[size];
        buf = buff;
        Size = size;
        size = udpsocket->readDatagram(buff,size,&host,&port);	//(b),最长接收1024字节数据



        if(size!=-1&&host==IPV4&&Port==port)
        {
            emit setPackage(buff, (unsigned short)size);
        }
        //delete [] buff;
   }


}
