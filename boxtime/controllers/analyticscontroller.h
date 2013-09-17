#ifndef ANALYTICSCONTROLLER_H
#define ANALYTICSCONTROLLER_H

#include <QtCore>
#include "genericcontrollermodule.h"
#include <settingscontroller.h>

namespace settings
{
    const bool DEFAULT_ANALYTICS_ENABLE = true;
}

class AnalyticsController : public GenericControllerModule
{
    Q_OBJECT
public:
    explicit AnalyticsController(QObject *parent = 0);
    
signals:
    
public slots:
    void load();
    void unload();
    
};

#endif // ANALYTICSCONTROLLER_H
