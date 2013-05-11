#-------------------------------------------------
#
# Project created by QtCreator 2013-05-11T16:05:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = boxtime
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    taskcontrol.cpp \
    tinyproxy.cpp

HEADERS  += mainwidget.h \
    taskcontrol.h \
    tinyproxy.h

FORMS    += mainwidget.ui

RESOURCES += \
    tinyproxy.qrc

