#include "genericmodule.h"
#include "settingscontroller.h"

GenericModule::GenericModule(QString name, bool defaultEnable, QObject *parent) :
    QObject(parent), name(name), defaultEnable(defaultEnable)
{
    SettingsController * settingsController = SettingsController::getInstance();
    connect(settingsController,SIGNAL(valueChanged(QString,QVariant)),this,SLOT(configValueChanged(QString,QVariant)));
}

void GenericModule::checkEnable()
{
    if(SettingsController::getInstance()->getValue<bool>(name+"/enable", defaultEnable))
        load();
}

void GenericModule::configValueChanged(const QString &key, const QVariant &newValue)
{
    if(name+"/enable"!=key)
        return;

    if(false==newValue)
        load();
    else
        unload();
}
