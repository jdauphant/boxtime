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

private:
    void setDefaultSystemProxy();
    void restoreDefaultSystemProxy();
    bool createConfigurationFiles();

    QProcess *proxyProcess;
    void setGsettingsParams(QString schema, QString key, QString value);  
};

#endif // PROXYCONTROLLER_H

