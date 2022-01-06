#ifndef SENDDATATOTCP_H
#define SENDDATATOTCP_H

#include <QMainWindow>
#include <QFileDialog>
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

private:
    Ui::SendDataToTcp *ui;
};
#endif // SENDDATATOTCP_H
