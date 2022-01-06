#ifndef GETIPADDRESS_H
#define GETIPADDRESS_H

#include <QMainWindow>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QProcess>
#include <QNetworkSession>

QT_BEGIN_NAMESPACE
namespace Ui { class getIPAddress; }
QT_END_NAMESPACE

class getIPAddress : public QMainWindow
{
    Q_OBJECT

public:
    getIPAddress(QWidget *parent = nullptr);
    ~getIPAddress();

private slots:
    void on_getIPBt_clicked();
    void on_getInterBt_clicked();
    void on_setIPBt_clicked();

private:
    Ui::getIPAddress *ui;

    void addBroswer(QString str);
    void clearBroswer();
};
#endif // GETIPADDRESS_H
