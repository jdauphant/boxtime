#include "settingscontroller.h"

#include <QtCore>

const QString SettingsController::ORGANIZATION_NAME = QString("boxtime");
const QString SettingsController::ORGANIZATION_DOMAIN = QString("boxti.me");
const QString SettingsController::APPLICATION_NAME = QString("<boxti.me/>");

SettingsController::SettingsController()
{
    QCoreApplication::setOrganizationName(this->ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(this->ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(this->APPLICATION_NAME);
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
    return getInstance()->settings->value(key, defaultValue).value<T>();
}

template QString SettingsController::getValue<QString>(const QString & key, const QVariant & defaultValue);
