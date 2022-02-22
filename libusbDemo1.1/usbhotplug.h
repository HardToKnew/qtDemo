#ifndef USBHOTPLUG_H
#define USBHOTPLUG_H

#include <QWidget>
#include <qt_windows.h>
#include <Dbt.h>
#include <QMetaType>
enum DEVICE_MESSAGE
{
    DEVICE_CONNECT,
    DEVICE_DISCONNECT
};

class USBHotplug: public QWidget
{
    Q_OBJECT

public:
    explicit USBHotplug(QWidget *parent = 0);
    ~USBHotplug();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:

    /*!
     * \brief registerDevice 注册设备
     */
    void registerDevice();

signals:
    void changeDevice(DEVICE_MESSAGE flag, unsigned short Vid, unsigned  short Pid);
};

#endif // DIALOG_H
