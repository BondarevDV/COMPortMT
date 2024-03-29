#include "port.h"
#include <qdebug.h>

Port::Port(QObject *parent) :
    QObject(parent)
{
}

Port::~Port()
{
    qDebug("By in Thread!");
    emit finished_Port();
}

void Port::processPort(){
    qDebug("Start listen port");
    connect(&COMPort,SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(&COMPort, SIGNAL(readyRead()),this,SLOT(ReadInPort()));
}

void Port::WriteSettingsPort(QString name, int baudrate,int DataBits,
                         int Parity,int StopBits, int FlowControl){
    qDebug()<<"WriteSettingsPort";
    qDebug()<<"SettingsPort.name: "<< name;
    SettingsPort.name = name;
    SettingsPort.baudRate = static_cast<QSerialPort::BaudRate>(baudrate);
    SettingsPort.dataBits = static_cast<QSerialPort::DataBits>(DataBits);
    SettingsPort.parity = static_cast<QSerialPort::Parity>(Parity);
    SettingsPort.stopBits = static_cast<QSerialPort::StopBits>(StopBits);
    SettingsPort.flowControl = static_cast<QSerialPort::FlowControl>(FlowControl);
    this->ConnectPort();
}

void Port :: ConnectPort(void){//
    COMPort.setPortName(SettingsPort.name);
    qDebug()<<"ConnectPort: "<<SettingsPort.name;
    if (COMPort.open(QIODevice::ReadWrite)) {
        if (COMPort.setBaudRate(SettingsPort.baudRate)
                && COMPort.setDataBits(SettingsPort.dataBits)//DataBits
                && COMPort.setParity(SettingsPort.parity)
                && COMPort.setStopBits(SettingsPort.stopBits)
                && COMPort.setFlowControl(SettingsPort.flowControl))
        {
            if (COMPort.isOpen()){
                error_((SettingsPort.name.toLocal8Bit() + " >> Открыт!\r"));
            }
        } else {
            COMPort.close();
            error_(COMPort.errorString().toLocal8Bit());
          }
    } else {
        COMPort.close();
        error_(COMPort.errorString().toLocal8Bit());
    }
}

void Port::handleError(QSerialPort::SerialPortError error)//
{
    if ( (COMPort.isOpen()) && (error == QSerialPort::ResourceError)) {
        error_(COMPort.errorString().toLocal8Bit());
        DisconnectPort();
    }
}//


void  Port::DisconnectPort(){
    if(COMPort.isOpen()){
        COMPort.close();
        error_(SettingsPort.name.toLocal8Bit() + " >> Закрыт!\r");
    }
}

void Port :: WriteToPort(QByteArray data){
    qDebug()<< data;
    if(COMPort.isOpen()){
        COMPort.write(data);
    }
}

void Port::WriteForProtocolToPort(QByteArray data)
{
    // Сборка пакета и передача:
    data_transfer_protocol.shproto_packet_start(data_transfer_protocol.packet_tx, 0x03); //0x03 - код команды, для примера
    std::string tx = data.toStdString();
    for (int i = 0; i < data.length(); i++) {
        data_transfer_protocol.shproto_packet_add_data(data_transfer_protocol.packet_tx, (unsigned char)tx[i]); //Добавляем один байт данных, для примера он равен нулю
    }
    data_transfer_protocol.shproto_packet_complete(data_transfer_protocol.packet_tx); //Добавляем маркер конца пакета и контрольную сумму
    //output(data_transfer_protocol.packet_tx.data, data_transfer_protocol.packet_tx.len); //Передаём пакет через UART
    QByteArray data_tx;

    for (int i = 0; i < data_transfer_protocol.packet_tx->len ; i++) {
        data_tx.append(data_transfer_protocol.packet_tx->data[i]);
    }
    if(COMPort.isOpen()){
        COMPort.write(data_tx);
    }
}

void Port :: ReadForProtocolInPort(){
    QByteArray data;
    data.append(COMPort.readAll());
    for (int i = 0; i < data.size(); i++) {
        control_usb_data.fifo_set(static_cast<unsigned char>(data.at(i)));
    }
    outPort(QString("size = %1").arg(QString::number(control_usb_data.count())));
    while (!control_usb_data.fifo_is_empty()){
        outPort(QString::number(control_usb_data.fifo_get()));
    }

    //((QString)(adr.toInt())).toLatin1().toHex()
}

//
void Port :: ReadInPort(){
    QByteArray data;
    data.append(COMPort.readAll());
    for (int i = 0; i < data.size(); i++) {
        control_usb_data.fifo_set(static_cast<unsigned char>(data.at(i)));
    }
    outPort(QString("size = %1").arg(QString::number(control_usb_data.count())));
    while (!control_usb_data.fifo_is_empty()){
        outPort(QString::number(control_usb_data.fifo_get()));
    }

    //((QString)(adr.toInt())).toLatin1().toHex()
}

