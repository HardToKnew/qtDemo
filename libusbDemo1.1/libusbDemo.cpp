#include "libusbDemo.h"
#include "ui_libusbDemo.h"
#include <QThread>
#include <QDebug>

libusbDemo::libusbDemo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::libusbDemo)
{
    qDebug()<<"main:"<<QThread::currentThread()<<endl;
    ui->setupUi(this);
    ui->groupBox->close();
    thread = new QThread(this);
    usbThread = new USBThread;
    usbThread->moveToThread(thread);

#ifdef QT_DEBUG
    QByteArray array;
    for (int i=0;i<1024;i++)
    {
        array.append(i);

    }
    for (int i = 0; i < array.length(); ++i)
    {
           strOut.append( tr("%1 ").arg((quint8)array.at(i),2,16,QLatin1Char('0')).toUpper());
    }
    strOut +="\n";
    ui->textEdit->append(strOut);
#endif

    //ui->devicesComBox->setStyleSheet("QComboBox{font-family:'微软雅黑';font-size:11}");

    //init_usb();
    timer_100ms = new QTimer(this);
    timer_100ms->setInterval(100);


    timer_250ms = new QTimer;
    timer_250ms->setInterval(1000);

    timer_Write = new QTimer(this);
    timer_Write->setInterval(1000);
    time.start();

    timer_clear = new QTimer(this);
    timer_clear->setInterval(1000);
    //timer_clear.start();
    ui->stopBtn->close();

    qRegisterMetaType<USBListDesc>("USBListDesc");
    connect(usbThread, &USBThread::sendDevsDesc,  this, &libusbDemo::addUSBDevsList);
    connect(this, &libusbDemo::startUSB, usbThread, &USBThread::runUSBThread);
    connect(usbThread, &USBThread::sendPackage, this, &libusbDemo::recvPackage);
    connect(timer_100ms, &QTimer::timeout, this, &libusbDemo::setByteSpeed);
    connect(timer_250ms, &QTimer::timeout, this, &libusbDemo::setTexkBrower);
    connect(timer_Write, &QTimer::timeout, this, &libusbDemo::USBWrite);
    connect(ui->checkBox, &QAbstractButton::clicked, this, &libusbDemo::changePushButton);
    connect(timer_clear, &QTimer::timeout, this, &libusbDemo::clearByteCounts);

#ifdef USBHOTPLUG_H
        connect(&usbHotplug, &USBHotplug::changeDevice, this, &libusbDemo::changeDevices);
#endif
    usbThread->searchUSB();
    timer_clear->start();
    //thread_timer_250ms->start();
    timer_250ms->start();


}

void libusbDemo::on_openBt_clicked()
{
    //打开usb设备
    if("open"==ui->openBt->text())
    {
        byteRecv = 0;
        thread->start();
        timer_100ms->start();
        ui->openBt->setText("close");
        ui->groupBox->show();
        int devs_index = ui->devicesComBox->currentIndex();
        emit startUSB(devs_index);
    }
    else
    {

        ui->openBt->setText("open");
        ui->groupBox->close();
        ui->configComBox->clear();
        ui->interfaceComBox->clear();
        ui->pointComBox->clear();
        if(thread->isRunning())
        {
            usbThread->close();
            thread->quit();
            thread->wait();
        }
        if(timer_100ms->isActive())
        {
           timer_100ms->stop();
        }
        if(timer_Write->isActive())
        {
            timer_Write->stop();
        }
    }
}

void libusbDemo::on_sendBt_clicked()
{
    //if(timer_Write->isActive())
        //timer_Write->stop();
    byteSend = 0;

    QByteArray array;
    QString str;
    str = ui->sendTextEdit->toPlainText();
    array = QByteArray::fromHex(str.toLatin1().data());
   // emit writeUSB(array);
    //usbThread->USBWrite(array);


    if(ui->checkBox->isChecked()&&ui->lineEdit->text()!=NULL)
    {
        timer_Write->setInterval(ui->lineEdit->text().toDouble()*1000);
        timer_Write->start();
    }
    else
    {
        USBWrite();
        //usbThread->USBWrite(array);
    }
    if(ui->checkBox->isChecked()&&ui->lineEdit->text()!=NULL)
    {
        Sleep(ui->lineEdit->text().toDouble()*1000-1);

    }

    //ui->textEdit->append(strOut);
}



