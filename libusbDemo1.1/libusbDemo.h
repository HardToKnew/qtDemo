#ifndef LIBUSBDEMO_H
#define LIBUSBDEMO_H

#include <QMainWindow>
#include <iostream>
#include <QTimer>
#include <QTime>
#include <QString>
#include <QtConcurrent>
#include "libusb/libusb.h"
#include "usbthread.h"
#ifdef USBTHREAD_H
#include "usbhotplug.h"
#endif
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class libusbDemo; }
QT_END_NAMESPACE

class libusbDemo : public QMainWindow
{
    Q_OBJECT

public:
    libusbDemo(QWidget *parent = nullptr);
    ~libusbDemo();

private slots:

    void on_openBt_clicked();

    void on_sendBt_clicked();

    void on_clearBt_clicked();


    void addUSBDevsList(QString devsStr, USBListDesc desc);
    void setByteSpeed();
    void setTexkBrower();
    void clearByteCounts();
    void recvPackage(QByteArray BUF);
    void USBWrite();
    void changePushButton(bool flag);
#ifdef USBHOTPLUG_H
    void changeDevices(DEVICE_MESSAGE flag, unsigned short Vid, unsigned short Pid);
#endif





void on_stopBtn_clicked();

private:
    Ui::libusbDemo *ui;
    USBThread *usbThread;
    QThread *thread;
    QThread *thread_timer_250ms;
    QTimer *timer_100ms;
    QTimer *timer_250ms;
    QTimer *timer_Write;
    QTimer *timer_clear;
    double byteCount = 0;
    int byteSend = 0;
    int byteRecv = 0;
    int WirteNum = 0;
    QTime time, timeStop;
    QString strOut;
    QByteArray PackageIN;
    QString StrRecv;
    QList<int> PackageInLengh;
    int recvPackageNum = 0;

    bool timer_500ms_Mutex = false;//线程锁标志

    void timer_500ms_Run();

#ifdef USBHOTPLUG_H
    USBHotplug usbHotplug;
#endif

signals:
    void startUSB(int devs_index);

};
#endif // LIBUSBDEMO_H
