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

void SystemProxy::setMacOSXNetworksetupParams(const QStringList params)
{
    QProcess networksetupProcess;
    networksetupProcess.start("networksetup", params);
    networksetupProcess.waitForFinished();
}

void SystemProxy::setDefaultSystemProxy(const QString address, const unsigned int port)
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
    setMacOSXNetworksetupParams(QStringList() << "-setwebproxy" << "Ethernet" << address << portString << "off"
                                << "-setsecurewebproxy" << "Ethernet" << address << portString << "off"
                                << "-setwebproxy" << "Wi-Fi" << address << portString << "off"
                                << "-setsecurewebproxy" << "Wi-Fi" << address << portString << "off"
                                << "-setwebproxystate" << "Ethernet" << "on"
                                << "-setsecurewebproxystate" << "Ethernet"  << "on"
                                << "-setwebproxystate" << "Wi-Fi" << "on"
                                << "-setsecurewebproxystate" << "Wi-Fi" << "on");
#endif
#ifdef Q_OS_WIN32

#endif
}

void SystemProxy::enableSystemProxy()
{
#ifdef Q_OS_LINUX
    setGsettingsParams("org.gnome.system.proxy", "mode", "'manual'");
#endif
#ifdef Q_OS_MAC
    // we have to iterate network interface normally
    setMacOSXNetworksetupParams(QStringList() << "-setwebproxystate" << "Ethernet" << "on"
                                << "-setsecurewebproxystate" << "Ethernet"  << "on"
                                << "-setwebproxystate" << "Wi-Fi" << "on"
                                << "-setsecurewebproxystate" << "Wi-Fi" << "on");
#endif
}

void SystemProxy::disableSystemProxy()
{
#ifdef Q_OS_LINUX
    setGsettingsParams("org.gnome.system.proxy", "mode", "'none'");
#endif
#ifdef Q_OS_MAC
    // we have to iterate network interface normally
    setMacOSXNetworksetupParams(QStringList() << "-setwebproxystate" << "Ethernet" << "off"
                                << "-setsecurewebproxystate" << "Ethernet"  << "off"
                                << "-setwebproxystate" << "Wi-Fi" << "off"
                                << "-setsecurewebproxystate" << "Wi-Fi" << "off");
#endif
}
