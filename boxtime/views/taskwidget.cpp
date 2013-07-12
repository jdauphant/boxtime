#include "taskwidget.h"
#include "ui_taskwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QtGui>
#include "taskcontroller.h"
#include "settingscontroller.h"
#include "proxycontroller.h"
#include "storagecontroller.h"

using namespace settings;

TaskWidget::TaskWidget(QWidget *parent) :
    GenericWidget(parent),
    ui(new Ui::TaskWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint );

    int x = SettingsController::getInstance()->getValue<int>("taskwidget/x", (QApplication::desktop()->width() / 2) - (width() / 2));
    int y = SettingsController::getInstance()->getValue<int>("taskwidget/y", 0);
    move(x,y);

    ui->validationButton->setVisible(false);
    setVisibleAllDesktops();
    roundCorners(6);
    ui->taskLineEdit->setAttribute(Qt::WA_MacShowFocusRect, 0);

#ifdef Q_OS_MAC
    QFont taskLineEditFont(ui->taskLineEdit->font());
    taskLineEditFont.setPointSize(18);
    ui->taskLineEdit->setFont(taskLineEditFont);

    QFont timeLabelFont(ui->timeLabel->font());
    timeLabelFont.setPointSize(18);
    ui->timeLabel->setFont(timeLabelFont);
#endif

    QObject::connect(ui->taskLineEdit, SIGNAL(returnPressed()),this,SLOT(textValided()));
    QObject::connect(ui->validationButton, SIGNAL(clicked()),this,SLOT(doneClicked()));
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));

    initConnectToTaskController();
    StorageController::getInstance();
}

void TaskWidget::initConnectToTaskController()
{
    TaskController * taskController = TaskController::getInstance();
    connect(this,SIGNAL(newTask(QString)),taskController,SLOT(start(QString)));
    connect(this,SIGNAL(done()),taskController,SLOT(end()));
    connect(taskController,SIGNAL(newTime(double)),this,SLOT(newTime(double)));
    connect(this,SIGNAL(proxySettingChange(bool)),ProxyController::getInstance(),SLOT(enable(bool)));
    connect(taskController,SIGNAL(started(Task*)),this,SLOT(newTaskStarted(Task*)));
}

void TaskWidget::textValided()
{
    if(ui->taskLineEdit->text().size()>0)
    {
        ui->taskLineEdit->setEnabled(false);
        newTask(ui->taskLineEdit->text());
    }
}

void TaskWidget::newTaskStarted(Task * task)
{
    ui->taskLineEdit->setEnabled(false);
    ui->taskLineEdit->setText("<"+task->name+"/>");
    ui->taskLineEdit->setCursor(Qt::OpenHandCursor);
    // boxtime color : 3577B1;
    ui->taskLineEdit->setStyleSheet("QLineEdit { background: white; color:#1B4971; border-radius: 8px }");
    ui->taskLineEdit->setMaxLength(65);
    ui->validationButton->setVisible(true);
    ui->timeLabel->setText("");
}

void TaskWidget::doneClicked()
{
    ui->taskLineEdit->setMaxLength(60);
    ui->taskLineEdit->setStyleSheet("QLineEdit { background: white; color:black; border-radius: 8px }");
    ui->validationButton->setVisible(false);
    ui->taskLineEdit->setText("");
    done();
    ui->timeLabel->setText("00s  ");
    ui->taskLineEdit->setCursor(Qt::IBeamCursor);
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
        QApplication::setOverrideCursor(Qt::ClosedHandCursor);
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
void TaskWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint destination = event->globalPos() - dragPosition;
        move(destination);
        SettingsController::getInstance()->setValue("taskwidget/x", destination.x());
        SettingsController::getInstance()->setValue("taskwidget/y", destination.y());
        event->accept();
    }
}

void TaskWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        QApplication::restoreOverrideCursor();
        event->accept();
    }
}

void TaskWidget::showContextMenu(const QPoint& pos){
     QPoint globalPos = this->mapToGlobal(pos);
     QMenu *contextMenu=new QMenu;
     QAction * boxtimeLabel = contextMenu->addAction(QIcon("://ressources/logo_mini.png"), "Boxtime v1.0");
     boxtimeLabel->setIconVisibleInMenu(true);

#ifndef Q_OS_WIN32
     contextMenu->addSeparator();
     if(false==SettingsController::getInstance()->getValue<bool>("proxy/enable", DEFAULT_PROXY_ENABLE))
     {
        QAction * proxyEnableAction = contextMenu->addAction("Blocking (disable)");
        proxyEnableAction->setCheckable(true);
        connect(proxyEnableAction, SIGNAL(toggled(bool)), this, SIGNAL(proxySettingChange(bool)));
     }
#endif
     if(true==StorageController::getInstance()->historyExists())
     {
         QAction * exportAndOpenCSVAction = contextMenu->addAction("Export to spreadsheet");
         connect(exportAndOpenCSVAction, SIGNAL(triggered()), StorageController::getInstance(), SLOT(exportAndOpenCSV()));
     }

     contextMenu->addSeparator();
     QAction * exit = contextMenu->addAction("Exit");
     connect(exit, SIGNAL(triggered()),
             this, SLOT(close()));
     contextMenu->exec(globalPos);
}
