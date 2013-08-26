#include "module.h"
#include "settingscontroller.h"

QMap<QString, Module *> Module::moduleList;

Module::Module(QString name, bool defaultEnable) :
               name(name), defaultEnable(defaultEnable)
{
    add(name,this);
}

void Module::checkEnable()
{
    if(SettingsController::getInstance()->getValue<bool>(name+"/enable", defaultEnable))
        load();
}

void Module::configValueChanged(const QString &key, const QVariant &newValue)
{
    if(name+"/enable"!=key)
        return;

    if(false==newValue)
    {
        load();
        qDebug() << "Module"<< name << "loaded";
    }
    else
    {
        unload();
        qDebug() << "Module"<< name << "unloaded";
    }
}

void Module::add(QString name, Module *module)
{
    moduleList[name] = module;
}


const QMap<QString, Module *> Module::list()
{
    return moduleList;
}

