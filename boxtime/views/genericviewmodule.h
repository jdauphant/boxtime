#ifndef GENERICVIEWMODULE_H
#define GENERICVIEWMODULE_H

#include <QWidget>
#include "bmodule.h"

class GenericViewModule : public QWidget, public  BModule
{
    Q_OBJECT
public:
    explicit GenericViewModule(QString name, bool defaultEnable, QWidget *parent = 0);
    virtual ~GenericViewModule(){}

public slots:
    void configValueChanged(const QString & key, const QVariant & newValue){  BModule::configValueChanged(key,newValue); }
};

#endif // GENERICVIEWMODULE_H
