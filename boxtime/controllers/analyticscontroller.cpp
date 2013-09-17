#include "analyticscontroller.h"

AnalyticsController::AnalyticsController(QObject *parent) :
    GenericControllerModule("analytics", DEFAULT_ANALYTICS_ENABLE)
{
}

void AnalyticsController::load()
{
}

void AnalyticsController::unload()
{
}
