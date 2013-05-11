#include "taskcontrol.h"

TaskControl::TaskControl()
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutTimer()));
}


TaskControl * TaskControl::getInstance()
{
    static TaskControl * taskControl = 0;
    if(!taskControl)
    {
        taskControl = new TaskControl();
    }
    return taskControl;
}

void TaskControl::startTask(QString taskName)
{
    time = 0;
    timer->start(1000);

}
void TaskControl::timeoutTimer()
{
    time++;
    newTime(time);
}

void TaskControl::endingTask()
{
    timer->stop();
    time=0;
    newTime(time);
}


