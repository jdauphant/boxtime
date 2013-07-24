#include "proxycontroller.h"

#include <QtCore>
#include <QtNetwork>
#include <QApplication>
#include "systemproxy.h"
#include "taskcontroller.h"

ProxyController::ProxyController(): QObject(0),
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
    connect(proxyProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(handleProcessError(QProcess::ProcessError)));
    connect(proxyProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(handleFinished(int,QProcess::ExitStatus)));
    proxyProcess->start(programName ,
                        QStringList() << "--no-daemon" << "--pidfile" << pidFile << confdir+"/config");
    proxyProcess->waitForStarted();

    if(QProcess::Running == proxyProcess->state())
    {
        qDebug() << programName << "started pid:" << proxyProcess->pid() <<"configdir:" << confdir;
        setDefaultSystemProxy();

    }
    else
    {
        restoreDefaultSystemProxy();
        qWarning() << "Fail to start" << programName;
        return false;
    }
    return true;
}

bool ProxyController::stop()
{
    if(QProcess::Running == proxyProcess->state())
    {
        disconnect(proxyProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(handleProcessError(QProcess::ProcessError)));
        disconnect(proxyProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(handleFinished(int,QProcess::ExitStatus)));
        restoreDefaultSystemProxy();
        proxyProcess->terminate();
        proxyProcess->waitForFinished();
        qDebug() << SettingsController::getInstance()->getValue<QString>("proxy/process", DEFAULT_PROXY_PROCESS) << "proxy stopped";
    }
    return true;
}

void ProxyController::setDefaultSystemProxy()
{
    int port = SettingsController::getInstance()->getValue<int>("proxy/port", DEFAULT_PROXY_PORT);
    QString address = "127.0.0.1";
    SystemProxy::setAndEnableSystemProxy(address, port);
    qDebug() << "System proxy set to" << address << ":" << port;
}

void ProxyController::restoreDefaultSystemProxy()
{
    SystemProxy::disableSystemProxy();
    qDebug() << "System proxy restore to default";
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
    return true;
}

void ProxyController::handleProcessError(QProcess::ProcessError error)
{
    qWarning() << "Error with proxy process pid:" << proxyProcess->pid() <<  "error:" << error;
}

void ProxyController::handleFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Proxy Process finished pid:" << proxyProcess->pid() <<  "exitCode:" << exitCode<<  "exitStatus:" << exitStatus;
    restoreDefaultSystemProxy();
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
    return true;
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

bool ProxyController::isChangeProxyOk()
{
    bool result =  SystemProxy::isThatPosibleToChangeProxy();
    if(false==result)
    {
        qDebug("Proxy setup unvailable");
    }
    return result;
}

