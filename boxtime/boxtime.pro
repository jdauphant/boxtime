#-------------------------------------------------
#
# Project created by QtCreator 2013-05-11T16:05:11
#
#-------------------------------------------------

CONFIG(debug, debug|release) {
     DEFAULT_SUBDIR = debug
}
else: {
     DEFAULT_SUBDIR = release
}
DESTDIR = $$PWD/../bin/$$DEFAULT_SUBDIR

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

macx {
    ICON = logo.icns
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
}

## Libs
unix:!macx {
    LIBS += -lX11
}

macx {
    LIBS += -lobjc
}


DEPENDPATH += $$PWD/../lib/$$DEFAULT_SUBDIR

macx {
    QMAKE_POST_LINK += mkdir -p $$DESTDIR/$${TARGET}.app/Contents/Frameworks ;
    QMAKE_POST_LINK += cp $$PWD/../lib/$${DEFAULT_SUBDIR}/*.dylib $$DESTDIR/$${TARGET}.app/Contents/Frameworks/ ;
    QMAKE_POST_LINK += macdeployqt $$DESTDIR/$${TARGET}.app
}


# Lib mixpanel
LIBS += -L$$PWD/../lib/$$DEFAULT_SUBDIR -lmixpanel-qt
INCLUDEPATH += $$PWD/../mixpanel-qt/src $$PWD/../startuplaunch-qt


# Lib startup launch
LIBS += -lstartuplaunch-qt
INCLUDEPATH += $$PWD/../startuplaunch-qt
win32: PRE_TARGETDEPS += $$PWD/../lib/$$DEFAULT_SUBDIR/startuplaunch-qt.lib
else:unix: PRE_TARGETDEPS += $$PWD/../lib/$$DEFAULT_SUBDIR/libstartuplaunch-qt.a


OTHER_FILES +=
