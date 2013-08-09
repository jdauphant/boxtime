#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QTime>
#include "genericwidget.h"
#include "task.h"
#include "movablecharm.h"

namespace Ui {
class TaskWidget;
}

namespace settings {
    const int DEFAULT_TASK_WIDGET_WIDTH = 600;
}

class TaskWidget : public GenericWidget
{
    Q_OBJECT
    
public:
    static TaskWidget * getInstance();
    ~TaskWidget();
    Ui::TaskWidget *ui;

private:
    explicit TaskWidget(QWidget *parent = 0);
    TaskWidget(const TaskWidget & c): GenericWidget(c.parentWidget()) {}
    TaskWidget & operator=(const TaskWidget &) {return *this;}

signals:
    void newTask(QString taskName);

private slots:
    void textValided();
    void taskStarted(Task * task);
    void taskEnded();
    void newTime(double time);
    void showContextMenu(const QPoint& pos);
    void blockingSettingChange(bool enable);
    void onStartupSettingChange(bool enable);
    void openLogFile();

private:
    void initConnectToTaskController();
    bool event(QEvent * event);
    MovableCharm * movableCharm;
};

#endif // TASKWIDGET_H
