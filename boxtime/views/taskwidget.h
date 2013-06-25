#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QTime>
#include "genericwidget.h"

namespace Ui {
class TaskWidget;
}

class TaskWidget : public GenericWidget
{
    Q_OBJECT
    
public:
    explicit TaskWidget(QWidget *parent = 0);
    ~TaskWidget();


signals:
    void newTask(QString taskName);
    void done();
    void proxySettingChange(bool enable);

private slots:
    void newTask();
    void doneClicked();
    void newTime(double time);
    void showContextMenu(const QPoint& pos);
    
private:
    Ui::TaskWidget *ui;

protected:
     void mouseMoveEvent(QMouseEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);

private:
     QPoint dragPosition;
};

#endif // TASKWIDGET_H
