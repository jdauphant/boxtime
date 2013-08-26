#ifndef GENERICCONTROLLERMODULE_H
#define GENERICCONTROLLERMODULE_H

#include <QObject>
#include "module.h"

class GenericControllerModule : public QObject, public Module
{
    Q_OBJECT
public:
    GenericControllerModule(QString name, bool defaultEnable, QObject *parent = 0);
    virtual ~GenericControllerModule(){}

public slots:
    void configValueChanged(const QString & key, const QVariant & newValue){ Module::configValueChanged(key,newValue); }
};

#endif // GENERICCONTROLLERMODULE_H
