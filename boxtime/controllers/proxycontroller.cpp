#include "proxycontroller.h"

#include <QtCore>
#include <QtNetwork>
#include <settingscontroller.h>
#include "systemproxy.h"

using namespace settings;

ProxyController::ProxyController()
{
    proxyProcess = new QProcess(this);
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
        qDebug() << "Proxy is disable";
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
        qDebug() << "Fail to start proxy, we disable the fonctionality";
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

void ProxyController::enable(bool enable)
{
    if(false==enable)
    {
        stop();
        SettingsController::getInstance()->setValue("proxy/enable", false);
    }
    else
    {
        SettingsController::getInstance()->setValue("proxy/enable", true);
    }
}
