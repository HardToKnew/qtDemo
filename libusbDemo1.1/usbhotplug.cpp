#include "usbhotplug.h"

#include <QDebug>
//#include <QTimer>
//#include <QPalette>
//#include <QDateTime>
#include <QThread>

//#pragma comment(lib, "User32.lib")

USBHotplug::USBHotplug(QWidget *parent) :
        QWidget(parent)

{
    qRegisterMetaType<DEVICE_MESSAGE>("DEVICE_MESSAGE");
    registerDevice();
}

USBHotplug::~USBHotplug()
{
    //delete ui;
}

void USBHotplug::registerDevice()
{

    //需要监听的设备GUID
    const GUID GUID_DEVINTERFACE_LIST[] = {
        { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },//USB设备的GUID  //所有usb设备都具有该ID
        { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } } };
        //{ 0x1f97d968, 0x2c70, 0x469e, { 0x87, 0x8b, 0xd9, 0xd4, 0x63, 0xa2, 0xce, 0x82 } } };
        //{ 0x36FC9E60, 0xC465, 0x11CF, { 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } } };

    HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotifacationFiler;
    ZeroMemory(&NotifacationFiler,sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    NotifacationFiler.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotifacationFiler.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

    for (int i = 0; i < sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID); i++)  {
        NotifacationFiler.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
        hDevNotify = RegisterDeviceNotification((HANDLE)this->winId(), &NotifacationFiler, DEVICE_NOTIFY_WINDOW_HANDLE);
        if (!hDevNotify) {
            qCritical() << QStringLiteral("注册失败！");
        }
       //qDebug()<<i<<endl;
    }
}

bool USBHotplug::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);

    MSG *msg = reinterpret_cast<MSG *>(message);
    int msgType = msg->message;
    if (WM_DEVICECHANGE == msgType) {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch (msg->wParam) {
        case DBT_DEVICEARRIVAL:
        {
            if (DBT_DEVTYP_DEVICEINTERFACE == lpdb->dbch_devicetype)
            {
                //qDebug()<<"USB设备插入"<<endl;
                PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
                QString name = QString::fromWCharArray(pDevInf->dbcc_name);

                if (!name.isEmpty())
                {
                    if (name.contains("USB"))
                    {
                        QStringList listAll = name.split('#');
                        QStringList listID = listAll.at(1).split('&');
                        bool ok;
                        emit changeDevice(DEVICE_CONNECT, listID.at(0).right(4).toUShort(&ok, 16),listID.at(1).right(4).toUShort(&ok, 16));
                    }
                }
            }
        }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
        {
            if (DBT_DEVTYP_DEVICEINTERFACE == lpdb->dbch_devicetype)
            {
                 //qDebug()<<"USB设备拔出"<<endl;
                 PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
                 QString name = QString::fromWCharArray(pDevInf->dbcc_name);

                 if (!name.isEmpty())
                 {
                     if (name.contains("USB"))
                     {
                         QStringList listAll = name.split('#');
                         QStringList listID = listAll.at(1).split('&');
                         bool ok;
                         emit changeDevice(DEVICE_DISCONNECT, listID.at(0).right(4).toUShort(&ok, 16),listID.at(1).right(4).toUShort(&ok, 16));//VID  PID
                     }
                 }

            }
        }
            break;
        }
    }

    return false;
}

