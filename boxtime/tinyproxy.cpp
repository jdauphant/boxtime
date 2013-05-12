#include "tinyproxy.h"

#include <QDebug>
#include <QCoreApplication>

TinyProxy::TinyProxy()
{
    proc = new QProcess(this);

    QProcess p;
    p.start("killall tinyproxy");
    p.waitForFinished();
}

TinyProxy::~TinyProxy()
{
    stopProxy();
    delete proc;
}

TinyProxy * TinyProxy::getInstance()
{
    static TinyProxy * tinyProxy = 0;
    if(!tinyProxy)
    {
        tinyProxy = new TinyProxy();
    }
    return tinyProxy;
}

void TinyProxy::startProxy(bool blocking)
{
    QString programName = "./tinyproxy"; //macos
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

void TinyProxy::stopProxy()
{
    proc->terminate();
    proc->waitForFinished();
}
