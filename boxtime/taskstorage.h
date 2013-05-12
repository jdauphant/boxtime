#ifndef TASKSTORAGE_H
#define TASKSTORAGE_H

#include <QObject>
#include <QDateTime>

class TaskStorage : public QObject
{
    Q_OBJECT
public:
    static TaskStorage * getInstance();
    ~TaskStorage(){};

private:
    TaskStorage();
    TaskStorage(const TaskStorage & c): QObject(c.parent()) {}
    TaskStorage & operator=(const TaskStorage &) {return *this;}

public slots:
    void addLog(QString log, QDateTime startDateTime, double timeElapsed);
};

#endif // TASKSTORAGE_H


