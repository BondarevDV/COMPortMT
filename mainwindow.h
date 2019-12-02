#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "port.h"
#include "qcustomplot.h"
#include  "form.h"
#include  "graph.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
    Form* graph;
    Graph* histogram;

private:
    QByteArray received_data;
signals:
    void savesettings(QString name, int baudrate, int DataBits, int Parity, int StopBits, int FlowControl);
    void writeData(QByteArray data);
    void sendData(QByteArray data);

private slots:
    void on_Btn_Search_clicked();
    void checkCustomBaudRatePolicy(int idx);
    void Print(QString data);
    void on_cEnterText_returnPressed();
    void on_BtnSave_clicked();
    void CreateGraph();
    void recieveData(QByteArray data);



private:
    Ui::MainWindow *ui;
    Port PortNew;//

};

#endif // MAINWINDOW_H
