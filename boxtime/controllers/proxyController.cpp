#include "proxyController.h"

#include <QDebug>
#include <QCoreApplication>

ProxyController::ProxyController()
{
    proxyProcessus = new QProcess(this);

    QProcess killallProcessus;
    killallProcessus.start("killall tinyproxy");
    killallProcessus.waitForFinished();
}

ProxyController::~ProxyController()
{
    stopProxy();
    delete proxyProcessus;
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
    QString programName = "./tinyproxy";
    QStringList arguments;

    if(blocking)
        arguments << "-d" << "-c" << "./tinyproxy_withblocking.conf";
    else
        arguments << "-d" << "-c" << "./tinyproxy_noblocking.conf";

    //qDebug() << "Tinyproxy Working Directory : " << proc->workingDirectory();
    proxyProcessus->setWorkingDirectory(QCoreApplication::applicationDirPath());
    proxyProcessus->start(programName , arguments);
    proxyProcessus->waitForBytesWritten();
    QString output(proxyProcessus->readAllStandardOutput());
    qDebug() << output;
}

void ProxyController::stopProxy()
{
    proxyProcessus->terminate();
    proxyProcessus->waitForFinished();
}
