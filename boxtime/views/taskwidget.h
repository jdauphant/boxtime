#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QTime>
#include "genericwidget.h"
#include "task.h"

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

private slots:
    void textValided();
    void taskStarted(Task * task);
    void taskEnded();
    void newTime(double time);
    void showContextMenu(const QPoint& pos);
    void proxySettingChange(bool enable);
    void onStartupSettingChange(bool enable);
    void restorePositionFromSettings();

private:
    Ui::TaskWidget *ui;
    void initConnectToTaskController();

protected:
     void mouseMoveEvent(QMouseEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     bool event(QEvent * event);

private:
     QPoint dragPosition;
};

#endif // TASKWIDGET_H
