#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H

#include <QtCore>

#include "task.h"
#include "settingscontroller.h"

namespace settings
{
    const QString DEFAULT_EXPORT_CSV_FILE = SettingsController::getDocumentsPath()+QString("/boxtime_history_%s.csv");
    const char DEFAULT_EXPORT_CSV_SEPARATOR = ',';
}

class StorageController : public QObject
{
    Q_OBJECT

public:
    static StorageController * getInstance();
    ~StorageController(){ }
    QString getCSVFile();
    bool historyExists();
    const QString DEFAULT_STORAGE_FILE;

private:
    StorageController();
    StorageController(const StorageController & c): QObject(c.parent()) {}
    StorageController & operator=(const StorageController &) {return *this;}


public slots:
    void taskEnded(Task * task);
    void exportAndOpenCSV();
};

#endif // STORAGECONTROLLER_H
