#ifndef PROXYCONTROLLER_H
#define PROXYCONTROLLER_H

#include <QObject>
#include <QProcess>

class ProxyController: public QObject
{
    Q_OBJECT
public:
    static ProxyController * getInstance();
    ~ProxyController();
    const static QString DEFAULT_PROXY_PROCESS;
    const static QString DEFAULT_PROXY_KILLPROCESS;
    const static QStringList DEFAULT_BLOCK_LIST;

private:
    ProxyController();
    ProxyController(const ProxyController & c): QObject(c.parent()) {}
    ProxyController & operator=(const ProxyController &) {return *this;}

    void setupBlockingConfiguration();

    QProcess *proxyProcess;
    void setGsettingsParams(QString schema, QString key, QString value);

public slots:
    void startProxy();
    void stopProxy();
    void setDefaultSystemProxy();
    void restoreDefaultSystemProxy();
};

#endif // PROXYCONTROLLER_H

