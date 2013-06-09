#include "proxycontroller.h"

#include <QtCore>
#include <QtNetwork>
#include <settingscontroller.h>
#include "systemproxy.h"

const QStringList ProxyController::DEFAULT_BLOCK_LIST = QStringList() << "facebook.com" << "9gag.com" << "4chan.org"
          << "twitter.com" << "pinterest.com" << "tweetdeck.com"
          << "*.facebook.com" << "*.9gag.com" << "*.4chan.org" << "*.twitter.com" << "*.pinterest.com" << "*.tweetdeck.com"
          << "collegehumor.com" << "*.collegehumor.com" << "reddit.com" << "*.reddit.com"
          << "plus.google.com" << "mail.google.com" << "news.google.com"
          << "outlook.com" << "*.outlook.com" << "hotmail.com" << "www.hotmail.com" << "mail.live.com"
          << "hotmail.fr" << "www.hotmail.fr" << "mail.yahoo.com" << "*.mail.yahoo.com";

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

void ProxyController::setDefaultSystemProxy()
{
    SystemProxy::setDefaultSystemProxy("127.0.0.1",8888);
    SystemProxy::enableSystemProxy();

}

void ProxyController::restoreDefaultSystemProxy()
{
    SystemProxy::disableSystemProxy();
}
