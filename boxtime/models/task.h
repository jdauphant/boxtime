#ifndef TASK_H
#define TASK_H

#include <QtCore>

class Task
{
public:
    Task(QString name, QDateTime startDateTime, double timeElapsed);
    QString toCSVLine(QChar separator = ';');

    QString name;
    QDateTime startDateTime;
    double timeElapsed;
};

#endif // TASK_H


