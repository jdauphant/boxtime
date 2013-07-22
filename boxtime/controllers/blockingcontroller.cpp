#include "blockingcontroller.h"
#include "taskcontroller.h"

BlockingController::BlockingController() :
    GenericModuleController("blocking", DEFAULT_BLOCKING_ENABLE)
{
    checkEnable();
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
    proxyController = new ProxyController();
    proxyController->setBlockingList(SettingsController::getInstance()->getValue<QStringList>("proxy/blocklist", DEFAULT_BLOCKING_LIST));

    TaskController * taskController = TaskController::getInstance();
    connect(taskController,SIGNAL(started(Task *)),this,SLOT(block()));
    connect(taskController,SIGNAL(ended(Task *)),this,SLOT(unblock()));
    if(taskController->asCurrentTask())
       block();
}

void BlockingController::unload()
{
    TaskController * taskController = TaskController::getInstance();
    disconnect(taskController,SIGNAL(started(Task *)),this,SLOT(block()));
    disconnect(taskController,SIGNAL(ended(Task *)),this,SLOT(unblock()));
    if(isActive())
        unblock();

    delete proxyController;
}

bool BlockingController::isActive()
{
    return true;
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



