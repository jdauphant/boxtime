#include "proxycontroller.h"

#include <QtCore>
#include <QtNetwork>
#include <QApplication>
#include <settingscontroller.h>
#include "systemproxy.h"
#include "taskcontroller.h"

ProxyController::ProxyController()
{
    proxyProcess = new QProcess(this);

    TaskController * taskController = TaskController::getInstance();
    connect(taskController,SIGNAL(started(Task *)),this,SLOT(start()));
    connect(taskController,SIGNAL(ended(Task *)),this,SLOT(stop()));
    connect(SettingsController::getInstance(),SIGNAL(valueChanged(QString,QVariant)),this,SLOT(configValueChanged(QString,QVariant)));
}

ProxyController::~ProxyController()
{
    stop();
    delete proxyProcess;
}

ProxyController * ProxyController::getInstance()
{
    static ProxyController * proxyController = 0;
    if(!proxyController)
    {
        proxyController = new ProxyController();
    }
    return proxyController;
}

void ProxyController::start()
{
    if(false==SettingsController::getInstance()->getValue<bool>("proxy/enable", DEFAULT_PROXY_ENABLE))
    {
        qDebug("Proxy is disable");
        return;
    }

    QString programName = SettingsController::getInstance()->getValue<QString>("proxy/process", DEFAULT_PROXY_PROCESS);

    proxyProcess->setWorkingDirectory(QCoreApplication::applicationDirPath());
    qDebug() << "Tinyproxy Working Directory : " << proxyProcess->workingDirectory();
    proxyProcess->start(programName ,
       QStringList() << "-d" << "-c" << "./tinyproxy_withblocking.conf");
    proxyProcess->waitForBytesWritten();
    QString output(proxyProcess->readAllStandardOutput());
    qDebug() << output;

    if(QProcess::Running == proxyProcess->state())
    {
        setDefaultSystemProxy();
    }
    else
    {
        qWarning("Fail to start tinyproxy, fonctionality disable");
        SettingsController::getInstance()->setValue("proxy/enable", false);
    }
}

void ProxyController::stop()
{
    if(QProcess::Running == proxyProcess->state())
    {
        restoreDefaultSystemProxy();
        proxyProcess->terminate();
        proxyProcess->waitForFinished();
    }
}

void ProxyController::setDefaultSystemProxy()
{
    SystemProxy::setDefaultSystemProxy("127.0.0.1",8888);
}

void ProxyController::restoreDefaultSystemProxy()
{
    SystemProxy::disableSystemProxy();
}

void ProxyController::configValueChanged(const QString &key, const QVariant &newValue)
{
    if("proxy/enable"==key)
    {
        if(false==newValue)
        {
            stop();
        }
    }
}
