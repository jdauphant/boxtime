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
    DEFAULT_PROXY_PROCESS(QString("privoxy"))
#endif
#ifdef Q_OS_WIN32
    DEFAULT_PROXY_PROCESS(QString(""))
#endif
{
    proxyProcess = new QProcess(this);

    TaskController * taskController = TaskController::getInstance();
    connect(taskController,SIGNAL(started(Task *)),this,SLOT(start()));
    connect(taskController,SIGNAL(ended(Task *)),this,SLOT(stop()));
    connect(SettingsController::getInstance(),SIGNAL(valueChanged(QString,QVariant)),this,SLOT(configValueChanged(QString,QVariant)));
    setupBlockingConfiguration();
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

ProxyController * ProxyController::getInstance()
{
    static ProxyController * proxyController = 0;
    if(!proxyController)
    {
        proxyController = new ProxyController();
    }
    return proxyController;
}

void ProxyController::start()
{
    if(false==SettingsController::getInstance()->getValue<bool>("proxy/enable", DEFAULT_PROXY_ENABLE))
    {
        qDebug("Proxy is disable");
        return;
    }

    QString programName = SettingsController::getInstance()->getValue<QString>("proxy/process", DEFAULT_PROXY_PROCESS);
    QString confdir = SettingsController::getInstance()->getValue<QString>("proxy/confdir", DEFAULT_PROXY_CONFDIR);
    QString pidFile = confdir+"/"+programName+".pid";
    QFile().remove(pidFile);
    proxyProcess->setWorkingDirectory(QCoreApplication::applicationDirPath());
    proxyProcess->start(programName ,
                        QStringList() << "--no-daemon" << "--pidfile" << pidFile << confdir+"/config");
    proxyProcess->waitForBytesWritten();
    QString output(proxyProcess->readAllStandardOutput());
    qDebug() << "Start " << programName << " configdir=" << confdir;
    qDebug() << output;

    if(QProcess::Running == proxyProcess->state())
    {
        setDefaultSystemProxy();
    }
    else
    {
        qWarning() << "Fail to start" << programName << ", proxy fonctionality disable";
        SettingsController::getInstance()->setValue("proxy/enable", false);
    }
}

void ProxyController::stop()
{
    if(QProcess::Running == proxyProcess->state())
    {
        restoreDefaultSystemProxy();
        proxyProcess->terminate();
        proxyProcess->waitForFinished();
    }
}

void ProxyController::setDefaultSystemProxy()
{
    SystemProxy::setAndEnableSystemProxy("127.0.0.1", SettingsController::getInstance()->getValue<int>("proxy/port", DEFAULT_PROXY_PORT));
}

void ProxyController::restoreDefaultSystemProxy()
{
    SystemProxy::disableSystemProxy();
}

void ProxyController::configValueChanged(const QString &key, const QVariant &newValue)
{
    if("proxy/enable"!=key)
        return;

    if(false==newValue)
    {
        stop();
    }
    else if(TaskController::getInstance()->asCurrentTask())
    {
        start();
    }
}

void ProxyController::setupBlockingConfiguration()
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
        SettingsController::getInstance()->setValue("proxy/enable", false);
        return;
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
        qWarning() << "impossible to open " << confdir+"/user.action fonctionality disable";
        SettingsController::getInstance()->setValue("proxy/enable", false);
        return;
    }
    QTextStream actiongOut(&actionFile);
    actiongOut << "{ +block{Procrastination website} }" << endl;
    foreach(const QString &block, SettingsController::getInstance()->getValue<QStringList>("proxy/blocklist", DEFAULT_PROXY_BLOCKLIST))
        actiongOut << block << endl;
    actionFile.close();
}
