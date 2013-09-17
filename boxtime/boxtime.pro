#-------------------------------------------------
#
# Project created by QtCreator 2013-05-11T16:05:11
#
#-------------------------------------------------

DEFAULT_SUBDIR = release
debug: {
     DEFAULT_SUBDIR = debug
}

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = boxtime
TEMPLATE = app
INCLUDEPATH = controllers views models utils

DESTDIR = $$PWD/../bin/$$DEFAULT_SUBDIR

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
    views/protipswidget.cpp \
    views/genericviewmodule.cpp \
    controllers/genericcontrollermodule.cpp \
    bmodule.cpp \
    controllers/analyticscontroller.cpp

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
    views/protipswidget.h \
    views/genericviewmodule.h \
    controllers/genericcontrollermodule.h \
    bmodule.h \
    controllers/analyticscontroller.h

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

LIBS += -L$$PWD/../lib/$$DEFAULT_SUBDIR -lmixpanel-qt

INCLUDEPATH += $$PWD/../mixpanel-qt/src
DEPENDPATH += $$PWD/../lib/$$DEFAULT_SUBDIR

OTHER_FILES +=
