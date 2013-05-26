#include "taskWidget.h"
#include "ui_taskWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QtGui>

TaskWidget::TaskWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint );
    move((QApplication::desktop()->width() / 2) - (width() / 2), 0);
    ui->validationButton->setVisible(false);
    QObject::connect(ui->taskLineEdit, SIGNAL(returnPressed()),this,SLOT(newTask()));
    QObject::connect(ui->validationButton, SIGNAL(clicked()),this,SLOT(doneClicked()));
    this->setWindowOpacity(0.80);
}

void TaskWidget::newTask()
{
    if(ui->taskLineEdit->text().size()>0)
    {
        ui->taskLineEdit->setEnabled(false); // boxtime color : 3577B1;
        ui->taskLineEdit->setStyleSheet("QLineEdit{background: white; color:#1B4971; font-weight:bold; font-size:15px;}");
        newTask(ui->taskLineEdit->text());
        ui->taskLineEdit->setMaxLength(55);
        ui->taskLineEdit->setText("<"+ui->taskLineEdit->text()+"/>");
        ui->validationButton->setVisible(true);
    }
}

void TaskWidget::doneClicked()
{
    ui->taskLineEdit->setMaxLength(50);
    ui->taskLineEdit->setStyleSheet("QLineEdit{background: white; color:black; font-weight:bold; font-size:15px;}");
    ui->validationButton->setVisible(false);
    ui->taskLineEdit->setText("");
    done();
    ui->timeLabel->setText("00s  ");
    ui->taskLineEdit->setEnabled(true);
}

void TaskWidget::newTime(double time)
{
    QTime qtime = QTime(0,0,0,0).addSecs(time);
    QString timeText;
    if(time<60)
        timeText = qtime.toString("ss's'");
    else if(time<60*60)
        timeText = qtime.toString("mm'm':ss's'");
    else
        timeText = qtime.toString("HH'h':mm'm'");
    ui->timeLabel->setText(timeText);
}

TaskWidget::~TaskWidget()
{
    delete ui;
}

/* drag windows */
void TaskWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
void TaskWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}
