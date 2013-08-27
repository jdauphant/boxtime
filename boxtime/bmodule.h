#ifndef BMODULE_H
#define BMODULE_H

#include "QtCore"

class  BModule
{
public:
    virtual ~ BModule(){}
    const QString name;
    const bool defaultEnable;
    void checkEnable();

protected:
    explicit  BModule(QString name, bool defaultEnable);
    void configValueChanged(const QString & key, const QVariant & newValue);

public:
    virtual void load() =0;
    virtual void unload() =0;
    static void add(QString name,  BModule * module);
    static const QMap<QString,  BModule *> list();

private:
    static QMap<QString,  BModule *> moduleList;
};

#endif // BMODULE_H
