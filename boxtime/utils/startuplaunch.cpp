#include "startuplaunch.h"

#include <QDesktopServices>

StartupLaunch::StartupLaunch()
{
}


bool StartupLaunch::addOnStartup(QString name, QString executable, QString comments)
{
#ifdef Q_OS_MAC
return false;
#endif

#ifdef Q_OS_LINUX
QString filepath = QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+"/.config/autostart/"+name.toLower()+".desktop";
QFile file(filepath);
if(false==file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
{
    qWarning() << "Startuplaunch : impossible to open " << filepath;
    return false;
}
QTextStream out(&file);
out << "[Desktop Entry]" << endl
    << "Type=Application" << endl
    << "Exec=" << executable  << endl
    << "Hidden=false" << endl
    << "NoDisplay=false" << endl
    << "X-GNOME-Autostart-enabled=true" << endl
    << "Name[en]=" << name  << endl
    << "Name=" << name << endl
    << "Comment[en]=" << comments << endl
    << "Comment=" << comments << endl;
file.close();
return true;
#endif

#ifdef Q_OS_WIN32
return false;
#endif
}

bool StartupLaunch::removeFromStartup(QString name)
{
#ifdef Q_OS_MAC
return false;
#endif

#ifdef Q_OS_LINUX
return QFile::remove(QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+QString("/.config/autostart/")+name.toLower()+QString(".desktop"));
#endif

#ifdef Q_OS_WIN32
return false;
#endif
}


bool StartupLaunch::isOnStartup(QString name)
{
#ifdef Q_OS_MAC
return false;
#endif

#ifdef Q_OS_LINUX
return QFile::exists(QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+QString("/.config/autostart/")+name.toLower()+QString(".desktop"));
#endif

#ifdef Q_OS_WIN32
return false;
#endif
}
