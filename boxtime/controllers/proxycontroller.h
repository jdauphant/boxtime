#ifndef PROXYCONTROLLER_H
#define PROXYCONTROLLER_H

#include <QObject>
#include <QProcess>

namespace settings
{
/* const QStringList DEFAULT_BLOCK_LIST = QStringList() << "facebook.com" << "9gag.com" << "4chan.org"
          << "twitter.com" << "pinterest.com" << "tweetdeck.com"
          << "*.facebook.com" << "*.9gag.com" << "*.4chan.org" << "*.twitter.com" << "*.pinterest.com" << "*.tweetdeck.com"
          << "collegehumor.com" << "*.collegehumor.com" << "reddit.com" << "*.reddit.com"
          << "plus.google.com" << "mail.google.com" << "news.google.com"
          << "outlook.com" << "*.outlook.com" << "hotmail.com" << "www.hotmail.com" << "mail.live.com"
          << "hotmail.fr" << "www.hotmail.fr" << "mail.yahoo.com" << "*.mail.yahoo.com";*/

#ifdef Q_OS_MAC
const QString DEFAULT_PROXY_PROCESS = QString("./tinyproxy");
const QString DEFAULT_PROXY_KILLPROCESS = QString("killall tinyproxy");
const bool DEFAULT_PROXY_ENABLE = true;
#endif

#ifdef Q_OS_LINUX
const QString DEFAULT_PROXY_PROCESS = QString("tinyproxy_");
const QString DEFAULT_PROXY_KILLPROCESS = QString("pkill tinyproxy");
const bool DEFAULT_PROXY_ENABLE = true;
#endif

#ifdef Q_OS_WIN32
const QString DEFAULT_PROXY_PROCESS = QString("");
const QString DEFAULT_PROXY_KILLPROCESS = QString("");
const bool DEFAULT_PROXY_ENABLE = false;
#endif

//const QString DEFAULT_PROXY_CONF_FOLDER = SettingsController::getDataPath()+QString("/privoxy");
}

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

private slots:
    void start();
    void stop();
    void setDefaultSystemProxy();
    void restoreDefaultSystemProxy();
    void configValueChanged(const QString & key, const QVariant & newValue);
};

#endif // PROXYCONTROLLER_H

