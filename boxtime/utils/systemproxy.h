#ifndef SYSTEMPROXY_H
#define SYSTEMPROXY_H

#include <QtCore>

class SystemProxy
{
public:
    static void setAndEnableSystemProxy(const QString address, const unsigned int port);
    static void disableSystemProxy();
    static bool isThatPosibleToChangeProxy();

private:
    SystemProxy();
    SystemProxy(const SystemProxy &) {}
    SystemProxy & operator=(const SystemProxy &) {return *this;}

    static void setGsettingsParams(QString schema, QString key, QString value);
    static void setMacOSXChangeProxy(const QStringList params);
};

#endif // SYSTEMPROXY_H
