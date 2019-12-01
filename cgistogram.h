#ifndef CGISTOGRAM_H
#define CGISTOGRAM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "qcustomplot.h"

class CGistogram : public QWidget
{
    Q_OBJECT
public:
    explicit CGistogram(QWidget *parent = nullptr);

private:
    QCustomPlot *customPlot;
    QVBoxLayout *vbox;
    QPushButton *res;
    QCPBars *fossil;

private slots:
    void rndres();
    void setData();
};

#endif // CGISTOGRAM_H
