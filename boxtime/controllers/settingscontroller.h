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
