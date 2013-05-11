#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTime>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();


signals:
    void newTask(QString taskName);
    void done();

private slots:
    void newTask();
    void doneClicked();
    void newTime(double time);
    
private:
    Ui::MainWidget *ui;
    QTime qtime;
};

#endif // MAINWIDGET_H
