#include <QDateTime>
#include "taskController.h"
#include "proxyController.h"

TaskController::TaskController()
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutTimer()));

    tproxy = ProxyController::getInstance();
    tproxy->startProxy(ProxyController::WITHOUT_BLOCKING);

    taskStorage = TaskStorage::getInstance();
}


TaskController * TaskController::getInstance()
{
    static TaskController * taskControl = 0;
    if(!taskControl)
    {
        taskControl = new TaskController();
    }
    return taskControl;
}

void TaskController::startTask(QString taskName)
{
    time = 0;
    timer->start(1000);

    tproxy->stopProxy();
    tproxy->startProxy(ProxyController::WITH_BLOCKING);

    startDateTime = QDateTime::currentDateTime();
    currentTaskName = taskName;
}
void TaskController::timeoutTimer()
{
    time++;
    newTime(time);
}

void TaskController::endingTask()
{
    timer->stop();
    double timeElapsed = time;
    time=0;
    newTime(time);
    tproxy->stopProxy();
    tproxy->startProxy(ProxyController::WITHOUT_BLOCKING);
    taskStorage->addLog(currentTaskName, startDateTime, timeElapsed);
}


