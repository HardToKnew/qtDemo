#ifndef READFILE_H
#define READFILE_H

#include <QObject>
#include <iostream>
#include <fstream>
#include <string>
#include "protocol.h"
using namespace std;

/*struct   dDATA
{
    unsigned char data[32768];
    unsigned short size = 0;
    unsigned short index = 0;
};
struct   SPECTUM
{
    uint64_t channel[16384];
    unsigned short size = 0;
    unsigned short index = 0;
};*/
class readFile:public QObject
{
    Q_OBJECT
public:
    readFile(QObject *parent = nullptr);

    void getFileData(QString Filename);

    //void setProtocol();

private:
    dDATA dData;
    SPECTUM spectum;

    void setProtocol();

    void setCheckSum();

    bool isDigitStr(string src);

    unsigned short CRC16_CCITT_TABLE(unsigned char *puchMsg, unsigned short usDataLen);

signals:
    void readStop(dDATA* buf);
};

#endif // READFILE_H
