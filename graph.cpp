#include "graph.h"
#include "ui_graph.h"

Graph::Graph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Graph)
{
    ui->setupUi(this);
    gistogram = new CGistogram();
    ui->gridLayout->addWidget(gistogram,0,0,1,1);  // Устанавливаем CGistogram в окно проложения

}

Graph::~Graph()
{
    delete ui;
}

void Graph::recieveData(QByteArray &data)
{
    qDebug()<<"graph recieveData: "<< data;
}
