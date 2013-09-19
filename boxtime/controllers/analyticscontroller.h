#ifndef ANALYTICSCONTROLLER_H
#define ANALYTICSCONTROLLER_H

#include <QtCore>
#include "genericcontrollermodule.h"
#include <settingscontroller.h>
#include "mixpanel.h"
#include "task.h"

namespace settings
{
    const bool DEFAULT_ANALYTICS_ENABLE = true;
#ifndef QT_NO_DEBUG
    const QString DEFAULT_MIXPANEL_TOKEN = "30c717e1d2f0e9aa287140301982fda0";
#else
    const QString DEFAULT_MIXPANEL_TOKEN = "313b3217320d044eadce6e51a1d72fb0";
#endif
}

class AnalyticsController : public GenericControllerModule
{
    Q_OBJECT
public:
    explicit AnalyticsController(QObject *parent = 0);
    
signals:

private:
    QString uniqueId;
    Mixpanel mixpanel;
    bool firstLaunch;
    bool launchEventSend;
    QVariantMap defaultTrackProperties;

public slots:
    void load();
    void unload();
    void sendEvent(QString name, QVariantMap properties = QVariantMap());
    void updateProfil(QVariantMap properties, QString operation = Mixpanel::SET);

private slots:
    void taskStarted(Task * task);
    void taskEnded(Task * task);
    void configurationChanged(const QString & key, const QVariant & newValue);
    void applicationClose();
};

#endif // ANALYTICSCONTROLLER_H
