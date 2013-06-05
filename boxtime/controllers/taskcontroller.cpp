#include <QDateTime>
#include "taskcontroller.h"
#include "proxycontroller.h"

TaskController::TaskController()
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutTimer()));

    proxyController = ProxyController::getInstance();
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

    proxyController->startProxy();

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
    proxyController->stopProxy();
    Task(currentTaskName,startDateTime, timeElapsed).addToLogFile();
}


