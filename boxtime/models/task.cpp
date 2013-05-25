#include "task.h"
#include <QFile>
#include <QTextStream>

Task::Task(QString name, QDateTime startDateTime, double timeElapsed)
    :name(name),startDateTime(startDateTime),timeElapsed(timeElapsed)
{
}


void Task::addToLogFile()
{
    QFile file("./taskLog.csv");
    if (file.open(QFile::WriteOnly | QIODevice::Append))
    {
      QTextStream stream(&file);
      stream << name << ";" << startDateTime.toString(Qt::ISODate) << ";"<< timeElapsed << "\n";
      file.close();
    }
}
