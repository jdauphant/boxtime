#include "genericcontrollermodule.h"
#include "settingscontroller.h"

GenericControllerModule::GenericControllerModule(QString name, bool defaultEnable, QObject *parent) :
    QObject(parent),  BModule(name,defaultEnable)
{
    SettingsController * settingsController = SettingsController::getInstance();
    connect(settingsController,SIGNAL(valueChanged(QString,QVariant)),this,SLOT(configValueChanged(QString,QVariant)));
}
