#include "getipaddress.h"
#include "ui_getipaddress.h"
#include <QDebug>
#include <iostream>
using namespace std;
getIPAddress::getIPAddress(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::getIPAddress)
{
    ui->setupUi(this);
}

getIPAddress::~getIPAddress()
{
    delete ui;
}




void getIPAddress::on_getIPBt_clicked()
{
    this->clearBroswer();
    //获取主机名
    QString localHostName = QHostInfo::localHostName();
        //qDebug() << "localHostName" << localHostName;
    this->addBroswer(localHostName);

    //获取本机的IP地址(可能有多条）
    QHostInfo info = QHostInfo::fromName(localHostName);
        //qDebug()<<"IP Address："<< info.addresses();
    for(int i=0; i<info.addresses().size(); ++i)
    {
        this->addBroswer(info.addresses().at(i).toString());
    }



}

void getIPAddress::on_getInterBt_clicked()
{
    this->clearBroswer();
    //通过QNetworkInterface类来获取本机的IP地址和网络接口信息
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    //获取所有网络接口的列表
    foreach(QNetworkInterface interface,list)
    {
        //遍历每一个网络接口
        //if(interface.name().left(8)=="ethernet"){
        //  qDebug() << "Device: "<<interface.name();
        //}


        this->addBroswer("----------------------");
        //设备名
        this->addBroswer(interface.humanReadableName());
        //硬件地址
        this->addBroswer(interface.hardwareAddress());

        QList<QNetworkAddressEntry> entryList = interface.addressEntries();
        //获取IP地址条目列表，每个条目中包含一个IP地址，一个子网掩码和一个广播地址
        foreach(QNetworkAddressEntry entry,entryList)
        {
            //遍历每一个IP地址条目
            this->addBroswer("IP Address: "+entry.ip().toString());

            //子网掩码
            this->addBroswer("Netmask: "+entry.netmask().toString());

            //广播地址
            this->addBroswer("Broadcast: "+entry.broadcast().toString());

        }
    }
}



void getIPAddress::on_setIPBt_clicked()
{
    this->clearBroswer();
    //QString name = "\""+interface.humanReadableName()+"\"";
    QString name = "\"以太网\"";
    QString ip = "192.168.2.3";
    QString netmask = "255.255.255.0";
    QProcess cmd(this);
    // netsh interface ipv4 set address name = "以太网" source = static
    // address = 192.168.0.106 mask = 255.255.255.0 gateway = 192.168.0.1 gwmetric = 0
    QString command = "netsh interface ipv4 set address name="
            + name + " source=static address=" + ip
            + " mask = " + netmask;
    qDebug()<<command<<endl;
    cmd.start(command);
    cmd.waitForFinished();
    //QNetworkSession::accept();
}

void getIPAddress::addBroswer(QString str)
{
    ui->textBrowser->append(str);
}

void getIPAddress::clearBroswer()
{
    ui->textBrowser->clear();
}
