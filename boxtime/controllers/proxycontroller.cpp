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
    ,active(false)
{
    createConfigurationFiles();
    proxyProcess = new QProcess(this);
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

    QNetworkProxy currentSystemProxy = getCurrentSystemProxy();
    if(currentSystemProxy.type() != QNetworkProxy::NoProxy)
    {
        if((currentSystemProxy.hostName() == "localhost" || currentSystemProxy.hostName().startsWith("127.") )
            && currentSystemProxy.port() == getPort())
             qWarning() << "Proxy already setup for boxtime (maybe by another or old instance of boxtime)";
        else
            qWarning() << "Proxy already setup for other system";
        return false;
    }

    QString programName = getProgramPath();
    QString confdir = getConfDir();
    QString pidFile = getPidFile();

    if(QFile().exists(pidFile))
    {
        qWarning() << "Proxy pid file already exist" << pidFile;
        return false;
    }

    connect(proxyProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(handleProcessError(QProcess::ProcessError)));
    connect(proxyProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(handleFinished(int,QProcess::ExitStatus)));
    QStringList proxyArguments;
    proxyArguments << "--no-daemon" << "--pidfile" << pidFile << confdir+"/config";
    proxyProcess->start(programName, proxyArguments);
    proxyProcess->waitForStarted();

    if(QProcess::Running == proxyProcess->state())
    {
        qDebug() << programName << "started pid:" << proxyProcess->pid() << proxyArguments;
        setDefaultSystemProxy();
        active = true;
    }
    else
    {
        restoreDefaultSystemProxy();
        qWarning() << "Fail to start" << programName << proxyArguments;
        return false;
    }
    return true;
}

bool ProxyController::stop()
{
    if(active)
    {
        disconnect(proxyProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(handleProcessError(QProcess::ProcessError)));
        disconnect(proxyProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(handleFinished(int,QProcess::ExitStatus)));
        restoreDefaultSystemProxy();
        proxyProcess->terminate();
        proxyProcess->waitForFinished();
        active = false;
        qDebug() << getProgramPath() << "proxy stopped";
    }
    return true;
}

void ProxyController::setDefaultSystemProxy()
{
    int port = getPort();
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
        << "listen-address 127.0.0.1:" << getPort() << endl
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
    installTemplateFromRessource("blocked", confdir);
    installTemplateFromRessource("cgi-style.css", confdir);
    qDebug() << "Privoxy config writed";
    return true;
}

void ProxyController::installTemplateFromRessource(QString templateName, QString confdir)
{
    QString templateFilePath = confdir+"/templates/"+templateName;
    if(true==QFile().exists(templateFilePath))
    {
        qDebug() << "Privoxy template already installed" << templateName;
        return;
    }
    QFile templateFile(templateFilePath);
    if(false==templateFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
         qWarning() << "Impossible to open template file" << templateFilePath;
         return;
    }
    QTextStream templateFileOut(&templateFile);

    QFile templateResourceFile("://ressources/privoxy/templates/"+templateName);
    if(false==templateResourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
         qWarning() << "Impossible to open template ressource file" << "://ressources/privoxy/templates/"+templateName;
         return;
    }
    QTextStream templateFileIn(&templateResourceFile);
    templateFileOut << templateFileIn.readAll();
    templateFile.close();
    templateResourceFile.close();
    qDebug() << "Privoxy template installed : " << templateName;
}

QString ProxyController::getPidFile()
{
    return getConfDir()+"/process.pid";
}

QString ProxyController::getConfDir()
{
    return SettingsController::getInstance()->getValue<QString>("proxy/confdir", DEFAULT_PROXY_CONFDIR);
}

QString ProxyController::getProgramPath()
{
    return SettingsController::getInstance()->getValue<QString>("proxy/process", DEFAULT_PROXY_PROCESS);
}

int ProxyController::getPort()
{
    return SettingsController::getInstance()->getValue<int>("proxy/port", DEFAULT_PROXY_PORT);
}

QNetworkProxy ProxyController::getCurrentSystemProxy()
{
    QNetworkProxyQuery testNetworkQuery(QUrl("http://www.google.com"));
    QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(testNetworkQuery);
    foreach ( QNetworkProxy loopItem, listOfProxies ) {
        qDebug() << "proxy sytem find " << loopItem.hostName() << loopItem.port();
    }
    return listOfProxies.first();
}

void ProxyController::handleProcessError(QProcess::ProcessError error)
{
    qWarning() << "Error with proxy process pid:" << proxyProcess->pid() <<  "error:" << error;
}

void ProxyController::handleFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Proxy Process finished pid:" << proxyProcess->pid() <<  "exitCode:" << exitCode<<  "exitStatus:" << exitStatus;
    restoreDefaultSystemProxy();
    active=false;
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

bool ProxyController::isActive()
{
    return active;
}

