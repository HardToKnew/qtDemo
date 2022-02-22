#ifndef USBTHREAD_H
#define USBTHREAD_H

#include <QObject>
#include "libusb/libusb.h"
#include <QThread>
#include <QWidget>


#define RECV_LEN 4096
typedef enum {
    USBDEVICE = 0, USBCONFIG, USBINTERFACE, USBPONIT
}USBListDesc;
class USBThread :public QObject
{
    Q_OBJECT
public:
    explicit USBThread(QObject *parent = nullptr);
    ~USBThread();

    void searchUSB();
    void runUSBThread(int devs_index);
    void close();
    bool USBWrite(QByteArray buf);
    void getActiveVid_Pid(bool *state, unsigned short *VID, unsigned short *PID);




private:
    QThread thread;
    libusb_device **dev_;//all usb devices list
    libusb_device *devs[256] ;//使用libusb驱动的usb设备列表
    libusb_device *_dev ;//选中的usb设备
    libusb_device_handle *handle ;



    int recv_len;
    bool is_open;
    bool writeEnable = true;
    bool write_SYS = false;//写过数据标志
    int readFaildNum = 0; //读失败计数


    unsigned char recv_buf[RECV_LEN];



private slots:
    int USBRead(unsigned char*buf, int LEN);





signals:
    void sendDevsDesc(QString str, USBListDesc desc);
    void sendPackage(QByteArray buf);

};

#endif // USBTHREAD_H
