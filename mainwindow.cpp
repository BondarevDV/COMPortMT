#include "mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QDesktopWidget>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <errno.h>
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    graph = new Form();
    histogram = new Graph();
    connect(ui->BaudRateBox, SIGNAL(currentIndexChanged(int)) ,this, SLOT(checkCustomBaudRatePolicy(int)));
    ui->BaudRateBox->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    ui->BaudRateBox->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    ui->BaudRateBox->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    ui->BaudRateBox->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    ui->BaudRateBox->addItem(QLatin1String("Custom"));
   // fill data bits
    ui->DataBitsBox->addItem(QLatin1String("5"), QSerialPort::Data5);
    ui->DataBitsBox->addItem(QLatin1String("6"), QSerialPort::Data6);
    ui->DataBitsBox->addItem(QLatin1String("7"), QSerialPort::Data7);
    ui->DataBitsBox->addItem(QLatin1String("8"), QSerialPort::Data8);
    ui->DataBitsBox->setCurrentIndex(3);
   // fill parity
    ui->ParityBox->addItem(QLatin1String("None"), QSerialPort::NoParity);
    ui->ParityBox->addItem(QLatin1String("Even"), QSerialPort::EvenParity);
    ui->ParityBox->addItem(QLatin1String("Odd"), QSerialPort::OddParity);
    ui->ParityBox->addItem(QLatin1String("Mark"), QSerialPort::MarkParity);
    ui->ParityBox->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);
   // fill stop bits
    ui->StopBitsBox->addItem(QLatin1String("1"), QSerialPort::OneStop);
    #ifdef Q_OS_WIN
    ui->StopBitsBox->addItem(QLatin1String("1.5"), QSerialPort::OneAndHalfStop);
    #endif
    ui->StopBitsBox->addItem(QLatin1String("2"), QSerialPort::TwoStop);
   // fill flow control
    ui->FlowControlBox->addItem(QLatin1String("None"), QSerialPort::NoFlowControl);
    ui->FlowControlBox->addItem(QLatin1String("RTS/CTS"), QSerialPort::HardwareControl);
    ui->FlowControlBox->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);
    connect(ui->cBtnSend,SIGNAL(clicked()),this, SLOT(on_cEnterText_returnPressed()) );

    QThread *thread_New = new QThread;//Создаем поток для порта платы
    Port *PortNew = new Port();//Создаем обьект по классу
    PortNew->moveToThread(thread_New);//помешаем класс  в поток
    PortNew->COMPort.moveToThread(thread_New);//Помещаем сам порт в поток
    connect(PortNew, SIGNAL(error_(QString)), this, SLOT(Print(QString)));//Лог ошибок
    connect(thread_New, SIGNAL(started()), PortNew, SLOT(processPort()));//Переназначения метода run
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(quit()));//Переназначение метода выход
    connect(thread_New, SIGNAL(finished()), PortNew, SLOT(deleteLater()));//Удалить к чертям поток
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(deleteLater()));//Удалить к чертям поток
    connect(this,SIGNAL(savesettings(QString,int,int,int,int,int)),PortNew,SLOT(WriteSettingsPort(QString,int,int,int,int,int)));//Слот - ввод настроек!
    // connect(ui->BtnConnect, SIGNAL(clicked()),PortNew,SLOT(ConnectPort()));
    connect(ui->BtnConnect, SIGNAL(clicked()),this,SLOT(on_BtnSave_clicked()));
    connect(ui->BtnDisconect, SIGNAL(clicked()),PortNew,SLOT(DisconnectPort()));
    connect(PortNew, SIGNAL(outPort(QString)), this, SLOT(Print(QString)));//Лог ошибок
    connect(this,SIGNAL(writeData(QByteArray)),PortNew,SLOT(WriteToPort(QByteArray)));
    connect(ui->btn_graph, SIGNAL(clicked()), this, SLOT(CreateGraph()));
    thread_New->start();
    this->on_Btn_Search_clicked();
    connect(PortNew,SIGNAL(outPort(QByteArray)), histogram, SLOT(recieveData(QByteArray)));
}
//++++++++[Процедура закрытия приложения]+++++++++++++++++++++++++++++++++++++++++++++
MainWindow::~MainWindow()
{

    delete ui; //Удаление формы
}

//++++++++[Процедура пределения подключенных портов]+++++++++++++++++++++++++++++++++++
void MainWindow::on_Btn_Search_clicked()
{
    ui->PortNameBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
        ui->PortNameBox->addItem(info.portName());
        }
}

void MainWindow::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->BaudRateBox->itemData(idx).isValid();
    ui->BaudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->BaudRateBox->clearEditText();
    }
}

//+++++++++++++[Процедура ввода данных из строки]++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_cEnterText_returnPressed()
{
   QByteArray data;
   QDataStream stream(&data, QIODevice::WriteOnly);
   QStringList longList = ui->cEnterText->text().split(QRegExp("\\s+"), QString::SkipEmptyParts);
   qDebug()<<longList.size();
   bool ok;
   for (int i = 0; i < longList.size(); i++) {
       qDebug()<<sizeof(static_cast<qint8>(longList.at(i).toInt(&ok, 16)));
       data.append(static_cast<qint8>(longList.at(i).toInt(&ok, 16)));
   }
    qDebug()<<data.length();
    writeData(data); // Отправка данных в порт
    Print(QString::fromStdString(data.toHex().toStdString())); // Вывод данных в консоль
}



//+++++++++++++[Процедура вывода данных в консоль]++++++++++++++++++++++++++++++++++++++++
void MainWindow::Print(QString data)
{
    ui->consol->textCursor().insertText(data + '\r'); // Вывод текста в консоль
    ui->consol->moveCursor(QTextCursor::End);//Scroll
}

void MainWindow::on_BtnSave_clicked()
{
    qDebug()<< "on_BtnSave_clicked";
    qDebug()<< ui->PortNameBox->currentText();
    savesettings(ui->PortNameBox->currentText(),
                 ui->BaudRateBox->currentText().toInt(),
                 ui->DataBitsBox->currentText().toInt(),
                 ui->ParityBox->currentText().toInt(),
                 ui->StopBitsBox->currentText().toInt(),
                 ui->FlowControlBox->currentText().toInt());

}

void MainWindow::CreateGraph()
{
    qDebug() << "Create graph";
    histogram->show();
    // graph->show();
}

void MainWindow::recieveData(QByteArray data)
{
     qDebug() << "recieveData";
     sendData(data);
}




