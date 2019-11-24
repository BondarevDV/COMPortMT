#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T22:59:57
#
#-------------------------------------------------

QT       += core gui
QT      += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


TARGET = my_shag
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    port.cpp \
    qcustomplot.cpp \
    form.cpp

HEADERS  += mainwindow.h \
    port.h \
    form.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    form.ui
