#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H

#include <QtCore>
#include "task.h"

class StorageController : public QObject
{
    Q_OBJECT

public:
    static StorageController * getInstance();
    ~StorageController(){ }

private:
    StorageController();
    StorageController(const StorageController & c): QObject(c.parent()) {}
    StorageController & operator=(const StorageController &) {return *this;}


public slots:
    void taskStarted(Task * task);
    void taskEnded(Task * task);
};

#endif // STORAGECONTROLLER_H
