#include "usbthread.h"
#include <QThread>
#include <QDebug>
#define EP0ADDR 0x02//Write端点
#define EP1ADDR 0x86//Read端点
//#define EP1ADDR 0x81//Read端点 C8051F340
USBThread::USBThread(QObject *parent) : QObject(parent)
{

    #ifdef QT_DEBUG
        qDebug()<<"USBThread:"<<QThread::currentThread()<<endl;
    #endif


#ifdef _WIN32 //32位和64位 win

    #ifdef _WIN64 //64位 win
        qDebug() << "Windows X64" << endl;
    #else
        qDebug() << "Windows X86" << endl;
    #endif
#else
    qDebug() << "Linux" << endl;
#endif


}

USBThread::~USBThread()
{
     qDebug()<<"~USBThread:"<<QThread::currentThread()<<endl;
    libusb_free_device_list(dev_,1);
    libusb_exit(NULL);
}

void USBThread::searchUSB()
{
    qDebug()<<"searchUSB:"<<QThread::currentThread()<<endl;
    libusb_device *dev;

    int i=0;
    int ret;
    unsigned char str[512];
    ssize_t cnt;
    libusb_init(NULL);
    //LibK_Context_Init();
            //OutputDebugString();
    cnt = libusb_get_device_list(NULL, &dev_);

    cnt = 0;
    if(is_open)//如果设备正在运行保存该设备
    {
        devs[cnt] = _dev;
        cnt = 1;
        struct libusb_device_descriptor desc;
        ret = libusb_get_device_descriptor(_dev,&desc);
        ret = libusb_get_string_descriptor_ascii(handle,desc.iProduct,str, sizeof(str));
        emit sendDevsDesc((QString::fromStdString((char*)str)), USBDEVICE);
    }
    while ((dev = dev_[i++]) != NULL)
    {

        libusb_device_handle *handle = NULL;
        struct libusb_device_descriptor desc;
        ret = libusb_get_device_descriptor(dev,&desc);

        if(handle == NULL&&ret>=0)
            ret =libusb_open(dev,&handle);


        if(handle)
        {
            if(desc.iProduct)
            {
                ret = libusb_get_string_descriptor_ascii(handle,desc.iProduct,str, sizeof(str));

                if(ret>0&&cnt<256)
                {
                    devs[cnt] = dev;
                    emit sendDevsDesc((QString::fromStdString((char*)str)), USBDEVICE);
                    //qDebug()<<QString::fromStdString((char*)str)<<endl;
                    cnt++;
                }

            }
            libusb_close(handle);
        }
    }
}

