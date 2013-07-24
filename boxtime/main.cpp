#include <QtCore>
#include <QDesktopWidget>
#include <QFontDatabase>
#include <QApplication>
#include <iostream>

#include "taskwidget.h"
#include "taskcontroller.h"
#include "blockingcontroller.h"

void fileMessageHandler(QtMsgType type, const char *message)
{
    QString logText;
    unsigned int timestamp = QDateTime::currentDateTime().toTime_t();
    switch (type) {
    case QtDebugMsg:
        logText = QString("%1 DEBUG %2").arg(timestamp).arg(message);
        break;
    case QtWarningMsg:
        logText = QString("%1 WARNING: %2").arg(timestamp).arg(message);
        break;
    case QtCriticalMsg:
        logText = QString("%1 CRITICAL : %2").arg(timestamp).arg(message);
        break;
    case QtFatalMsg:
        logText = QString("%1 FATAL: %2").arg(timestamp).arg(message);
        abort();
    }

    QFile logFile(SettingsController::getInstance()->getValue<QString>("log/file", SettingsController::getInstance()->getDataPath()+DEFAULT_LOGFILE));
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream textStream(&logFile);
    textStream << logText << endl;
    std::cout << logText.toAscii().data();
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);
    qInstallMsgHandler(fileMessageHandler);
    qDebug() <<  "************* Application v" << VERSION << "started *************";
    qDebug() <<  "Pid :" << QCoreApplication::applicationPid();
    qDebug() <<  "Current directory :" << QDir::currentPath();
    qDebug() <<  "App directory :" << QCoreApplication::applicationDirPath();
    qDebug() <<  "QtVersion :" << qVersion();
    qDebug("Install fonts");
    if(-1 == QFontDatabase::addApplicationFont("://ressources/Nexa Light.otf"))
    {
        qDebug() << "Impossible to install font Nexa Light.otf";
    }
    if(-1 == QFontDatabase::addApplicationFont("://ressources/Nexa Bold.otf"))
    {
        qDebug() << "Impossible to install font Nexa Bold.otf";
    }

    qDebug("Load GUI");
    TaskWidget taskWidget;
    taskWidget.show();

    qDebug("Load modules");
    QObject::connect(&qApplication,SIGNAL(lastWindowClosed()),BlockingController::getInstance(),SLOT(unblock()));
    TaskController::getInstance()->checkRecovery();
    return qApplication.exec();
}



