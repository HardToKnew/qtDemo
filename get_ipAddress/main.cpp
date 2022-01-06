#include "getipaddress.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    getIPAddress w;
    w.show();
    return a.exec();
}
