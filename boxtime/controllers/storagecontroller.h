#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H

#include <QtCore>

#include "task.h"
#include "settingscontroller.h"

namespace settings
{
    const QString DEFAULT_STORAGE_FILE = SettingsController::getDataPath()+QString("/tasks_history.csv");
    const QString DEFAULT_EXPORT_CSV_FILE = SettingsController::getDocumentsPath()+QString("/boxtime_history_%s.csv");
}

class StorageController : public QObject
{
    Q_OBJECT

public:
    static StorageController * getInstance();
    ~StorageController(){ }
    QString getCSVFile();
    bool historyExists();

private:
    StorageController();
    StorageController(const StorageController & c): QObject(c.parent()) {}
    StorageController & operator=(const StorageController &) {return *this;}


public slots:
    void taskStarted(Task * task);
    void taskEnded(Task * task);
    void exportAndOpenCSV();
};

#endif // STORAGECONTROLLER_H
