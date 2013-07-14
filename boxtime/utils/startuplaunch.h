#ifndef STARTUPLAUNCH_H
#define STARTUPLAUNCH_H

#include <QtCore>

class StartupLaunch
{
public:
    static bool addOnStartup(QString name, QString executable, QString comments = "");
    static bool removeFromStartup(QString name);
    static bool isOnStartup(QString name);

private:
    StartupLaunch();
    StartupLaunch(const StartupLaunch &) {}
    StartupLaunch & operator=(const StartupLaunch &) {return *this;}
};

#endif // STARTUPLAUNCH_H
