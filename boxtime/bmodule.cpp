#include "bmodule.h"
#include "settingscontroller.h"

QMap<QString,  BModule *>  BModule::moduleList;

 BModule:: BModule(QString name, bool defaultEnable) :
               name(name), defaultEnable(defaultEnable)
{
    add(name,this);
}

void  BModule::checkEnable()
{
    if(SettingsController::getInstance()->getValue<bool>(name+"/enable", defaultEnable))
        load();
}

void  BModule::configValueChanged(const QString &key, const QVariant &newValue)
{
    if(name+"/enable"!=key)
        return;

    if(true==newValue)
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

void  BModule::add(QString name,  BModule *module)
{
    moduleList[name] = module;
}


const QMap<QString,  BModule *>  BModule::list()
{
    return moduleList;
}

