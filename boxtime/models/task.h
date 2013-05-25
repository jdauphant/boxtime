#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QDateTime>

class Task : public QObject
{
    Q_OBJECT
public:
    Task(QString name, QDateTime startDateTime, double timeElapsed);
    void addToLogFile();

    QString name;
    QDateTime startDateTime;
    double timeElapsed;
};

#endif // TASK_H


