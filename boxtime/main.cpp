#include "taskwidget.h"
#include "taskcontroller.h"
#include <QtCore>
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);
    TaskWidget taskWidget;

    TaskController * taskController = TaskController::getInstance();

    QObject::connect(&taskWidget,SIGNAL(newTask(QString)),taskController,SLOT(startTask(QString)));
    QObject::connect(&taskWidget,SIGNAL(done()),taskController,SLOT(endingTask()));
    QObject::connect(taskController,SIGNAL(newTime(double)),&taskWidget,SLOT(newTime(double)));
    QObject::connect(&qApplication,SIGNAL(aboutToQuit()),ProxyController::getInstance(),SLOT(stopProxy()));

    taskWidget.show();

    return qApplication.exec();
}



