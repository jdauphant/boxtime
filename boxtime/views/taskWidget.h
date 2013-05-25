#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QTime>

namespace Ui {
class TaskWidget;
}

class TaskWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TaskWidget(QWidget *parent = 0);
    ~TaskWidget();


signals:
    void newTask(QString taskName);
    void done();

private slots:
    void newTask();
    void doneClicked();
    void newTime(double time);
    
private:
    Ui::TaskWidget *ui;

protected:
     void mouseMoveEvent(QMouseEvent *event);
     void mousePressEvent(QMouseEvent *event);

private:
     QPoint dragPosition;
};

#endif // TASKWIDGET_H
