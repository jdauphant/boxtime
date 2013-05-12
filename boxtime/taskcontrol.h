#ifndef TASKCONTROL_HPP
#define TASKCONTROL_HPP

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include "tinyproxy.h"
#include "taskstorage.h"

class TaskControl : public QObject
{
    Q_OBJECT
public:
    static TaskControl * getInstance();
    ~TaskControl(){ }

private:
    TaskControl();
    TaskControl(const TaskControl & c): QObject(c.parent()) {}
    TaskControl & operator=(const TaskControl &) {return *this;}

    QTimer * timer;
    double time;
    TinyProxy* tproxy;
    TaskStorage* taskStorage;
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

#endif // TASKCONTROL_HPP
