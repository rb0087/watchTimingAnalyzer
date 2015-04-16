#-------------------------------------------------
#
# Project created by QtCreator 2014-02-24T16:38:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = plottingTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../qcustomplot/qcustomplot.cpp

HEADERS  += mainwindow.h \
    ../qcustomplot/qcustomplot.h

FORMS    += mainwindow.ui
