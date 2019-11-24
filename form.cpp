#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    qDebug() << "Create Form";
    customPlot = new QCustomPlot(); // Инициализируем графическое полотно
       ui->gridLayout->addWidget(customPlot,0,0,1,1);  // Устанавливаем customPlot в окно проложения

       customPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
       customPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
       customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
       customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
       customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);   // Подпись координат по Оси X в качестве Даты и Времени
       customPlot->xAxis->setDateTimeFormat("hh:mm");  // Устанавливаем формат даты и времени

       // Настраиваем шрифт по осям координат
       customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
       customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

       // Автоматическое масштабирование тиков по Оси X
       customPlot->xAxis->setAutoTickStep(true);

       /* Делаем видимыми оси X и Y по верхней и правой границам графика,
        * но отключаем на них тики и подписи координат
        * */
       customPlot->xAxis2->setVisible(true);
       customPlot->yAxis2->setVisible(true);
       customPlot->xAxis2->setTicks(false);
       customPlot->yAxis2->setTicks(false);
       customPlot->xAxis2->setTickLabels(false);
       customPlot->yAxis2->setTickLabels(false);

       customPlot->yAxis->setTickLabelColor(QColor(Qt::red)); // Красный цвет подписей тиков по Оси Y
       customPlot->legend->setVisible(true);   //Включаем Легенду графика
       // Устанавливаем Легенду в левый верхний угол графика
       customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

       // Инициализируем график и привязываем его к Осям
       graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
       customPlot->addPlottable(graphic);  // Устанавливаем график на полотно
       graphic->setName("Значение");       // Устанавливаем
       graphic->setPen(QPen(QColor(Qt::red))); // Устанавливаем цвет графика
       graphic->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
       graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков

       /* Подключаем сигнал от Оси X об изменении видимого диапазона координат
        * к СЛОТу для переустановки формата времени оси.
        * */
       connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
               this, SLOT(slotRangeChanged(QCPRange)));

       // Будем строить график с сегодняшнего дни и текущей секунды в будущее
       double now = QDateTime::currentDateTime().toTime_t();
       // Объявляем вектора времени и доходов
       QVector <double> time(400), income(400);

       srand(15); // Инициализируем генератор псевдослучайных чисел

       // Заполняем график значениями
       for (int i=1; i<400; ++i)
         {
           time[i-1] = now + 3600*i;
           income[i-1] = qFabs(income[i-1]) + (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
         }

       graphic->setData(time, income); // Устанавливаем данные
       customPlot->rescaleAxes();      // Масштабируем график по данным
       customPlot->replot();           // Отрисовываем график
}

Form::~Form()
{
    delete ui;
}

void Form::slotRangeChanged(const QCPRange &newRange)
{
    /* Если область видимости графика меньше одного дня,
     * то отображаем часы и минуты по Оси X,
     * в противном случае отображаем дату "День Месяц Год"
     * */
    qDebug() << "slotRangeChanged";
    customPlot->xAxis->setDateTimeFormat((newRange.size() <= 86400)? "hh:mm" : "dd MMM yy");
}

void Form::recieveData(QByteArray &data)
{
     qDebug() << "recieveData";

}


