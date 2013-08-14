#include "genericviewmodule.h"
#include "settingscontroller.h"

GenericViewModule::GenericViewModule(QString name, bool defaultEnable, QWidget *parent) :
    QWidget(parent), name(name), defaultEnable(defaultEnable)
{
    SettingsController * settingsController = SettingsController::getInstance();
    connect(settingsController,SIGNAL(valueChanged(QString,QVariant)),this,SLOT(configValueChanged(QString,QVariant)));
}

void GenericViewModule::checkEnable()
{
    if(SettingsController::getInstance()->getValue<bool>(name+"/enable", defaultEnable))
        load();
}

void GenericViewModule::configValueChanged(const QString &key, const QVariant &newValue)
{
    if(name+"/enable"!=key)
        return;

    if(false==newValue)
        load();
    else
        unload();
}
