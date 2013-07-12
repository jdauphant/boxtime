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
}

class SettingsController
{
public:
    ~SettingsController();

    static SettingsController * getInstance();
    static QString getDataPath();
    static QString getDocumentsPath();

    template<typename T>
    T getValue(const QString & key, const QVariant & defaultValue);

public slots:
    void setValue(const QString & key, const QVariant & value);
    void removeValue(const QString & key);

private:
    SettingsController();
    SettingsController(const SettingsController &) {}
    SettingsController & operator=(const SettingsController &) {return *this;}
    QSettings * settings;
};

using namespace settings;
#endif // SETTINGSCONTROLLER_H
