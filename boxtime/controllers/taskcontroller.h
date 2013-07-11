#ifndef TASKCONTROLLER_HPP
#define TASKCONTROLLER_HPP

#include <QtCore>
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
    Task * currentTask;

public slots:
    void start(QString taskName);
    void end();

signals:
    void newTime(double time);
    void started(Task * task);
    void ended(Task * task);


private slots:
    void timerTimeout();
};

#endif // TASKCONTROLLER_HPP
