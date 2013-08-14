#ifndef GENERICWIDGET_H
#define GENERICWIDGET_H

#include <QWidget>
#include "genericviewmodule.h"

class GenericWidget : public GenericViewModule
{
    Q_OBJECT
public:
    explicit GenericWidget(QString name, bool defaultEnable, QWidget *parent = 0);
    void roundCorners(int r);
    void setVisibleAllDesktops();

private:
    static QRegion roundedCorners(const QRect& rect, int r);
    
};

#endif // GENERICWIDGET_H