void USBThread::runUSBThread(int devs_index)
{
#ifdef QT_DEBUG
    qDebug()<<"runUSBThread:"<<QThread::currentThread()<<"   "<<devs_index<<endl;
#endif

    is_open = true;
    write_SYS = false;//写过数据标志清零
    writeEnable = true;//开启写数据

    //libusb_device *dev ;//选中的usb设备
    _dev = devs[devs_index];
    handle = NULL;

    int ret = libusb_open(_dev,&handle); //打开设备，获取设备操作符
    if(ret<0)
    {
        exit(1);
    }

    struct libusb_device_descriptor desc;  //设备信息描述符
    ret= libusb_get_device_descriptor(_dev, &desc); //获取设备信息描述符
    if (ret < 0)
    {
        return;
    }


    /**********************获取配置信息****************************/
    struct libusb_config_descriptor *bConfig;//配置信息
    ret = libusb_get_active_config_descriptor(_dev, &bConfig);
    for (int i=0;i<(int)desc.bNumConfigurations;i++)
    {

        QString strPoint = "Config: "+QString::number(bConfig[i].bConfigurationValue, 16);
        emit sendDevsDesc(strPoint, USBCONFIG);

        for (int j=0;j<(int) bConfig[i].bNumInterfaces;j++)
        {

            QString strOut = "bInterface: "+QString::number(bConfig[i].interface[j].altsetting->bInterfaceNumber, 16);
            emit sendDevsDesc(strOut, USBINTERFACE);

            int pointNum = (int)bConfig[i].interface[j].altsetting->bNumEndpoints;



            for (int k = 0;k<pointNum;k++)
            {
                strOut = "EndPoint: "+QString::number(bConfig[i].interface[j].altsetting->endpoint[k].bEndpointAddress, 16);

                if(bConfig[i].interface[j].altsetting->endpoint[k].bEndpointAddress&0x80)
                    emit sendDevsDesc(strOut+" IN", USBPONIT);
                else
                   emit sendDevsDesc(strOut+" OUT", USBPONIT);
            }
        }
    }


    //设置配置和接口

    libusb_set_auto_detach_kernel_driver (handle,0);
    //libusb规定在用设备操作符发送数据前，需设置usb设备的配置模式和接口
    ret=libusb_set_configuration(handle, 1);//bConfigurationValue  is 1;
    if (ret< 0)
    {  //设置usb设备配置模式为1
        exit(1);
    }


    ret=libusb_claim_interface(handle, 0);
    if (ret < 0)
    {   //设置usb设备接口为0
        exit(1);
    }

    while (is_open)
    {
        recv_len = 1024;
        int ret = USBRead(recv_buf, recv_len);
        if(ret>=0)
        {
            QByteArray buf((char*)recv_buf, recv_len);
            write_SYS = false;
            writeEnable = true;
            readFaildNum = 0;

            emit sendPackage(buf);
        }
        else
        {
#ifdef QT_DEBUG
    //qDebug()<<"USBReade:faild!"<<QThread::currentThread()<<readFaildNum<<" "<<writeEnable<<endl;
#endif
    readFaildNum+=1;
    Sleep(500);
    if(write_SYS&&readFaildNum ==5)
        writeEnable = false;


        }
        //QByteArray buf((char*)recv_buf, 6);
        //emit sendPackge(buf);
        Sleep(1);
    }
}

void USBThread::close()
{
    is_open = false;
    libusb_close(handle);

}

bool USBThread::USBWrite(QByteArray buf)
{
#ifdef QT_DEBUG
    //qDebug()<<"USBWrite:"<<QThread::currentThread()<<"   "<<writeEnable<<endl;
#endif
    write_SYS = true;
    if(!writeEnable)//禁止写入
        return false;
    int size = buf.length();
    int ret = -1;
    if(handle)
        ret = libusb_bulk_transfer(handle,EP0ADDR,(unsigned char *)buf.data(),size,&size,500);  //以块传输方式发送16进制数据 选择端口号为0x01
    if (ret!=0)
    {
#ifdef QT_DEBUG
    //qDebug()<<"USBWrite:faild!"<<QThread::currentThread()<<endl;
#endif
        Sleep(50);
        return false;
    }
    return true;
}

void USBThread::getActiveVid_Pid(bool *state, unsigned short *VID, unsigned short *PID)
{
    if(!is_open)
    {
        *state = false;
        return;
    }
    struct libusb_device_descriptor desc;
    int ret = libusb_get_device_descriptor(_dev, &desc);
    if(ret<0)
    {
        *state = false;
        return;
    }

    *state = true;
    *VID = desc.idVendor;
    *PID = desc.idProduct;
}

int USBThread::USBRead(unsigned char*buf, int LEN)
{

    int ret=-1;
    if(handle)
        ret = libusb_bulk_transfer(handle, EP1ADDR, buf, LEN, &LEN,500);
    //以块传输方式接收16进制数据 选择端口号为0x82
    //libusb_bulk_transfer函数参数解释：设备操作符 端口地址 要接受的数据 预计接受的字节数 实际接受的字节数 超时时间
    //libusb_bulk_transfer根据端口决定此时函数是接收还是发送数据

    return ret;
}