void libusbDemo::addUSBDevsList(QString devstr, USBListDesc desc)
{
    //USBDEVICE  USBCONFIG USBINTERFACE USBPONIT
    switch (desc)
    {
    case USBDEVICE:
        ui->devicesComBox->addItem(devstr);
        break;
    case USBCONFIG:
        ui->configComBox->addItem(devstr);
        break;
    case USBINTERFACE:
        ui->interfaceComBox->addItem(devstr);
        break;
    default:
        ui->pointComBox->addItem(devstr);
    }

}

void libusbDemo::setByteSpeed()
{
    if(time.elapsed()!=0)
        ui->speedLineEdit->setText(QString::number((double)byteCount/(time.elapsed()*0.001)));
    //time.restart();
    //byteCount = 0;
    ui->numSendLineEdit->setText(QString::number(byteSend));
    ui->numRecvLineEdit->setText(QString::number(byteRecv));
}

void libusbDemo::setTexkBrower()
{

    QString StrOut;
    int index = 0;
    timer_500ms_Mutex = true;
    StrOut.append("\n");
    for (int i =  0;i<PackageInLengh.length();++i)
    {
        index+=i;

        StrOut.append( StrRecv.mid(i*28,28));
        for (int j = 0; j < PackageInLengh.at(i); ++j)
        {

            StrOut.append( "0x"+tr("%1 ").arg((quint8) PackageIN.at(index+j),2,16,QLatin1Char('0')).toUpper());
        }
        StrOut.append("\n\n");
    }
    if(PackageInLengh.length()>0)
        ui->receiveTextBro->append(StrOut);

    PackageInLengh.clear();
    PackageIN.clear();
    StrRecv.clear();
    timer_500ms_Mutex =false;

    qDebug()<<"setTexkBrower:"<<QThread::currentThread()<<endl;

}

void libusbDemo::clearByteCounts()
{
    if(!time.isNull())
        time.restart();
    byteCount = 0;
   // qDebug()<<"searchUSB:"<<QThread::currentThread()<<endl;
}

void libusbDemo::on_clearBt_clicked()
{
    ui->receiveTextBro->clear();
    ui->textEdit->clear();
    //usbThread->searchUSB();
}


void libusbDemo::recvPackage(QByteArray BUF)
{

    PackageIN.append(BUF);
    byteCount+=BUF.length();
    byteRecv+=BUF.length();

    //qDebug()<<PackageIN.length()<<endl;
    QDateTime time;

    time = QDateTime::currentDateTime();

    QString strBuffer = time.toString("yyyy-MM-dd hh:mm:ss");
    StrRecv.append( tr("success:").toUpper()+strBuffer+"\n");
    //qDebug()<<StrRecv.length()<<endl;
    //byteCount+=BUF.length();
    //byteRecv+=BUF.length();
    //recvPackageNum +=1;
    PackageInLengh.append(BUF.length());
    /*qDebug()<<int(*(PackageInLengh.end()))<<PackageInLengh.length()<<endl;
    for (QList<int>::iterator i =  PackageInLengh.begin();
         i!=PackageInLengh.end();++i)
    {
        qDebug()<<*i<<endl;
    }*/
    //for (int i = 0; i < BUF.length(); ++i)
    //{
           //StrRecv.append( "0x"+tr("%1 ").arg((quint8)BUF.at(i),2,16,QLatin1Char('0')).toUpper());
    //}
    //StrRecv +="\n\n";
    //qDebug()<<"<StrRecv.length:"<<StrRecv.length()<<endl;
}

