#ifndef PROTOCOL_H
#define PROTOCOL_H
struct   dDATA
{
    unsigned char data[32768];
    unsigned short size = 0;
    unsigned short index = 0;
};
struct   SPECTUM
{
    unsigned long long channel[16384];
    unsigned short size = 0;
    unsigned short index = 0;
};
#endif // PROTOCOL_H
