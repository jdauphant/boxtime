#include "settingscontroller.h"

#include <QtCore>
#include <QDesktopServices>
#include <QApplication>
#include "task.h"
#include "startuplaunch.h"

SettingsController::SettingsController()
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(VERSION);
    settings = new QSettings();
    checkStartupLaunch();
    connect(this, SIGNAL(valueChanged(QString,QVariant)),this, SLOT(settingValueChanged(QString,QVariant)));
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
    QString dataFolder = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    if(false==QDir(dataFolder).exists())
    {
        bool mkpathResult = QDir().mkpath(dataFolder);
        qDebug() << "Try to create " << dataFolder << " result=" << mkpathResult;
    }
    return dataFolder;
}

QString SettingsController::getDocumentsPath()
{
    return QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
}

template<typename T>
T SettingsController::getValue(const QString & key, const QVariant & defaultValue)
{
    return this->settings->value(key, defaultValue).value<T>();
}

void SettingsController::setValue(const QString & key, const QVariant & value)
{
    if(this->settings->value(key)!=value)
    {
        this->settings->setValue(key, value);
        valueChanged(key, value);
    }
}

void SettingsController::removeValue(const QString & key)
{
    this->settings->remove(key);
}

void SettingsController::checkStartupLaunch()
{
    if(getValue<bool>("application/onstartup", DEFAULT_STARTUP_LAUNCH)==true)
    {
        if(false==StartupLaunch::isOnStartup(APPLICATION_NAME))
        {
            StartupLaunch::addOnStartup(APPLICATION_NAME, QCoreApplication::applicationFilePath(), APPLICATION_DESCRIPTION);
        }
    }
    else
    {
        StartupLaunch::removeFromStartup(APPLICATION_NAME);
    }
}

void SettingsController::settingValueChanged(const QString &key, const QVariant &)
{
    if("application/onstartup"==key)
    {
        checkStartupLaunch();
    }
}

template QString SettingsController::getValue<QString>(const QString & key, const QVariant & defaultValue);
template bool SettingsController::getValue<bool>(const QString & key, const QVariant & defaultValue);
template int SettingsController::getValue<int>(const QString & key, const QVariant & defaultValue);
template QStringList SettingsController::getValue<QStringList>(const QString & key, const QVariant & defaultValue);
template char SettingsController::getValue<char>(const QString & key, const QVariant & defaultValue);

