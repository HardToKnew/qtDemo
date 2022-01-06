#include "senddatatotcp.h"
#include "ui_senddatatotcp.h"
#include <QDebug>
#include <iostream>
SendDataToTcp::SendDataToTcp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SendDataToTcp)
{
    ui->setupUi(this);
    //获取本地以太网IP
    //获取随机端口号
}

SendDataToTcp::~SendDataToTcp()
{
    delete ui;
}


void SendDataToTcp::on_OpenFileBt_clicked()
{
    QFileDialog fd;
    fd.setAcceptMode(QFileDialog::AcceptOpen);//文件对话框为打开类型
    fd.setViewMode(QFileDialog::List);//仅列表
    fd.setWindowTitle("标题");
    fd.setFileMode(QFileDialog::ExistingFile);//存在的，单个文件名

    //fd.setFilter(TR("Image Files(*.jpg *.png))");//设置文件类型过滤器
    QStringList filters;
    filters << "mca files (*.mca *.MCA)"
            << "Any files (*)";
    fd.setNameFilters(filters);


    if(fd.exec() == QDialog::Accepted) {//注意使用的是QFileDialog::Accepted或者QDialog::Accepted,不是QFileDialog::Accept
            QString path = fd.selectedFiles()[0];//得到用户选择的文件名

            //读取文件
            //协议转换
            //添加校验位
            //显示

    }
}


void SendDataToTcp::on_closeBt_clicked()
{
    //取消监听
    //取消数据放送
    //线程结束


}

void SendDataToTcp::on_sendBt_clicked()
{
    //判断TCP是否运行
    //根据端口号开启TCP监听，线程接收数据
    //按照设定时间 开启线程  定时向设定IP端口号发送数据
}
