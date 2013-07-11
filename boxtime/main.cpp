#include <QtCore>
#include <QApplication>
#include <QDesktopWidget>
#include <QFontDatabase>

#include "taskwidget.h"

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);
    if(-1 == QFontDatabase::addApplicationFont("://ressources/Nexa Light.otf"))
    {
        qDebug() << "Impossible to install font Nexa Light.otf";
    }
    if(-1 == QFontDatabase::addApplicationFont("://ressources/Nexa Bold.otf"))
    {
        qDebug() << "Impossible to install font Nexa Bold.otf";
    }

    TaskWidget taskWidget;
    taskWidget.show();

    return qApplication.exec();
}



