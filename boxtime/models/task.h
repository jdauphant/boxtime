#ifndef TASK_H
#define TASK_H

#include <QtCore>

class Task
{
public:
    typedef enum {
        NORMAL,
        BREATHER,
        UNKNOWN
    } TYPE;

    Task(QString name, QDateTime startDateTime, long timeElapsed, TYPE type = NORMAL);
    QString toCSVLine(QChar separator = ';');

    QString name;
    QDateTime startDateTime;
    long timeElapsed;
    TYPE type;
};

#endif // TASK_H


