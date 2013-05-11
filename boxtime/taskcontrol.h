#ifndef TASKCONTROL_HPP
#define TASKCONTROL_HPP

#include <QObject>
#include <QTimer>
#include "tinyproxy.h"

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

public slots:
    void startTask(QString taskName);
    void endingTask();

signals:
    void newTime(double time);

private slots:
    void timeoutTimer();
};

#endif // TASKCONTROL_HPP
