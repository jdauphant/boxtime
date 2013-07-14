#include <QtCore>
#include <QDesktopWidget>
#include <QFontDatabase>
#include <QApplication>

#include "taskwidget.h"
#include "taskcontroller.h"
#include "proxycontroller.h"

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);
    qDebug("Install fonts");
    if(-1 == QFontDatabase::addApplicationFont("://ressources/Nexa Light.otf"))
    {
        qDebug() << "Impossible to install font Nexa Light.otf";
    }
    if(-1 == QFontDatabase::addApplicationFont("://ressources/Nexa Bold.otf"))
    {
        qDebug() << "Impossible to install font Nexa Bold.otf";
    }

    qDebug("Load GUI");
    TaskWidget taskWidget;
    taskWidget.show();

    qDebug("Load modules");
    QObject::connect(&qApplication,SIGNAL(lastWindowClosed()),ProxyController::getInstance(),SLOT(stop()));
    TaskController::getInstance()->checkRecovery();
    return qApplication.exec();
}



