#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QApplication>
#include <QDesktopWidget>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);
    move((QApplication::desktop()->width() / 2) - (width() / 2), 0);
    ui->pushButton->setVisible(false);
    QObject::connect(ui->lineEdit, SIGNAL(returnPressed()),this,SLOT(newTask()));
    QObject::connect(ui->pushButton, SIGNAL(clicked()),this,SLOT(doneClicked()));
}

void MainWidget::newTask()
{
    if(ui->lineEdit->text().size()>0)
    {
        ui->lineEdit->setEnabled(false);
        newTask(ui->lineEdit->text());
        ui->pushButton->setVisible(true);
    }
}

void MainWidget::doneClicked()
{
    ui->pushButton->setVisible(false);
    ui->lineEdit->setText("");
    done();
    ui->label->setText("00:00:00");
    ui->lineEdit->setEnabled(true);
}

void MainWidget::newTime(double time)
{
    qtime = QTime(0,0,time,0);
    ui->label->setText(qtime.toString("hh:mm:ss"));
}

MainWidget::~MainWidget()
{
    delete ui;
}
