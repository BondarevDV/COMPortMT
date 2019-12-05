#ifndef PORT_H
#define PORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "controller.h"
//#include "shproto.h"
#include "qshproto.h"


struct Settings {
    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};

class Port : public QObject
{
    Q_OBJECT

public:

    explicit Port(QObject *parent = nullptr);
    qshproto data_transfer_protocol;

    ~Port();

    QSerialPort COMPort;

    Settings SettingsPort;
    Controller control_usb_data;

signals:

    void finished_Port();

    void error_(QString err);

    void outPort(QString data);

public slots:

    void  DisconnectPort();

    void ConnectPort(void);

    void WriteSettingsPort(QString name, int baudrate, int DataBits, int Parity, int StopBits, int FlowControl);

    void processPort();

    void WriteToPort(QByteArray data);
    void WriteForProtocolToPort(QByteArray data);
    void ReadForProtocolInPort();

    void ReadInPort();

private slots:

    void handleError(QSerialPort::SerialPortError error);//

public:

};

#endif // PORT_H
