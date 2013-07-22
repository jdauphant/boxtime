#-------------------------------------------------
#
# Project created by QtCreator 2013-05-11T16:05:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = boxtime
TEMPLATE = app
INCLUDEPATH = controllers views models utils

SOURCES += main.cpp\
    models/task.cpp \
    controllers/settingscontroller.cpp \
    controllers/proxycontroller.cpp \
    controllers/taskcontroller.cpp \
    views/taskwidget.cpp \
    views/genericwidget.cpp \
    utils/systemproxy.cpp \
    controllers/storagecontroller.cpp \
    utils/startuplaunch.cpp \
    controllers/blockingcontroller.cpp \
    controllers/genericmodulecontroller.cpp

HEADERS  += \
    models/task.h \
    controllers/settingscontroller.h \
    controllers/proxycontroller.h \
    controllers/taskcontroller.h \
    views/taskwidget.h \
    views/genericwidget.h \
    utils/systemproxy.h \
    controllers/storagecontroller.h \
    utils/startuplaunch.h \
    controllers/blockingcontroller.h \
    controllers/genericmodulecontroller.h

FORMS    += \
    views/taskwidget.ui

RESOURCES += \
    ressources.qrc \
    privoxy.qrc

unix:!macx {
    LIBS += -lX11
}

macx {
    ICON = logo.icns
    LIBS += -lobjc
}
