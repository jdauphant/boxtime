#ifndef GENERICCONTROLLERMODULE_H
#define GENERICCONTROLLERMODULE_H

#include <QObject>
#include "bmodule.h"

class GenericControllerModule : public QObject, public  BModule
{
    Q_OBJECT
public:
    GenericControllerModule(QString name, bool defaultEnable, QObject *parent = 0);
    virtual ~GenericControllerModule(){}

public slots:
    void configValueChanged(const QString & key, const QVariant & newValue){  BModule::configValueChanged(key,newValue); }
};

#endif // GENERICCONTROLLERMODULE_H
