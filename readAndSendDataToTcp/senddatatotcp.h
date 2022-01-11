#ifndef SENDDATATOTCP_H
#define SENDDATATOTCP_H

#include <QMainWindow>
#include <QFileDialog>
#include <QHostInfo>
#include <QThread>
#include <QTimer>
#include "readfile.h"
#include "udpservice.h"
QT_BEGIN_NAMESPACE
namespace Ui { class SendDataToTcp; }
QT_END_NAMESPACE


class SendDataToTcp : public QMainWindow
{
    Q_OBJECT

public:
    SendDataToTcp(QWidget *parent = nullptr);
    ~SendDataToTcp();

private slots:
    void on_OpenFileBt_clicked();

    void on_closeBt_clicked();

    void on_sendBt_clicked();

    void on_pushButton_clicked();

private:
    Ui::SendDataToTcp *ui;
    QThread *threadFile;
    readFile *m_readFile;
    QThread *threadUdpService;
    udpService *m_UdpService;

    dDATA *data;


    QString getIPv4();

    QString getRandPort();

    void showData(dDATA* buf);
    void showRecvData(char* data, unsigned short size);
    void test();
signals:
    void startReadFileThread(QString filename);
    void startUdpServiceThread(QHostAddress localIP, quint16 localPort, QHostAddress serviceIP, quint16 servicePort, dDATA *buf, int time);


};
#endif // SENDDATATOTCP_H
