#include "proxycontroller.h"

#include <QDebug>
#include <QCoreApplication>
#include <settingscontroller.h>
//#include <HTTP.h>

#ifdef Q_OS_MAC
const QString ProxyController::DEFAULT_PROXY_PROCESS = QString("./tinyproxy");
const QString ProxyController::DEFAULT_PROXY_KILLPROCESS = QString("killall tinyproxy");
#endif

#ifdef Q_OS_LINUX
const QString ProxyController::DEFAULT_PROXY_PROCESS = QString("tinyproxy");
const QString ProxyController::DEFAULT_PROXY_KILLPROCESS = QString("pkill tinyproxy");
#endif

/*
#ifdef Q_OS_WIN32
const QString ProxyController::DEFAULT_PROXY_PROCESS = QString("");
const QString ProxyController::DEFAULT_PROXY_KILLPROCESS = QString("");
#endif
*/

ProxyController::ProxyController()
{
    proxyProcess = new QProcess(this);

    QProcess killallProcess;
    killallProcess.start(SettingsController::getValue<QString>("proxy/killprocess",DEFAULT_PROXY_KILLPROCESS));
    killallProcess.waitForFinished();
}

ProxyController::~ProxyController()
{
    stopProxy();
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

void ProxyController::startProxy(bool blocking)
{
    /*HTTPAPI * api = new HTTPAPI();
    api->InitHTTPProxy("127.0.0.1","8080");*/

    QString programName = SettingsController::getValue<QString>("proxy/process",DEFAULT_PROXY_PROCESS);
    QStringList arguments;

    if(blocking)
        arguments << "-d" << "-c" << "./tinyproxy_withblocking.conf";
    else
        arguments << "-d" << "-c" << "./tinyproxy_noblocking.conf";

    //qDebug() << "Tinyproxy Working Directory : " << proc->workingDirectory();
    proxyProcess->setWorkingDirectory(QCoreApplication::applicationDirPath());
    proxyProcess->start(programName , arguments);
    proxyProcess->waitForBytesWritten();
    QString output(proxyProcess->readAllStandardOutput());
    qDebug() << output;
}

void ProxyController::stopProxy()
{
    proxyProcess->terminate();
    proxyProcess->waitForFinished();
}
