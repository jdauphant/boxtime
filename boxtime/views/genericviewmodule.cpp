#include "genericviewmodule.h"
#include "settingscontroller.h"

GenericViewModule::GenericViewModule(QString name, bool defaultEnable, QWidget *parent) :
    QWidget(parent),  BModule(name, defaultEnable)
{
    SettingsController * settingsController = SettingsController::getInstance();
    connect(settingsController,SIGNAL(valueChanged(QString,QVariant)),this,SLOT(configValueChanged(QString,QVariant)));
}
