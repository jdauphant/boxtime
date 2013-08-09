#ifndef PROTIPSWIDGET_H
#define PROTIPSWIDGET_H

#include <QWidget>
#include "genericwidget.h"

namespace Ui {
class ProTipsWidget;
}

class ProTipsWidget : public GenericWidget
{
    Q_OBJECT
    
public:
    explicit ProTipsWidget(QWidget *parent = 0);
    ~ProTipsWidget();

public slots:
    void show();
    
private:
    Ui::ProTipsWidget *ui;
    bool eventFilter(QObject *, QEvent *event );
};

#endif // PROTIPSWIDGET_H
