#include "libusbDemo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    libusbDemo w;
    w.show();
    //w.setWindowIcon(QIcon(":/images/icon.ico"));
    return a.exec();
}
