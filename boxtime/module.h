#ifndef MODULE_H
#define MODULE_H

#include "QtCore"

class Module
{
public:
    virtual ~Module(){}
    const QString name;
    const bool defaultEnable;
    void checkEnable();

protected:
    explicit Module(QString name, bool defaultEnable);
    void configValueChanged(const QString & key, const QVariant & newValue);

public:
    virtual void load() =0;
    virtual void unload() =0;
    static void add(QString name, Module * module);
    static const QMap<QString, Module *> list();

private:
    static QMap<QString, Module *> moduleList;
};

#endif // MODULE_H
