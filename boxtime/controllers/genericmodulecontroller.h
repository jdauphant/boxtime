#ifndef GENERICMODULE_H
#define GENERICMODULE_H

#include <QObject>

class GenericModuleController : public QObject
{
    Q_OBJECT
public:
    GenericModuleController(QString name, bool defaultEnable, QObject *parent = 0);
    virtual ~GenericModuleController(){}
    const QString name;
    const bool defaultEnable;
    void checkEnable();

protected slots:
    void configValueChanged(const QString & key, const QVariant & newValue);

public slots:
    virtual void load() =0;
    virtual void unload() =0;
};

#endif // GENERICMODULE_H
