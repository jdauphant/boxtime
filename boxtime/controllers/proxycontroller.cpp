#include "proxycontroller.h"

#include <QtCore>
#include <QtNetwork>
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

#ifdef Q_OS_WIN32
const QString ProxyController::DEFAULT_PROXY_PROCESS = QString("");
const QString ProxyController::DEFAULT_PROXY_KILLPROCESS = QString("");
#endif


ProxyController::ProxyController()
{
    proxyProcess = new QProcess(this);

#ifdef Q_OS_UNIX
    QProcess killallProcess;
    killallProcess.start(SettingsController::getValue<QString>("proxy/killprocess", DEFAULT_PROXY_KILLPROCESS));
    killallProcess.waitForFinished();
#endif
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

void ProxyController::startProxy()
{
    setDefaultSystemProxy();
    QString programName = SettingsController::getValue<QString>("proxy/process", DEFAULT_PROXY_PROCESS);

    proxyProcess->setWorkingDirectory(QCoreApplication::applicationDirPath());
    qDebug() << "Tinyproxy Working Directory : " << proxyProcess->workingDirectory();
    proxyProcess->start(programName ,
       QStringList() << "-d" << "-c" << "./tinyproxy_withblocking.conf");
    proxyProcess->waitForBytesWritten();
    QString output(proxyProcess->readAllStandardOutput());
    qDebug() << output;
}

void ProxyController::stopProxy()
{
    restoreDefaultSystemProxy();
    proxyProcess->terminate();
    proxyProcess->waitForFinished();
}

void ProxyController::setGsettingsParams(QString schema, QString key, QString value)
{
    QProcess gsettingsMode;
    gsettingsMode.start("gsettings",
         QStringList() << "set" << schema << key << value);
    gsettingsMode.waitForFinished();
}

void ProxyController::setDefaultSystemProxy()
{
#ifdef Q_OS_LINUX
    setGsettingsParams("org.gnome.system.proxy.http", "host", "'127.0.0.1'");
    setGsettingsParams("org.gnome.system.proxy.http", "port", "8888");

    setGsettingsParams("org.gnome.system.proxy.https", "host", "'127.0.0.1'");
    setGsettingsParams("org.gnome.system.proxy.https", "port", "8888");

    setGsettingsParams("org.gnome.system.proxy.socks", "host", "'127.0.0.1'");
    setGsettingsParams("org.gnome.system.proxy.socks", "port", "8888");

    setGsettingsParams("org.gnome.system.proxy", "mode", "'manual'");
#endif
}

void ProxyController::restoreDefaultSystemProxy()
{
#ifdef Q_OS_LINUX
    setGsettingsParams("org.gnome.system.proxy", "mode", "'none'");
#endif
}


