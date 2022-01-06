#include "senddatatotcp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SendDataToTcp w;
    w.show();
    return a.exec();
}
