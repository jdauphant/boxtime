#include "task.h"
#include <QFile>
#include <QTextStream>

Task::Task(QString name, QDateTime startDateTime, long timeElapsed, TYPE type)
    :name(name),startDateTime(startDateTime),timeElapsed(timeElapsed),type(type)
{
}

QString Task::toCSVLine(QChar separator)
{
    return name+separator+startDateTime.toString(Qt::ISODate)+separator+QString::number(timeElapsed);
}
