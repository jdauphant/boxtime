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
}

MainWidget::~MainWidget()
{
    delete ui;
}
