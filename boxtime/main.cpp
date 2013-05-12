#include "mainwidget.h"
#include "taskcontrol.h"
#include <QApplication>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;

    TaskControl * taskControl = TaskControl::getInstance();
    w.show();

    QObject::connect(&w,SIGNAL(newTask(QString)),taskControl,SLOT(startTask(QString)));
    QObject::connect(&w,SIGNAL(done()),taskControl,SLOT(endingTask()));
    QObject::connect(taskControl,SIGNAL(newTime(double)),&w,SLOT(newTime(double)));
    QObject::connect(&a,SIGNAL(aboutToQuit()),TinyProxy::getInstance(),SLOT(stopProxy()));
    return a.exec();
}

