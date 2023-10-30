#-------------------------------------------------
#
# Project created by QtCreator 2023-10-24T16:11:45
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 QMQTT_NO_SSL

TARGET = mqtt_client
TEMPLATE = app

include($$PWD/qmqtt-master/qmqtt.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    mqttclient/mqttclient.cpp

HEADERS  += mainwindow.h \
    mqttclient/mqttclient.h

FORMS    += mainwindow.ui
