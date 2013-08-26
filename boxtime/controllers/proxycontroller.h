#ifndef PROXYCONTROLLER_H
#define PROXYCONTROLLER_H

#include <QtCore>
#include <settingscontroller.h>

namespace settings
{
    const int DEFAULT_PROXY_PORT = 18118;
}

class ProxyController: public QObject
{
    Q_OBJECT
public:
    ProxyController();
    ~ProxyController();
    const QString DEFAULT_PROXY_CONFDIR;
    const QString DEFAULT_PROXY_PROCESS;
    bool start();
    bool stop();
    bool setBlockingList(QStringList blockingList);
    bool isConfigurationOk();
    bool isChangeProxyOk();

private slots:
    void handleProcessError(QProcess::ProcessError error);
    void handleFinished( int exitCode, QProcess::ExitStatus exitStatus );

private:
    void setDefaultSystemProxy();
    void restoreDefaultSystemProxy();
    bool createConfigurationFiles();
    void installTemplateFromRessource(QString templateName, QString confdir);
    QProcess *proxyProcess;
};

#endif // PROXYCONTROLLER_H

