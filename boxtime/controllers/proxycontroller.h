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
    const static bool WITH_BLOCKING = true;
    const static bool WITHOUT_BLOCKING = false;
    const static QString DEFAULT_PROXY_PROCESS;
    const static QString DEFAULT_PROXY_KILLPROCESS;

private:
    ProxyController();
    ProxyController(const ProxyController & c): QObject(c.parent()) {}
    ProxyController & operator=(const ProxyController &) {return *this;}

    QProcess *proxyProcess;

public slots:
    void startProxy(bool blocking);
    void stopProxy();
};

#endif // PROXYCONTROLLER_H

