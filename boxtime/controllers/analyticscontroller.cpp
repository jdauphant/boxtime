#include "analyticscontroller.h"
#include "settingscontroller.h"
#include "taskcontroller.h"
#include "taskwidget.h"
#include <QApplication>
#include <QDesktopWidget>

AnalyticsController::AnalyticsController(QObject *parent) :
    GenericControllerModule("analytics", DEFAULT_ANALYTICS_ENABLE), mixpanel(DEFAULT_MIXPANEL_TOKEN), firstLaunch(false), launchEventSend(false)
{

    uniqueId = SettingsController::getInstance()->getValue<QString>("analytics/uniqueId", QString());
    if(QString()==uniqueId)
    {
        uniqueId = QUuid::createUuid().toString();
        SettingsController::getInstance()->setValue("analytics/uniqueId", uniqueId);
        firstLaunch = true;
        qDebug() << "New analytics unique id generated" << uniqueId;
    }
    qDebug() << "Analytics unique id" << uniqueId;
    mixpanel.setDistinct_id(uniqueId);
    defaultTrackProperties.insert("OS Family", settings::OS_FAMILY);
}

void AnalyticsController::load()
{
    connect(TaskController::getInstance(),SIGNAL(started(Task*)),this,SLOT(taskStarted(Task*)));
    connect(TaskController::getInstance(),SIGNAL(ended(Task*)),this,SLOT(taskEnded(Task*)));
    connect(SettingsController::getInstance(),SIGNAL(valueChanged(QString,QVariant)),this,SLOT(configurationChanged(QString,QVariant)));

    if(firstLaunch)
    {
        sendEvent("First Launch");
        QVariantMap properties;
        properties.insert("OS Family", settings::OS_FAMILY);
        QSize desktopSize = QApplication::desktop()->size();
        properties.insert("Resolution", QString::number(desktopSize.width())+"x"+QString::number(desktopSize.height()));
        updateProfil(properties);
    }
    if(!launchEventSend)
    {
        sendEvent("Launch");
        QVariantMap properties;
        properties.insert("Last Launch", QDateTime::currentDateTime());
        updateProfil(properties);
    }
}

void AnalyticsController::unload()
{
    disconnect(TaskController::getInstance(),SIGNAL(started(Task*)),this,SLOT(taskStarted(Task*)));
    disconnect(TaskController::getInstance(),SIGNAL(ended(Task*)),this,SLOT(taskEnded(Task*)));
    disconnect(SettingsController::getInstance(),SIGNAL(valueChanged(QString,QVariant)),this,SLOT(configurationChanged(QString,QVariant)));
}

void AnalyticsController::sendEvent(QString name, QVariantMap properties)
{
    QVariantMap sendedProperties = properties.unite(defaultTrackProperties);
    if(mixpanel.track(name, sendedProperties))
        qDebug() << "Event" << name << "sended" << sendedProperties;
    else
        qWarning() << "Problem to send event";
}

void AnalyticsController::updateProfil(QVariantMap properties)
{
    if(mixpanel.engage(properties))
        qDebug() << "Profil updated" << properties;
    else
        qWarning() << "Problem to update profil";

}

void AnalyticsController::taskStarted(Task *task)
{
    QVariantMap properties;
    properties.insert("Name Length", task->name.length());
    if(task->isRestored())
        sendEvent("Task Restored",properties);
    else
        sendEvent("Task Started",properties);
}

void AnalyticsController::taskEnded(Task *task)
{
    QVariantMap properties;
    properties.insert("Time Elapsed", QVariant::fromValue(task->timeElapsed));
    properties.insert("Start Date", task->startDateTime);
    if(task->isRestored())
        properties.insert("Restored", true);
    sendEvent("Task Ended",properties);
}

void AnalyticsController::configurationChanged(const QString &key, const QVariant &newValue)
{
    if(key.endsWith("enable"))
    {
        QVariantMap properties;
        QStringList split = key.split("/");
        properties.insert(split[0], newValue);
        updateProfil(properties);
    }
}

void AnalyticsController::applicationClose()
{
    sendEvent("Close");
}
