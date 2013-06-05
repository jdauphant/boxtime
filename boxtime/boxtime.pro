#-------------------------------------------------
#
# Project created by QtCreator 2013-05-11T16:05:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = boxtime
TEMPLATE = app
INCLUDEPATH = controllers views models

SOURCES += main.cpp\
    models/task.cpp \
    controllers/settingscontroller.cpp \
    controllers/proxycontroller.cpp \
    controllers/taskcontroller.cpp \
    views/taskwidget.cpp \
    views/genericwidget.cpp

HEADERS  += \
    models/task.h \
    controllers/settingscontroller.h \
    controllers/proxycontroller.h \
    controllers/taskcontroller.h \
    views/taskwidget.h \
    views/genericwidget.h

FORMS    += \
    views/taskwidget.ui

RESOURCES += \
    ressources.qrc

unix:!macx {
    LIBS += -lX11
}
