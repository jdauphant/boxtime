#ifndef TASK_H
#define TASK_H

#include <QtCore>

class Task
{
public:
    Task(QString name, QDateTime startDateTime, long timeElapsed);
    QString toCSVLine(QChar separator = ';');

    QString name;
    QDateTime startDateTime;
    long timeElapsed;
};

#endif // TASK_H


