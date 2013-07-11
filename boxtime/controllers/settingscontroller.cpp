#include "settingscontroller.h"

#include <QtCore>
#include <QDesktopServices>

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

QString SettingsController::getDataPath()
{
    getInstance();
    QString dataFolder = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    if(false==QDir(dataFolder).exists())
    {
        bool mkpathResult = QDir().mkpath(dataFolder);
        qDebug() << "Try to create " << dataFolder << " result=" << mkpathResult;
    }
    return dataFolder;
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
