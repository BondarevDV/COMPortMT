#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include "cgistogram.h"

namespace Ui {
class Graph;
}

class Graph : public QWidget
{
    Q_OBJECT

public:
    explicit Graph(QWidget *parent = nullptr);
    ~Graph();
    CGistogram* gistogram;

private:
    Ui::Graph *ui;

public slots:
    void recieveData(QByteArray &data);
};

#endif // GRAPH_H
