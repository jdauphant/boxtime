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
    changeProxyProcess.setWorkingDirectory(QCoreApplication::applicationDirPath());
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
    const QString DEFAULT_PROXYCHANGE_EXEC = QString("/usr/sbin/changelocalproxy");
    const QString DEFAULT_PROXYCHANGE_INSTALLER = QString("clproxy_install.sh");
    bool result = QFile().exists(DEFAULT_PROXYCHANGE_EXEC);
    if(false==result)
    {
        QString proxychangeInstallerPath = QCoreApplication::applicationDirPath()+"/"+DEFAULT_PROXYCHANGE_INSTALLER;
        if(false==QFile().exists(proxychangeInstallerPath))
        {
            qWarning() << proxychangeInstallerPath << "missing";
            return false;
        }

        QProcess changeProxyProcess;
        changeProxyProcess.start("osascript", QStringList() << "-e" << "do shell script \""+proxychangeInstallerPath+"\" with administrator privileges");
        changeProxyProcess.waitForFinished();

        result = QFile().exists(DEFAULT_PROXYCHANGE_EXEC);
        if(result)
            qDebug() << "Service " << DEFAULT_PROXYCHANGE_EXEC << " installed.";
        else
            qDebug() << "Script "<< proxychangeInstallerPath << " fail to install " << DEFAULT_PROXYCHANGE_EXEC;
    }
    return result;
#endif
    return false;
}
