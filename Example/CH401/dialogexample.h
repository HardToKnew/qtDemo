#ifndef DIALOGEXAMPLE_H
#define DIALOGEXAMPLE_H

#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QFrame>
#include "inputdlg.h"
class DialogExample : public QDialog
{
    Q_OBJECT

public:
    DialogExample(QWidget *parent = nullptr);
    ~DialogExample();

private:

    /*标准文件对话框*/
    QPushButton *fileBtn;
    QLineEdit *fileLineEdit;
    QGridLayout *mainLayout;
    /*标准颜色对话框*/
    QPushButton *colorBtn;
    QFrame *colorFrame;
    /*标准字体对话框*/
    QPushButton *fontBtn;
    QLineEdit *fontLineEdit;
    /*标准对话框实例*/
    InputDlg *inputDlg;
    QPushButton *inputBtn;


private slots:
    void showFile();
    void showColor();
    void showFont();
    void showInputDlg();


};
#endif // DIALOGEXAMPLE_H
