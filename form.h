#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private:
    Ui::Form *ui;
    QCustomPlot *customPlot;    // Объявляем графическое полотно
    QCPGraph *graphic;          // Объявляем график

private slots:
    void slotRangeChanged (const QCPRange &newRange);
    void recieveData(QByteArray &data);

};

#endif // FORM_H
