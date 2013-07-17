#ifndef PROXYCONTROLLER_H
#define PROXYCONTROLLER_H

#include <QObject>
#include <QProcess>
#include <settingscontroller.h>

namespace settings
{
const QStringList DEFAULT_PROXY_BLOCKLIST = QStringList() << "facebook.com" << "9gag.com" << "4chan.org"
          << "twitter.com" << "pinterest.com" << "tweetdeck.com"
          << ".facebook.com" << ".9gag.com" << ".4chan.org" << ".twitter.com" << ".pinterest.com" << ".tweetdeck.com"
          << "collegehumor.com" << ".collegehumor.com" << "reddit.com" << ".reddit.com"
          << "plus.google.com" << "news.google.com" //<< "mail.google.com"
          //<< "outlook.com" << ".outlook.com" << "hotmail.com" << "www.hotmail.com" << "mail.live.com"
          //<< "hotmail.fr" << "www.hotmail.fr" << "mail.yahoo.com" << ".mail.yahoo.com"
             ;

#ifdef Q_OS_MAC
const QString DEFAULT_PROXY_PROCESS = QString("./privoxy");
const bool DEFAULT_PROXY_ENABLE = false;
#endif

#ifdef Q_OS_LINUX
const QString DEFAULT_PROXY_PROCESS = QString("privoxy");
const bool DEFAULT_PROXY_ENABLE = true;
#endif

#ifdef Q_OS_WIN32
const QString DEFAULT_PROXY_PROCESS = QString("");
const bool DEFAULT_PROXY_ENABLE = false;
#endif

const int DEFAULT_PROXY_PORT = 18118;

}

class ProxyController: public QObject
{
    Q_OBJECT
public:
    static ProxyController * getInstance();
    ~ProxyController();
    const QString DEFAULT_PROXY_CONFDIR;

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

