#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T22:59:57
#
#-------------------------------------------------

QMAKE_EXTRA_TARGETS += before_build makefilehook

makefilehook.target = $(MAKEFILE)
makefilehook.depends = .beforebuild

PRE_TARGETDEPS += .beforebuild

before_build.target = .beforebuild
before_build.depends = FORCE
before_build.commands = chcp 1251

QT       += core gui
QT      += serialport

# QMAKE_LFLAGS += -static-libgcc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


TARGET = my_shag
TEMPLATE = app


SOURCES += main.cpp\
    controller.cpp \
        mainwindow.cpp \
    port.cpp \
    qcustomplot.cpp \
    form.cpp \
    shproto.cpp

HEADERS  += mainwindow.h \
    controller.h \
    port.h \
    form.h \
    qcustomplot.h \
    shproto.h

FORMS    += mainwindow.ui \
    form.ui
