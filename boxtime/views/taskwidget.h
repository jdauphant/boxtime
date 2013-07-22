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
    void move(int x, int y);
    void move(const QPoint & qpoint);

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
    void restorePositionFromSettings();
    void openLogFile();

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
