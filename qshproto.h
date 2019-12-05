#ifndef QSHPROTO_H
#define QSHPROTO_H

#include <QObject>


    #define SHPROTO_START  (0xFE|0x80)
    #define SHPROTO_ESC    (0xFD|0x80)
    #define SHPROTO_FINISH (0xA5|0x80)
    typedef struct {
        unsigned char * data;
        unsigned short buffer_size;
        unsigned short len;
        unsigned short crc;
        unsigned char ready;
        unsigned char complete;
        unsigned char cmd;
        unsigned char esc;
    } shproto_struct;

// crc16 (modbus algo)
    unsigned short crc16(unsigned short crc, unsigned char data) {
        unsigned char i;
        crc = crc ^ data;
        for (i = 0; i < 8; ++i) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc = (crc >> 1);
            }
        }
        return crc;
    }




class qshproto : public QObject
{
    Q_OBJECT
public:
    explicit qshproto(QObject *parent = nullptr);
    static unsigned char packet_buff_tx[1024];
    shproto_struct* packet_tx;
    void init();

signals:


public slots:
    void shproto_packet_start(shproto_struct * shproto_packet, unsigned char cmd);
    void shproto_packet_add_data(shproto_struct * shproto_packet, unsigned char tx_byte);
    unsigned short shproto_packet_complete(shproto_struct * shproto_packet);
    void shproto_byte_received(shproto_struct * shproto_packet, unsigned char rx_byte);
};

#endif // QSHPROTO_H
