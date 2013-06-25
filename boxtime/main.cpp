#include "taskwidget.h"
#include "taskcontroller.h"
#include <QtCore>
#include <QApplication>
#include <QDesktopWidget>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);
    if(-1 == QFontDatabase::addApplicationFont("://ressources/Nexa Light.otf"))
    {
        qDebug() << "Impossible to install font Nexa Light.otf";
    }
    if(-1 == QFontDatabase::addApplicationFont("://ressources/Nexa Bold.otf"))
    {
        qDebug() << "Impossible to install font Nexa Bold.otf";
    }

    TaskWidget taskWidget;

    TaskController * taskController = TaskController::getInstance();

    QObject::connect(&taskWidget,SIGNAL(newTask(QString)),taskController,SLOT(start(QString)));
    QObject::connect(&taskWidget,SIGNAL(done()),taskController,SLOT(ending()));
    QObject::connect(taskController,SIGNAL(newTime(double)),&taskWidget,SLOT(newTime(double)));
    QObject::connect(&qApplication,SIGNAL(aboutToQuit()),ProxyController::getInstance(),SLOT(stop()));
    QObject::connect(&taskWidget,SIGNAL(proxySettingChange(bool)),ProxyController::getInstance(),SLOT(enable(bool)));

    taskWidget.show();

    return qApplication.exec();
}



