#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QString>
#include <QSettings>

class SettingsController
{
public:
    ~SettingsController();
    static const QString ORGANIZATION_NAME;
    static const QString ORGANIZATION_DOMAIN;
    static const QString APPLICATION_NAME;

    template<typename T>
    inline static T getValue(const QString & key, const QVariant & defaultValue);

private:
    SettingsController();
    static SettingsController * getInstance();
    SettingsController(const SettingsController & c) {}
    SettingsController & operator=(const SettingsController &) {return *this;}
    QSettings * settings;
};

#endif // SETTINGSCONTROLLER_H
