#-------------------------------------------------
#
# Project created by QtCreator 2014-08-29T01:36:28
#   This project tries to use the example for QAudioInput
#      to read microphone data into an array
#      significant portions of the code are from that example.
#-------------------------------------------------

QT       += core
QT       += multimedia
QT       -= gui

TARGET = simpleAudioInput
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    myaudioinput.cpp

HEADERS += \
    myaudioinput.h
