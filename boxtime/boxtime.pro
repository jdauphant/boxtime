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
    views/movablecharm.cpp \
    utils/systemproxy.cpp \
    controllers/storagecontroller.cpp \
    utils/startuplaunch.cpp \
    controllers/blockingcontroller.cpp \
    controllers/genericmodule.cpp \
    views/protipswidget.cpp

HEADERS  += \
    models/task.h \
    controllers/settingscontroller.h \
    controllers/proxycontroller.h \
    controllers/taskcontroller.h \
    views/taskwidget.h \
    views/genericwidget.h \
    views/movablecharm.h \
    utils/systemproxy.h \
    controllers/storagecontroller.h \
    utils/startuplaunch.h \
    controllers/blockingcontroller.h \
    controllers/genericmodule.h \
    views/protipswidget.h

FORMS    += \
    views/taskwidget.ui \
    views/protipswidget.ui

RESOURCES += \
    ressources.qrc \
    privoxy.qrc

unix:!macx {
    LIBS += -lX11
}

macx {
    ICON = logo.icns
    LIBS += -lobjc
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
}

OTHER_FILES +=
