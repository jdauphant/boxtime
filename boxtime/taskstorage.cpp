#include "taskstorage.h"
#include <QFile>
#include <QTextStream>

TaskStorage::TaskStorage()
{
}

TaskStorage * TaskStorage::getInstance()
{
    static TaskStorage * taskStorage = 0;
    if(!taskStorage)
    {
        taskStorage = new TaskStorage();
    }
    return taskStorage;
}

void TaskStorage::addLog(QString log, QDateTime startDateTime, double timeElapsed)
{
    QFile file("./taskLog.csv");
    if (file.open(QFile::WriteOnly | QIODevice::Append))
    {
      QTextStream stream(&file);
      stream << log << ";" << startDateTime.toString(Qt::ISODate) << ";"<< timeElapsed << "\n";
      file.close();
    }
}
