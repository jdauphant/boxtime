#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QStringList>
#include <QString>
#include <QSettings>

namespace settings
{
const QString ORGANIZATION_NAME = QString("boxtime");
const QString ORGANIZATION_DOMAIN = QString("boxti.me");
const QString APPLICATION_NAME = QString("boxti.me");

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
}

class SettingsController
{
public:
    ~SettingsController();

    static SettingsController * getInstance();

    template<typename T>
    T getValue(const QString & key, const QVariant & defaultValue);

public slots:
    void setValue(const QString & key, const QVariant & value);

private:
    SettingsController();
    SettingsController(const SettingsController &) {}
    SettingsController & operator=(const SettingsController &) {return *this;}
    QSettings * settings;
};

#endif // SETTINGSCONTROLLER_H
