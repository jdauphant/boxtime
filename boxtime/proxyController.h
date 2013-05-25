#ifndef TINYPROXY_H
#define TINYPROXY_H

#include <QObject>
#include <QProcess>

class TinyProxy: public QObject
{
    Q_OBJECT
public:
    static TinyProxy * getInstance();
    ~TinyProxy();
    const static bool WITH_BLOCKING = true;
    const static bool WITHOUT_BLOCKING = false;

private:
    TinyProxy();
    TinyProxy(const TinyProxy & c): QObject(c.parent()) {}
    TinyProxy & operator=(const TinyProxy &) {return *this;}

    QProcess *proc;

public slots:
    void startProxy(bool blocking);
    void stopProxy();
};

#endif // TINYPROXY_H

