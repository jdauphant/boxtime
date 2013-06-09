#ifndef SYSTEMPROXY_H
#define SYSTEMPROXY_H

#include <QtCore>

class SystemProxy
{
public:
    static void setDefaultSystemProxy(const QString address, const unsigned int port);
    static void enableSystemProxy();
    static void disableSystemProxy();

private:
    SystemProxy();
    SystemProxy(const SystemProxy & c) {}
    SystemProxy & operator=(const SystemProxy &) {return *this;}

    static void setGsettingsParams(QString schema, QString key, QString value);
};

#endif // SYSTEMPROXY_H
