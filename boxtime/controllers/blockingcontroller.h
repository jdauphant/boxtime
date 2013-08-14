#ifndef BLOCKINGCONTROLLER_H
#define BLOCKINGCONTROLLER_H

#include <QObject>
#include "proxycontroller.h"
#include "genericmodulecontroller.h"

namespace settings
{
    const QStringList DEFAULT_BLOCKING_LIST = QStringList() << "facebook.com"
          << "twitter.com" << "pinterest.com" << "tweetdeck.com"
          << ".facebook.com"  << ".twitter.com" << ".pinterest.com" << ".tweetdeck.com"
          << "9gag.com" << "4chan.org" << ".9gag.com" << ".4chan.org" << "collegehumor.com" << ".collegehumor.com" << "tumblr.com" << ".tumblr.com"
          << "plus.google.com" << "news.google.com" << "lemonde.fr" << ".lemonde.fr" << "reddit.com" << ".reddit.com" << "rue89.com" << ".rue89.com"
          << "imdb.com" << ".imdb.com" << "imdb.fr" << ".imdb.fr" << "allocine.fr" << ".allocine.fr"
             //<< "mail.google.com"
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

private:
    BlockingController();
    BlockingController(const BlockingController & c): GenericModuleController("blocking", DEFAULT_BLOCKING_ENABLE,c.parent()) {}
    BlockingController & operator=(const BlockingController &) {return *this;}
    ProxyController * proxyController;

public slots:
    void block();
    void unblock();
    void load();
    void unload();

signals:

};

#endif // BLOCKINGCONTROLLER_H
