#ifndef PROTIPSWIDGET_H
#define PROTIPSWIDGET_H

#include <QtGui>
#include "genericwidget.h"
#include "taskwidget.h"

namespace Ui {
class ProTipsWidget;
}


namespace settings
{
    const bool DEFAULT_PROTIPS_ENABLE = true;
}

class ProTipsWidget : public GenericWidget
{
    Q_OBJECT
    
public:
    explicit ProTipsWidget(QWidget *parent = 0);
    ~ProTipsWidget();

public slots:
    void show();
    void putback();
    void load();
    void unload();
    
private:
    Ui::ProTipsWidget *ui;
    bool eventFilter(QObject *, QEvent *event );
    TaskWidget * taskWidget;

};

#endif // PROTIPSWIDGET_H
