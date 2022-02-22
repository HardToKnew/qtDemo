#include "dialogexample.h"
#include <QPushButton>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>

DialogExample::DialogExample(QWidget *parent)
    : QDialog(parent)
{

    fileBtn = new QPushButton;
    fileBtn->setText(tr("文件标准对话框实例"));
    fileLineEdit = new QLineEdit;
    colorBtn = new QPushButton;
    colorBtn->setText(tr("颜色校准对话框实例"));
    colorFrame = new QFrame;
    colorFrame->setFrameShape(QFrame::Box);
    colorFrame->setAutoFillBackground(true);
    fontBtn = new QPushButton;
    fontBtn->setText(tr("字体标准对话框实例"));
    fontLineEdit = new QLineEdit;
    fontLineEdit->setText(tr("Welcome!"));
    inputBtn = new QPushButton;
    inputBtn->setText(tr("标准输入对话框实例"));

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(fileBtn, 0, 0);
    mainLayout->addWidget(fileLineEdit, 0, 1);
    mainLayout->addWidget(colorBtn, 1, 0);
    mainLayout->addWidget(colorFrame, 1, 1);
    mainLayout->addWidget(fontBtn, 2, 0);
    mainLayout->addWidget(fontLineEdit, 2, 1);
    mainLayout->addWidget(inputBtn, 3, 0);

    connect(fileBtn, SIGNAL(clicked()), this, SLOT(showFile()));
    connect(colorBtn, SIGNAL(clicked()), this, SLOT(showColor()));
    connect(fontBtn, SIGNAL(clicked()), this, SLOT(showFont()));
    connect(inputBtn, SIGNAL(clicked()), this, SLOT(showInputDlg()));

}

DialogExample::~DialogExample()
{
}

void DialogExample::showFile()
{
    QString s = QFileDialog::getOpenFileName(this, "open file dialog", "/", "C++ files(*cpp);; C files(*.c);;Head files(*.h)");
    fileLineEdit->setText(s);
}


void DialogExample::showColor()
{
    QColor c = QColorDialog::getColor(Qt::blue);
    if(c.isValid())
    {
        colorFrame->setPalette(QPalette(c));
    }
}

void DialogExample::showFont()
{
    bool ok;
    QFont f =QFontDialog::getFont(&ok);
    if(ok)
    {
        fontLineEdit->setFont(f);
    }
}

void DialogExample::showInputDlg()
{
    inputDlg = new InputDlg(this);
    inputDlg->setMinimumSize(200, 200);//不加这一句，报错"setGeometry: Unable to set geometry"
    //inputDlg->setWindowFlags(/*Qt::WindowCloseButtonHint | */Qt::MSWindowsFixedSizeDialogHint);
    inputDlg->show();
}
