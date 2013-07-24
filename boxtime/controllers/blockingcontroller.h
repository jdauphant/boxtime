#ifndef BLOCKINGCONTROLLER_H
#define BLOCKINGCONTROLLER_H

#include <QObject>
#include "proxycontroller.h"
#include "genericmodulecontroller.h"

namespace settings
{
    const QStringList DEFAULT_BLOCKING_LIST = QStringList() << "facebook.com" << "9gag.com" << "4chan.org"
          << "twitter.com" << "pinterest.com" << "tweetdeck.com"
          << ".facebook.com" << ".9gag.com" << ".4chan.org" << ".twitter.com" << ".pinterest.com" << ".tweetdeck.com"
          << "collegehumor.com" << ".collegehumor.com" << "reddit.com" << "tumblr.com" << ".tumblr.com" <<
             ".reddit.com" << "lemonde.fr" << ".lemonde.fr"
          << "plus.google.com" << "news.google.com" //<< "mail.google.com"
          //<< "outlook.com" << ".outlook.com" << "hotmail.com" << "www.hotmail.com" << "mail.live.com"
          //<< "hotmail.fr" << "www.hotmail.fr" << "mail.yahoo.com" << ".mail.yahoo.com"
             ;
#ifdef Q_OS_UNIX
    const bool DEFAULT_BLOCKING_ENABLE = true;
#endif

#ifdef Q_OS_WIN32
    const bool DEFAULT_BLOCKING_ENABLE = false;
#endif

}

class BlockingController : public GenericModuleController
{
    Q_OBJECT
public:
    static BlockingController * getInstance();
    ~BlockingController(){}
    bool isActive();
    void load();
    void unload();

private:
    BlockingController();
    BlockingController(const BlockingController & c): GenericModuleController("blocking", DEFAULT_BLOCKING_ENABLE,c.parent()) {}
    BlockingController & operator=(const BlockingController &) {return *this;}
    ProxyController * proxyController;

public slots:
    void block();
    void unblock();

signals:

};

#endif // BLOCKINGCONTROLLER_H
