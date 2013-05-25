#include "proxyController.h"

#include <QDebug>
#include <QCoreApplication>

ProxyController::ProxyController()
{
    proc = new QProcess(this);

    QProcess p;
    p.start("killall tinyproxy");
    p.waitForFinished();
}

ProxyController::~ProxyController()
{
    stopProxy();
    delete proc;
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
    proc->setWorkingDirectory(QCoreApplication::applicationDirPath());
    proc->start(programName , arguments);
    proc->waitForBytesWritten();
    QString output(proc->readAllStandardOutput());
    qDebug() << output;
}

void ProxyController::stopProxy()
{
    proc->terminate();
    proc->waitForFinished();
}
