#ifndef GENERICMODULE_H
#define GENERICMODULE_H

#include <QObject>

class GenericModule : public QObject
{
    Q_OBJECT
public:
    GenericModule(QString name, bool defaultEnable, QObject *parent = 0);
    virtual ~GenericModule(){}
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
