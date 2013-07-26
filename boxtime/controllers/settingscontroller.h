#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QStringList>
#include <QString>
#include <QSettings>

namespace settings
{
const QString ORGANIZATION_NAME = QString("boxtime");
const QString ORGANIZATION_DOMAIN = QString("boxti.me");
#ifndef QT_NO_DEBUG
const QString APPLICATION_NAME = QString("boxtime_debug");
#else
const QString APPLICATION_NAME = QString("boxtime");
#endif
const QString APPLICATION_DESCRIPTION = QString("Boxtime is a little widget that help you to keep focus on one task.");
const QString VERSION = QString("0.6.4");
const QString MAINTAINER = QString("Julien DAUPHANT <julien@boxti.me>");
const QString WEBSITE_HOMEPAGE = QString("http://www.boxti.me");
const QString DEFAULT_LOGFILE = QString("/logfile.txt");

const bool DEFAULT_STARTUP_LAUNCH = true;
}

class SettingsController: public QObject
{
    Q_OBJECT
public:
    ~SettingsController();

    static SettingsController * getInstance();
    QString getDataPath();
    static QString getDocumentsPath();

    template<typename T>
    T getValue(const QString & key, const QVariant & defaultValue);

public slots:
    void setValue(const QString & key, const QVariant & value);
    void removeValue(const QString & key);
    void checkStartupLaunch();

signals:
    void valueChanged(const QString & key, const QVariant & newValue);

private slots:
    void settingValueChanged(const QString & key, const QVariant &);

private:
    SettingsController();
    SettingsController(const SettingsController &c): QObject(c.parent()) {}
    SettingsController & operator=(const SettingsController &) {return *this;}
    QSettings * settings;
};

using namespace settings;
#endif // SETTINGSCONTROLLER_H
