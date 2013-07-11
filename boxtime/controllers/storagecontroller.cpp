#include "storagecontroller.h"
#include "taskcontroller.h"

StorageController::StorageController()
{
    TaskController * taskController = TaskController::getInstance();
    connect(taskController,SIGNAL(started(Task *)),this,SLOT(taskStarted(Task*)));
    connect(taskController,SIGNAL(ended(Task *)),this,SLOT(taskEnded(Task*)));

    qDebug() << "Storage File : " << SettingsController::getInstance()->getValue<QString>("storage/file",DEFAULT_STORAGE_FILE);
}

StorageController * StorageController::getInstance()
{
    static StorageController * storageController = 0;
    if(!storageController)
    {
        storageController = new StorageController();
    }
    return storageController;
}

void StorageController::taskStarted(Task* task)
{

}

void StorageController::taskEnded(Task* task)
{
    QFile file(SettingsController::getInstance()->getValue<QString>("storage/file",DEFAULT_STORAGE_FILE));
    if (file.open(QFile::WriteOnly | QIODevice::Append))
    {
      QTextStream stream(&file);
      stream << task->toCSVLine() << endl;
      file.close();
    }
}
