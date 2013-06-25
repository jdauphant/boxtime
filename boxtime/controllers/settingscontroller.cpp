#include "settingscontroller.h"

#include <QtCore>

using namespace settings;

SettingsController::SettingsController()
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    settings = new QSettings();
}

SettingsController::~SettingsController()
{
    delete settings;
}

SettingsController * SettingsController::getInstance()
{
    static SettingsController * settingsController = 0;
    if(!settingsController)
    {
        settingsController = new SettingsController();
    }
    return settingsController;
}

template<typename T>
T SettingsController::getValue(const QString & key, const QVariant & defaultValue)
{
    return this->settings->value(key, defaultValue).value<T>();
}

void SettingsController::setValue(const QString & key, const QVariant & value)
{
    this->settings->setValue(key, value);
}


template QString SettingsController::getValue<QString>(const QString & key, const QVariant & defaultValue);
template bool SettingsController::getValue<bool>(const QString & key, const QVariant & defaultValue);
