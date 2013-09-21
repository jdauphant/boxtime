#include "task.h"
#include <QFile>
#include <QTextStream>

using namespace task;

Task::Task(QString name, QDateTime startDateTime, long timeElapsed, Type type, State state)
    :name(name),startDateTime(startDateTime),timeElapsed(timeElapsed),_type(type), _state(state), restored(false)
{
}

QString Task::toCSVLine(QChar separator)
{
    return name+separator+startDateTime.toString(Qt::ISODate)+separator+QString::number(timeElapsed);
}

void Task::setRestored()
{
    restored = true;
}
