#include "tinyproxy.h"

#include <QDebug>
#include <QDir>

TinyProxy::TinyProxy()
{
    proc = new QProcess(this);
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
    QString programName = "/opt/local/sbin/tinyproxy"; //macos
    QStringList arguments;

    if(blocking)
        arguments << "-d" << "-c" << "/opt/local/tinyproxy/tinyproxy_withblocking.conf";
    else
        arguments << "-d" << "-c" << "/opt/local/tinyproxy/tinyproxy_noblocking.conf";

    //qDebug() << "Tinyproxy Working Directory : " << proc->workingDirectory();
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
