#include "tinyproxy.h"

TinyProxy::TinyProxy()
{
    proc = new QProcess();
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
    QString program = "tinyproxy";
    QStringList arguments;

    if(blocking)
        arguments << " -d " << " -c " << " tinyproxy/conf/tinyproxy_withblocking.conf";
    else
        arguments << " -c " << " tinyproxy/conf/tinyproxy_noblocking.conf";
        //sh.start("sh", QStringList() << "-c" << "ifconfig | grep inet");

    proc->start(program , arguments);
}

void TinyProxy::stopProxy()
{
    proc->kill();
}
