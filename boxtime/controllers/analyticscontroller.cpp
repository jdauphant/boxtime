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
    connect(QApplication::instance(),SIGNAL(lastWindowClosed()),this,SLOT(applicationClose()));

    QVariantMap properties;
    if(firstLaunch)
    {
        sendEvent("First Launch");
        properties.insert("OS Family", settings::OS_FAMILY);
    }
    if(!launchEventSend)
        sendEvent("Launch");

    properties.insert("Last Launch", QDateTime::currentDateTime());
    updateProfil(properties);

    QSize desktopSize = QApplication::desktop()->size();
    QVariantMap resolutionProperties;
    QVariantList resolutions;
    resolutions << QString::number(desktopSize.width())+"x"+QString::number(desktopSize.height());
    resolutionProperties.insert("Resolution", resolutions);
    updateProfil(resolutionProperties, Mixpanel::UNION);
}

void AnalyticsController::unload()
{
    disconnect(TaskController::getInstance(),SIGNAL(started(Task*)),this,SLOT(taskStarted(Task*)));
    disconnect(TaskController::getInstance(),SIGNAL(ended(Task*)),this,SLOT(taskEnded(Task*)));
    disconnect(SettingsController::getInstance(),SIGNAL(valueChanged(QString,QVariant)),this,SLOT(configurationChanged(QString,QVariant)));
    disconnect(QApplication::instance(),SIGNAL(lastWindowClosed()),this,SLOT(applicationClose()));
}

void AnalyticsController::sendEvent(QString name, QVariantMap properties)
{
    QVariantMap sendedProperties = properties.unite(defaultTrackProperties);
    if(mixpanel.track(name, sendedProperties))
        qDebug() << "Event" << name << "sended";
    else
        qWarning() << "Problem to send event";
}

void AnalyticsController::updateProfil(QVariantMap properties, QString operation)
{
    if(mixpanel.engage(properties, operation))
        qDebug() << "Profil updated";
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
    QVariantMap eventProperties;
    eventProperties.insert("Time Elapsed", QVariant::fromValue((int)task->timeElapsed)); // (int) because mixpanel-qt don't support long if qjson version is 0.7.1
    eventProperties.insert("Start Date", task->startDateTime);
    if(task->isRestored())
        eventProperties.insert("Restored", true);
    sendEvent("Task Ended", eventProperties);

    QVariantMap profilProperties;
    profilProperties.insert("Total Task", 1);
    profilProperties.insert("Total Task Time", QVariant::fromValue((int)task->timeElapsed)); // (int) because mixpanel-qt don't support long if qjson version is 0.7.1
    updateProfil(profilProperties, Mixpanel::ADD);
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
    QEventLoop loop;
    connect(&mixpanel, SIGNAL(sended()), &loop, SLOT(quit()));
    loop.exec();
}
