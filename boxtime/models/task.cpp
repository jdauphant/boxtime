#include "task.h"
#include <QFile>
#include <QTextStream>

Task::Task(QString name, QDateTime startDateTime, double timeElapsed)
    :name(name),startDateTime(startDateTime),timeElapsed(timeElapsed)
{
}

QString Task::toCSVLine(QChar separator)
{
    return name+separator+startDateTime.toString(Qt::ISODate)+separator+QString::number(timeElapsed);
}
