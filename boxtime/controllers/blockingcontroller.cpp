#include "blockingcontroller.h"
#include "taskcontroller.h"
#include <QApplication>

BlockingController::BlockingController() :
    GenericControllerModule("blocking", DEFAULT_BLOCKING_ENABLE)
{
    proxyController = NULL;
}

BlockingController * BlockingController::getInstance()
{
    static BlockingController * blockingController = 0;
    if(!blockingController)
    {
        blockingController = new BlockingController();
    }
    return blockingController;
}

void BlockingController::load()
{
    if(proxyController==NULL)
    {
        proxyController = new ProxyController();
        proxyController->setBlockingList(SettingsController::getInstance()->getValue<QStringList>("proxy/blocklist", DEFAULT_BLOCKING_LIST));
    }

    TaskController * taskController = TaskController::getInstance();
    connect(taskController,SIGNAL(started(Task *)),this,SLOT(block()));
    connect(taskController,SIGNAL(ended(Task *)),this,SLOT(unblock()));
    connect(QApplication::instance(),SIGNAL(lastWindowClosed()),this,SLOT(unblock()));
    if(false==proxyController->isChangeProxyOk())
    {
        qWarning("Disable blocking");
        SettingsController::getInstance()->setValue("blocking/enable", false);
        return;
    }

    if(taskController->asCurrentTask())
       block();
}

void BlockingController::unload()
{
    TaskController * taskController = TaskController::getInstance();
    disconnect(taskController,SIGNAL(started(Task *)),this,SLOT(block()));
    disconnect(taskController,SIGNAL(ended(Task *)),this,SLOT(unblock()));
    disconnect(QApplication::instance(),SIGNAL(lastWindowClosed()),this,SLOT(unblock()));
    if(isActive())
        unblock();
}

bool BlockingController::isActive()
{
    return proxyController!=NULL && proxyController->isActive();
}

void BlockingController::block()
{
    if(false==proxyController->start())
    {
        qWarning() << "Fail to start proxy, blocking is disable";
    }
}

void BlockingController::unblock()
{
    proxyController->stop();
}



