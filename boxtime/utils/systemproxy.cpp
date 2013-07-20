#include "systemproxy.h"
#include <QtNetwork>

SystemProxy::SystemProxy()
{
}

void SystemProxy::setGsettingsParams(QString schema, QString key, QString value)
{
    QProcess gsettingsProcess;
    gsettingsProcess.start("gsettings",
         QStringList() << "set" << schema << key << value);
    gsettingsProcess.waitForFinished();
}

void SystemProxy::setMacOSXChangeProxy(const QStringList params)
{
    QProcess changeProxyProcess;
    changeProxyProcess.start("/usr/sbin/changelocalproxy", params);
    changeProxyProcess.waitForFinished();
}



void SystemProxy::setAndEnableSystemProxy(const QString address, const unsigned int port)
{
    QString portString = QString::number(port);
#ifdef Q_OS_LINUX
    //Gnome only
    setGsettingsParams("org.gnome.system.proxy.http", "host", "'"+address+"'");
    setGsettingsParams("org.gnome.system.proxy.http", "port", portString);

    setGsettingsParams("org.gnome.system.proxy.https", "host", "'"+address+"'");
    setGsettingsParams("org.gnome.system.proxy.https", "port", portString);

    setGsettingsParams("org.gnome.system.proxy.socks", "host", "'"+address+"'");
    setGsettingsParams("org.gnome.system.proxy.socks", "port", portString);

    setGsettingsParams("org.gnome.system.proxy", "mode", "'manual'");
#endif
#ifdef Q_OS_MAC
    setMacOSXChangeProxy(QStringList() << "on" << portString);
#endif
#ifdef Q_OS_WIN32

#endif
}

void SystemProxy::disableSystemProxy()
{
#ifdef Q_OS_LINUX
    setGsettingsParams("org.gnome.system.proxy", "mode", "'none'");
#endif
#ifdef Q_OS_MAC
    setMacOSXChangeProxy(QStringList() << "off");
#endif
}

bool SystemProxy::isThatPosibleToChangeProxy()
{
#ifdef Q_OS_LINUX
    return QFile().exists("/usr/bin/gsettings");
#endif
#ifdef Q_OS_MAC
    bool result = QFile().exists("/usr/sbin/changelocalproxy");
    if(false==result)
    {
        if(false==QFile().exists("./clproxy_install.sh"))
        {
            qWarning("./clproxy_install.sh missing");
            return false;
        }

        QProcess changeProxyProcess;
        changeProxyProcess.start("osascript", QStringList() << "-e" << "do shell script \"./clproxy_install.sh\" with administrator privileges");
        if(false==changeProxyProcess.waitForFinished())
        {

        }


        result = QFile().exists("/usr/sbin/changelocalproxy");
        if(result)
            qDebug("Service /usr/sbin/changelocalproxy installed.");
    }
    return result;
#endif
    return false;
}
