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
    const int DEFAULT_TASKWIDGET_WIDTH = 300;
    const bool DEFAULT_TASKWIDGET_ENABLE = true;
    const bool DEFAULT_TASKWIDGET_INACTIVITYTIMER = false;
}

using namespace settings;

class TaskWidget : public GenericWidget
{
    Q_OBJECT
    
public:
    static TaskWidget * getInstance();
    ~TaskWidget();
    Ui::TaskWidget *ui;

private:
    explicit TaskWidget(QWidget *parent = 0);
    TaskWidget(const TaskWidget & c): GenericWidget("taskwidget", DEFAULT_TASKWIDGET_ENABLE, c.parentWidget()) {}
    TaskWidget & operator=(const TaskWidget &) {return *this;}

signals:
    void newTask(QString taskName);

public slots:
    void load();
    void unload();

private slots:
    void textValided();
    void taskStarted(Task * task);
    void taskEnded();
    void newTime(double time);
    void showContextMenu(const QPoint& pos);
    void blockingSettingChange(bool enable);
    void onStartupSettingChange(bool enable);
    void inactivityTimerChange(bool enable);
    void protipsChange(bool enable);
    void openLogFile();

private:
    bool event(QEvent * event);
    MovableCharm * movableCharm;
};

#endif // TASKWIDGET_H
