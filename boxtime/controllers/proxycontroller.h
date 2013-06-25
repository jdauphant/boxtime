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

private:
    ProxyController();
    ProxyController(const ProxyController & c): QObject(c.parent()) {}
    ProxyController & operator=(const ProxyController &) {return *this;}

    void setupBlockingConfiguration();

    QProcess *proxyProcess;
    void setGsettingsParams(QString schema, QString key, QString value);

public slots:
    void start();
    void stop();
    void setDefaultSystemProxy();
    void restoreDefaultSystemProxy();
    void enable(bool enable);
};

#endif // PROXYCONTROLLER_H

