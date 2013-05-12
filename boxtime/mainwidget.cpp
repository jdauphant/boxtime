#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QtGui>

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
    this->setWindowOpacity(0.85);
}

void MainWidget::newTask()
{
    if(ui->lineEdit->text().size()>0)
    {
        ui->lineEdit->setEnabled(false); // 3577B1;
        ui->lineEdit->setStyleSheet("QLineEdit{background: white; color:#1B4971; font-weight:bold; font-size:15px;}");
        newTask(ui->lineEdit->text());
        ui->pushButton->setVisible(true);
    }
}

void MainWidget::doneClicked()
{
    ui->lineEdit->setStyleSheet("QLineEdit{background: white; color:black; font-weight:normal; font-size:15px;}");
    ui->pushButton->setVisible(false);
    ui->lineEdit->setText("");
    done();
    ui->label->setText("00:00:00");
    ui->lineEdit->setEnabled(true);
}

void MainWidget::newTime(double time)
{
    qtime = QTime();
    qtime = qtime.addSecs(time);
    ui->label->setText(qtime.toString("hh:mm:ss"));
}

MainWidget::~MainWidget()
{
    delete ui;
}

/* drag windows */
void MainWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}
