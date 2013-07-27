#include <QDateTime>
#include "taskcontroller.h"
#include "settingscontroller.h"

TaskController::TaskController(): timer(new QTimer(this)), currentTask(NULL)
{
    connect(timer,SIGNAL(timeout()),this,SLOT(timerTimeout()));
}

/* You need to check after all modules load (and all connect ok) */
void TaskController::checkRecovery()
{
    QString recoveryTaskName = SettingsController::getInstance()->getValue<QString>("current_task/name", QString(""));
    if(recoveryTaskName != QString(""))
    {
        QDateTime recoveryTaskStartDateTime = QDateTime::fromTime_t(SettingsController::getInstance()->getValue<int>("current_task/start_time", QDateTime::currentDateTime().toTime_t()));
        int calculateTime = QDateTime::currentDateTime().toTime_t()-recoveryTaskStartDateTime.toTime_t();
        currentTask = new Task(recoveryTaskName,recoveryTaskStartDateTime, calculateTime);
        timer->start(1000);
        qDebug() << "Task" << currentTask->name << "restore at" << currentTask->timeElapsed << "secondes";
        started(currentTask);
        newTime(currentTask->timeElapsed);
    }
    else
    {
        currentTask = new Task(QString("Unknown task"),QDateTime::currentDateTime(),0,Task::UNKNOWN);
        timer->start(1000);
    }
}

bool TaskController::asCurrentTask()
{
    return currentTask != NULL && currentTask->type==Task::NORMAL;
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
    SettingsController::getInstance()->setValue("current_task/name", currentTask->name);
    SettingsController::getInstance()->setValue("current_task/start_time", currentTask->startDateTime.toTime_t());

    timer->start(1000);
    qDebug() << "Task" << taskName << "started.";
    started(currentTask);    
    newTime(currentTask->timeElapsed);
}

void TaskController::timerTimeout()
{
    int calculateTime = QDateTime::currentDateTime().toTime_t()-currentTask->startDateTime.toTime_t();
    if(calculateTime >= currentTask->timeElapsed)
        currentTask->timeElapsed=calculateTime;
    else
        currentTask->timeElapsed++;
    newTime(currentTask->timeElapsed);
}

void TaskController::end()
{
    timer->stop();
    newTime(0);
    SettingsController::getInstance()->removeValue("current_task/name");
    SettingsController::getInstance()->removeValue("current_task/start_time");    
    qDebug() << "Task" << currentTask->name << "ended after" << currentTask->timeElapsed << "secondes";
    ended(currentTask);
    delete currentTask;
    currentTask = new Task(QString("Unknown task"),QDateTime::currentDateTime(),0,Task::UNKNOWN);
    timer->start(1000);
}


