#ifndef GENERICWIDGET_H
#define GENERICWIDGET_H

#include <QWidget>

class GenericWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GenericWidget(QWidget *parent = 0);
    void roundCorners(int r);
    void setVisibleAllDesktops();

private:
    static QRegion roundedCorners(const QRect& rect, int r);
    
};

#endif // GENERICWIDGET_H
