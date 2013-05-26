#include "taskwidget.h"
#include "taskcontroller.h"
#include <QApplication>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);
    TaskWidget taskWidget;

    TaskController * taskController = TaskController::getInstance();
    taskWidget.show();

    QObject::connect(&taskWidget,SIGNAL(newTask(QString)),taskController,SLOT(startTask(QString)));
    QObject::connect(&taskWidget,SIGNAL(done()),taskController,SLOT(endingTask()));
    QObject::connect(taskController,SIGNAL(newTime(double)),&taskWidget,SLOT(newTime(double)));
    QObject::connect(&qApplication,SIGNAL(aboutToQuit()),ProxyController::getInstance(),SLOT(stopProxy()));
    return qApplication.exec();
}

