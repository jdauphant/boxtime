#include <QDateTime>
#include "taskcontroller.h"

TaskController::TaskController()
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerTimeout()));
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

void TaskController::start(QString taskName)
{
    currentTask = new Task(taskName,QDateTime::currentDateTime(), 0);
    timer->start(1000);

    started(currentTask);
}

void TaskController::timerTimeout()
{
    currentTask->timeElapsed++;
    newTime(currentTask->timeElapsed);
}

void TaskController::end()
{
    timer->stop();
    newTime(0);
    ended(currentTask);
}


