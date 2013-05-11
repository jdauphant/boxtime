#include "taskcontrol.h"
#include "tinyproxy.h"

TaskControl::TaskControl()
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutTimer()));

    tproxy = TinyProxy::getInstance();
    tproxy->startProxy(TinyProxy::WITHOUT_BLOCKING);
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

    tproxy->stopProxy();
    tproxy->startProxy(TinyProxy::WITH_BLOCKING);

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
    tproxy->stopProxy();
    tproxy->startProxy(TinyProxy::WITHOUT_BLOCKING);
}


