# 项目介绍

* 使用线程打开MCA文件，读取数据并添加包头文件，将数据转换为三字节数据
* 使用CRC-16CCITT查表算法计算校验位并添加到数据末尾
* 使用开始发送按钮开启新线程，进行定时使用UDP发送数据，同时接收目的地址和IP的数据进行显示

###### 数据格式

|        | SYNC1 | SYNC2 | PID1 | PID2 | LEN MSB | LEN LSB | Data field | CRC MSB | CRC LSB |
| ------ | ----- | ----- | ---- | ---- | ------- | ------- | ---------- | ------- | ------- |
| Offset | 0     | 1     | 2    | 3    | 4       | 5       | 6-12293    | 12294   | 12295   |
| Value  | 0xF5  | 0xFA  | 0x81 | 0x09 | 0x30    | 0       | data       | CRC-16  |         |



每个数据3字节，LSB-MSB 高字节在后，byte0,byte1,byte2

data[0] = byte0+byte1$\times$​256+byte2$\times$​256$\times$​256