void libusbDemo::USBWrite()
{
    QByteArray array;
    QString str;
    str = ui->sendTextEdit->toPlainText();
    array = QByteArray::fromHex(str.toLatin1().data());
    /*if(array.length()!=512)
    {
        array.clear();
        for (int i = 0; i<512;i++)
        {
            array.append(i);
        }
    }*/
    if(array.length()>0)
    {
        bool ok = usbThread->USBWrite(array);
        if(ok){
            WirteNum = 0;
            //qDebug()<<WirteNum<<endl;
        }
        else
        {
            WirteNum += 1;
            //qDebug()<<WirteNum<<endl;
        }

    }
    if(WirteNum == 5&&timer_Write->isActive())
    {
        timer_Write->stop();
    }

    strOut.clear();
    strOut = "SUCCESS Write:"+QString::number(array.length())+"Byte\n";
    byteSend += array.length();

    ui->oneSendLineEdit->setText(QString::number(array.length()));
    /*for (int i = 0; i < array.length(); ++i)
    {
           strOut.append( "0x"+tr("%1 ").arg((quint8)array.at(i),2,16,QLatin1Char('0')).toUpper());
    }
    strOut +="\n";
    ui->textEdit->append(strOut);*/
}

void libusbDemo::changePushButton(bool flag)
{
    if(flag)
        ui->stopBtn->show();
    else
        ui->stopBtn->close();
}
#ifdef USBHOTPLUG_H
void libusbDemo::changeDevices(DEVICE_MESSAGE flag, unsigned short Vid, unsigned short Pid)//改变USB状态
{
    unsigned short VID;
    unsigned short PID;
    bool state = false;
    usbThread->getActiveVid_Pid(&state, &VID, &PID);
    switch (flag)
    {
    case DEVICE_CONNECT://设备连接
        ui->devicesComBox->clear();
        //usbThread->searchUSB();
        break;
    default://设备断开
#ifdef QT_DEBUG
        qDebug()<<"设备已断开！ PID状态："<<state<< hex<<VID<<":"<<PID<<"    "<<Vid<<":"<<Pid<<endl;
#endif
        if(state&&VID==Vid&&PID==Pid)//断开的设备是正在使用的设备
        {
            qDebug()<<"设备使用中，已断开！ PID状态："<<state<< hex<<VID<<":"<<PID<<"    "<<Vid<<":"<<Pid<<endl;
            ui->openBt->setText("open");
            ui->groupBox->close();
            ui->configComBox->clear();
            ui->interfaceComBox->clear();
            ui->pointComBox->clear();
            ui->devicesComBox->clear();
            if(thread->isRunning())
            {
                usbThread->close();
                thread->quit();
                thread->wait();
            }
            if(timer_100ms->isActive())
            {
               timer_100ms->stop();
            }
            if(timer_Write->isActive())
            {
                timer_Write->stop();
            }
            //Sleep(50);
            //usbThread->searchUSB();
        }
        else//设备不是正在使用的设备
        {
            #ifdef QT_DEBUG
            qDebug()<<"移除设备不是正在运行设备"<<endl;
            #endif
            ui->devicesComBox->clear();
            //usbThread->searchUSB();
        }
    }
    Sleep(50);
    usbThread->searchUSB();


}
#endif


libusbDemo::~libusbDemo()
{
    //libusb_free_device_list(dev_,1);
    //libusb_exit(NULL);
    if(thread->isRunning())
    {
        usbThread->close();
        thread->quit();
        thread->wait();
    }
    delete ui;

}


void libusbDemo::on_stopBtn_clicked()
{
    if(timer_Write->isActive())
    {
        timer_Write->stop();
    }
}

void libusbDemo::timer_500ms_Run()
{

    if(!timer_500ms_Mutex)
        QtConcurrent::run(this, &libusbDemo::setTexkBrower);
    //qDebug()<<"run:"<<QThread::currentThread()<<endl;
}
