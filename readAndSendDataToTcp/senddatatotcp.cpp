#include "senddatatotcp.h"
#include "ui_senddatatotcp.h"
#include <QDebug>
#include <iostream>

SendDataToTcp::SendDataToTcp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SendDataToTcp)
{
    ui->setupUi(this);

    //qDebug()<<"main:"<<QThread::currentThreadId()<<endl;
    //获取本地以太网IP
    QString host = getIPv4();
    ui->localIP->setText(host);

    //获取随机端口号
    QString port = getRandPort();
    ui->localPort->setText(port);

    ui->servicerIP->setText(host);
    ui->servicerPort->setText("8080");

    threadFile = new QThread(this);
    m_readFile = new readFile;
    m_readFile->moveToThread(threadFile);

    connect(this, &SendDataToTcp::startReadFileThread, m_readFile, &readFile::getFileData);
    connect( m_readFile, &readFile::readStop, this, &SendDataToTcp::showData);



    threadUdpService = new QThread(this);
    m_UdpService = new udpService;
    m_UdpService->moveToThread(threadUdpService);
    connect(this, &SendDataToTcp::startUdpServiceThread, m_UdpService, &udpService::startUdpService);
    connect(threadUdpService, &QThread::finished, m_UdpService, &udpService::closeUdpService);

    connect(m_UdpService, &udpService::setPackage, this, &SendDataToTcp::showRecvData);





}

SendDataToTcp::~SendDataToTcp()
{
    if(!threadFile->isFinished())
       threadFile->quit();
    if(threadFile->isFinished())
        delete threadFile;
    if(threadUdpService->isRunning() == true)
    {
        threadUdpService->quit();
        threadUdpService->wait();
    }
    delete  threadUdpService;

    delete m_UdpService;
    delete m_readFile;
    delete ui;
}


void SendDataToTcp::on_OpenFileBt_clicked()
{
    QFileDialog fd;
    fd.setAcceptMode(QFileDialog::AcceptOpen);//文件对话框为打开类型
    fd.setViewMode(QFileDialog::List);//仅列表
    fd.setWindowTitle("OpenFile");
    fd.setFileMode(QFileDialog::ExistingFile);//存在的，单个文件名

    //fd.setFilter(TR("Image Files(*.jpg *.png))");//设置文件类型过滤器
    QStringList filters;
    filters << "mca files (*.mca *.MCA)"
            << "Any files (*)";
    fd.setNameFilters(filters);


    if(fd.exec() == QDialog::Accepted) {//注意使用的是QFileDialog::Accepted或者QDialog::Accepted,不是QFileDialog::Accept
            QString path = fd.selectedFiles()[0];//得到用户选择的文件名

            //读取文件

            if(threadFile->isRunning() == true)
            {
                threadFile->quit();
                threadFile->wait();
            }
            if(threadFile->isRunning() == true)
                return;
            //启动线程，但是没有启动线程处理函数
            threadFile->start();

            emit startReadFileThread(path);
            //显示

    }
}


void SendDataToTcp::on_closeBt_clicked()
{

    //取消监听
    //取消数据放送
    //线程结束

    if(threadUdpService==NULL)
        return;
    if(threadUdpService->isRunning() == true)
    {
        threadUdpService->quit();
        threadUdpService->wait();
    }


}

void SendDataToTcp::on_sendBt_clicked()
{
    //判断TCP是否运行
    //根据端口号开启TCP监听，线程接收数据
    //按照设定时间 开启线程  定时向设定IP端口号发送数据
    if(data->size<0)
        return;
    if(threadUdpService!=NULL)
    {
        if(threadUdpService->isRunning() == true)
        {
            threadUdpService->quit();
            threadUdpService->wait();
        }
    }
    //启动线程，但是没有启动线程处理函数
    qRegisterMetaType<QHostAddress >("QHostAddress");


    threadUdpService->start();


    QHostAddress localIP = QHostAddress(ui->localIP->text());
    quint16 localPort = ui->localPort->text().toInt();
    QHostAddress serviceIP = QHostAddress(ui->servicerIP->text());
    quint16 servicePort = ui->servicerPort->text().toInt();

    int time = ui->lineEdit->text().toInt()*1000;
    emit startUdpServiceThread(localIP, localPort, serviceIP, servicePort,data, time);

}

QString SendDataToTcp::getIPv4()
{
    QString localHostName = QHostInfo::localHostName();
    //获取本机的IP地址(可能有多条）
    QString host;
    QHostInfo info = QHostInfo::fromName(localHostName);
    for(int i=0; i<info.addresses().size(); ++i)
    {
        if(QAbstractSocket::IPv4Protocol == info.addresses().at(i).protocol())
        {
            host = info.addresses().at(i).toString();
            return host;
        }

    }
    return host;
}

QString SendDataToTcp::getRandPort()
{
    srand((unsigned int)time(0));//初始化种子为随机值
    srand(time(0) + rand());
    int port = (rand() % (65535-1024+1)+ 1024);
    return QString::number(port);
}

void SendDataToTcp::showData(dDATA* buf)
{
    //qDebug()<<"main:closeReadThread"<<QThread::currentThreadId()<<endl;
    if(threadFile->isRunning() == true)
    {
        threadFile->quit();
        threadFile->wait();
    }
    //显示

    data = buf;
    QString str;
    for (int i = 0; i<buf->size; i++)
    {
        str+="0x"+QString("%1").arg(buf->data[i], 2, 16, QLatin1Char('0')).toUpper()+" ";
    }
    ui->textBrowser->setText(str);
}

void SendDataToTcp::showRecvData( char *buf, unsigned short size)
{
    //qDebug()<<"main:showRecvData"<<QThread::currentThreadId()<<size<<endl;
    QString str = "REICVE DATA:\n";
    for (int i = 0; i<size; i++)
    {

        str+="0x"+QString("%1").arg((int)buf[i], 2, 16, QLatin1Char('0')).toUpper()+" ";
    }
    delete [] buf;
    ui->textBrowser->append(str);
}

void SendDataToTcp::test()
{
    //qDebug()<<"main:test"<<QThread::currentThreadId()<<endl;
}



void SendDataToTcp::on_pushButton_clicked()
{
    ui->textBrowser->clear();
}
