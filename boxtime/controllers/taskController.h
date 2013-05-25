#ifndef TASKCONTROLLER_HPP
#define TASKCONTROLLER_HPP

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include "proxyController.h"
#include "task.h"

class TaskController : public QObject
{
    Q_OBJECT
public:
    static TaskController * getInstance();
    ~TaskController(){ }

private:
    TaskController();
    TaskController(const TaskController & c): QObject(c.parent()) {}
    TaskController & operator=(const TaskController &) {return *this;}

    QTimer * timer;
    double time;
    ProxyController* proxyController;
    QString currentTaskName;
    QDateTime startDateTime;

public slots:
    void startTask(QString taskName);
    void endingTask();

signals:
    void newTime(double time);

private slots:
    void timeoutTimer();
};

#endif // TASKCONTROLLER_HPP
