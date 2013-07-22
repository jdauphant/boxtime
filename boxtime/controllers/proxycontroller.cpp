#include "proxycontroller.h"

#include <QtCore>
#include <QtNetwork>
#include <QApplication>
#include "systemproxy.h"
#include "taskcontroller.h"

ProxyController::ProxyController():
    DEFAULT_PROXY_CONFDIR(SettingsController::getInstance()->getDataPath()+QString("/privoxy")),

#ifdef Q_OS_MAC
    DEFAULT_PROXY_PROCESS(QCoreApplication::applicationDirPath()+QString("/privoxy"))
#endif
#ifdef Q_OS_LINUX
    DEFAULT_PROXY_PROCESS(QString("/usr/sbin/privoxy"))
#endif
#ifdef Q_OS_WIN32
    DEFAULT_PROXY_PROCESS(QString(""))
#endif
{
    proxyProcess = new QProcess(this);
    createConfigurationFiles();

    if(false==SystemProxy::isThatPosibleToChangeProxy())
    {
        qDebug("Proxy setup unvailable, fonctionality disable");
        SettingsController::getInstance()->setValue("proxy/enable", false);
    }
}

ProxyController::~ProxyController()
{
    stop();
    delete proxyProcess;
}

bool ProxyController::start()
{
    if(false==isConfigurationOk() && false==createConfigurationFiles())
        return false;

    QString programName = SettingsController::getInstance()->getValue<QString>("proxy/process", DEFAULT_PROXY_PROCESS);
    QString confdir = SettingsController::getInstance()->getValue<QString>("proxy/confdir", DEFAULT_PROXY_CONFDIR);
    QString pidFile = confdir+"/"+programName+".pid";
    QFile().remove(pidFile);
    proxyProcess->start(programName ,
                        QStringList() << "--no-daemon" << "--pidfile" << pidFile << confdir+"/config");
    proxyProcess->waitForStarted();
    QString output(proxyProcess->readAllStandardOutput());
    qDebug() << "Start " << programName << " configdir=" << confdir;

    if(QProcess::Running == proxyProcess->state())
    {
        setDefaultSystemProxy();
    }
    else
    {
        qWarning() << "Fail to start" << programName;
        return false;
    }
    return true;
}

bool ProxyController::stop()
{
    if(QProcess::Running == proxyProcess->state())
    {
        restoreDefaultSystemProxy();
        proxyProcess->terminate();
        proxyProcess->waitForFinished();
        qDebug() << SettingsController::getInstance()->getValue<QString>("proxy/process", DEFAULT_PROXY_PROCESS) << "proxy stopped";
    }
    return true;
}

void ProxyController::setDefaultSystemProxy()
{
    SystemProxy::setAndEnableSystemProxy("127.0.0.1", SettingsController::getInstance()->getValue<int>("proxy/port", DEFAULT_PROXY_PORT));
}

void ProxyController::restoreDefaultSystemProxy()
{
    SystemProxy::disableSystemProxy();
}

bool ProxyController::createConfigurationFiles()
{
    QString confdir = SettingsController::getInstance()->getValue<QString>("proxy/confdir", DEFAULT_PROXY_CONFDIR);
    if(false==QDir().exists(confdir))
        QDir().mkdir(confdir);
    if(false==QDir().exists(confdir+"/templates"))
        QDir().mkdir(confdir+"/templates");
    if(false==QDir().exists(confdir+"/log"))
        QDir().mkdir(confdir+"/log");
    QFile configFile(confdir+"/config");
    if(false==configFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qWarning() << "impossible to open " << confdir+"/config fonctionality disable";
        return false;
    }
    QTextStream configOut(&configFile);
    configOut << "confdir " << confdir << endl
        << "logdir " << confdir << "/log" << endl
        << "logfile logfile" << endl
        << "actionsfile user.action" << endl
        << "listen-address 127.0.0.1:" << SettingsController::getInstance()->getValue<int>("proxy/port", DEFAULT_PROXY_PORT) << endl
        << "toggle  1" << endl
        << "enable-remote-toggle  0" << endl
        << "enable-remote-http-toggle  0" << endl
        << "enable-edit-actions 0" << endl
        << "enforce-blocks 0" << endl
        << "allow-cgi-request-crunching 0" << endl
        << "split-large-forms 0" << endl
        << "keep-alive-timeout 15" << endl
        << "tolerate-pipelining 1" << endl
        << "max-client-connections 256" << endl;
    configFile.close();

    QFile actionFile(confdir+"/user.action");
    if(false==actionFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qWarning() << "Impossible to open " << confdir+"/user.action";
        return false;
    }
    actionFile.close();
    qDebug() << "Privoxy config writed";
}


bool ProxyController::setBlockingList(QStringList blockingList)
{
    QString confdir = SettingsController::getInstance()->getValue<QString>("proxy/confdir", DEFAULT_PROXY_CONFDIR);
    if(false==QDir().exists(confdir))
        QDir().mkdir(confdir);
    QFile actionFile(confdir+"/user.action");
    if(false==actionFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qWarning() << "impossible to open " << confdir+"/user.action fonctionality disable";
        return false;
    }
    QTextStream actiongOut(&actionFile);
    actiongOut << "{ +block{Procrastination website} }" << endl;
    foreach(const QString &block, blockingList)
        actiongOut << block << endl;
    actionFile.close();
    qDebug() << "Proxy blocking list changed";
}

bool ProxyController::isConfigurationOk()
{
    QString confdir = SettingsController::getInstance()->getValue<QString>("proxy/confdir", DEFAULT_PROXY_CONFDIR);
    if(false==QDir().exists(confdir))
        return false;
    if(false==QDir().exists(confdir+"/templates"))
        return false;
    if(false==QDir().exists(confdir+"/log"))
        return false;
    if(false==QFile().exists(confdir+"/config"))
        return false;
    if(false==QFile().exists(confdir+"/user.action"))
        return false;
    return true;
}

