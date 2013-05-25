#-------------------------------------------------
#
# Project created by QtCreator 2013-05-11T16:05:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = boxtime
TEMPLATE = app
INCLUDEPATH = controllers views models

SOURCES += main.cpp\
    models/taskstorage.cpp \
    controllers/proxyController.cpp \
    controllers/taskController.cpp \
    views/taskWidget.cpp

HEADERS  += \
    models/taskstorage.h \
    controllers/proxyController.h \
    controllers/taskController.h \
    views/taskWidget.h

FORMS    += \
    views/taskWidget.ui

RESOURCES += \
    images.qrc
