#ifndef GENERICVIEWMODULE_H
#define GENERICVIEWMODULE_H

#include <QWidget>

class GenericViewModule : public QWidget
{
    Q_OBJECT
public:
    explicit GenericViewModule(QString name, bool defaultEnable, QWidget *parent = 0);
    virtual ~GenericViewModule(){}
    const QString name;
    const bool defaultEnable;
    void checkEnable();

protected slots:
    void configValueChanged(const QString & key, const QVariant & newValue);

public slots:
    virtual void load() =0;
    virtual void unload() =0;
    
};

#endif // GENERICVIEWMODULE_H
