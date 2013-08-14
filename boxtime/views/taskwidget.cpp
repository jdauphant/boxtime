#include "taskwidget.h"
#include "ui_taskwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QtGui>
#include "taskcontroller.h"
#include "storagecontroller.h"
#include "settingscontroller.h"
#include "blockingcontroller.h"

TaskWidget::TaskWidget(QWidget *parent) :
    GenericWidget("taskwidget",DEFAULT_TASKWIDGET_ENABLE,parent),
    ui(new Ui::TaskWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);

    ui->validationButton->setVisible(false);
    setVisibleAllDesktops();
    ui->taskLineEdit->setAttribute(Qt::WA_MacShowFocusRect, 0);

#ifdef Q_OS_MAC
    QFont taskLineEditFont(ui->taskLineEdit->font());
    taskLineEditFont.setPointSize(17);
    ui->taskLineEdit->setFont(taskLineEditFont);

    QFont timeLabelFont(ui->timeLabel->font());
    timeLabelFont.setPointSize(19);
    ui->timeLabel->setFont(timeLabelFont);

    QFont noTaskLabelFont(ui->noTaskLabel->font());
    noTaskLabelFont.setPointSize(17);
    ui->noTaskLabel->setFont(noTaskLabelFont);
#endif

    QObject::connect(ui->taskLineEdit, SIGNAL(returnPressed()),this,SLOT(textValided()));

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));

    movableCharm = new MovableCharm();
    movableCharm->activateOn(this);

    this->setFixedWidth(SettingsController::getInstance()->getValue<int>("taskwidget/width", DEFAULT_TASKWIDGET_WIDTH));
    roundCorners(6);
}

void TaskWidget::load()
{
    StorageController::getInstance();
    TaskController * taskController = TaskController::getInstance();
    connect(this,SIGNAL(newTask(QString)),taskController,SLOT(start(QString)));
    connect(taskController,SIGNAL(newTime(double)),this,SLOT(newTime(double)));
    connect(taskController,SIGNAL(started(Task*)),this,SLOT(taskStarted(Task*)));
    connect(ui->validationButton, SIGNAL(clicked()),taskController,SLOT(end()));
    connect(taskController,SIGNAL(ended(Task*)),this,SLOT(taskEnded()));
    show();
}

void TaskWidget::unload()
{
    TaskController * taskController = TaskController::getInstance();
    disconnect(this,SIGNAL(newTask(QString)),taskController,SLOT(start(QString)));
    disconnect(taskController,SIGNAL(newTime(double)),this,SLOT(newTime(double)));
    disconnect(taskController,SIGNAL(started(Task*)),this,SLOT(taskStarted(Task*)));
    disconnect(ui->validationButton, SIGNAL(clicked()),taskController,SLOT(end()));
    disconnect(taskController,SIGNAL(ended(Task*)),this,SLOT(taskEnded()));
    hide();
}

void TaskWidget::textValided()
{
    if(ui->taskLineEdit->text().size()>0)
    {
        ui->taskLineEdit->setEnabled(false);
        newTask(ui->taskLineEdit->text());
    }
}

void TaskWidget::taskStarted(Task * task)
{
    ui->taskLineEdit->setEnabled(false);
    ui->taskLineEdit->setMaxLength(65);
    ui->taskLineEdit->setText("<"+task->name+"/>");
    ui->taskLineEdit->setCursor(Qt::OpenHandCursor);
    // boxtime color : 3577B1;
    ui->taskLineEdit->setStyleSheet("QLineEdit { background: white; color:#1B4971; border-radius: 8px }");
    ui->validationButton->setVisible(true);
    ui->timeLabel->setText("");
    ui->horizontalSpacer->changeSize(0,0);
    ui->noTaskLabel->setVisible(false);
    QFontMetrics fontMetrics(ui->taskLineEdit->font());
    int newWidth = fontMetrics.width(ui->taskLineEdit->text())+120;
    qDebug() << "Width set to" << newWidth;
    this->setFixedWidth(newWidth);
    roundCorners(6);
}

void TaskWidget::taskEnded()
{
    ui->taskLineEdit->setMaxLength(50);
    ui->taskLineEdit->setStyleSheet("QLineEdit { background: white; color:black; border-radius: 8px }");
    ui->validationButton->setVisible(false);
    ui->taskLineEdit->setText("");
    ui->timeLabel->setText("00s");
    ui->horizontalSpacer->changeSize(5,0);
    ui->taskLineEdit->setCursor(Qt::IBeamCursor);
    ui->taskLineEdit->setEnabled(true);
    ui->noTaskLabel->setVisible(true);
    this->setFixedWidth(SettingsController::getInstance()->getValue<int>("taskwidget/width", DEFAULT_TASKWIDGET_WIDTH));
    roundCorners(6);
}



void TaskWidget::newTime(double time)
{
    QTime qtime = QTime(0,0,0,0).addSecs(time);
    QString timeText;
    if(time<60)
        timeText = qtime.toString("ss's'");
    else if(time<60*60)
        timeText = qtime.toString("mm'm':ss's'");
    else if(time<60*60*24)
        timeText = qtime.toString("HH'h':mm'm'");
    else
        timeText = QString::number(qFloor(time/(60*60*24)))+qtime.toString("'d':HH'h'");
    ui->timeLabel->setText(timeText);
}

TaskWidget::~TaskWidget()
{
    delete ui;
}


void TaskWidget::showContextMenu(const QPoint& pos){
     QPoint globalPos = this->mapToGlobal(pos);
     QMenu *contextMenu=new QMenu;
     QAction * aboutLabel = contextMenu->addAction(QIcon("://ressources/logo_mini.png"), APPLICATION_NAME+QString(" v")+VERSION);
     aboutLabel->setIconVisibleInMenu(true);

#ifndef Q_OS_WIN32
     contextMenu->addSeparator();
     QAction * blockingEnableAction = contextMenu->addAction("Disable blocking");
     connect(blockingEnableAction, SIGNAL(toggled(bool)), this, SLOT(blockingSettingChange(bool)));
     if(true==SettingsController::getInstance()->getValue<bool>("blocking/enable", DEFAULT_BLOCKING_ENABLE))
     {
        blockingEnableAction->setCheckable(true);
        blockingEnableAction->setChecked(true);
     }
     else
     {
         blockingEnableAction->setText("Enable blocking");
         blockingEnableAction->setCheckable(true);
         blockingEnableAction->setChecked(false);
     }
#endif
#ifdef Q_OS_LINUX
     QAction * onStartupEnableAction = contextMenu->addAction("Disable startup launch");
     connect(onStartupEnableAction, SIGNAL(toggled(bool)), this, SLOT(onStartupSettingChange(bool)));
     if(true==SettingsController::getInstance()->getValue<bool>("application/onstartup",DEFAULT_STARTUP_LAUNCH))
     {
        onStartupEnableAction->setCheckable(true);
        onStartupEnableAction->setChecked(true);
     }
     else
     {
         onStartupEnableAction->setText("Enable startup launch");
         onStartupEnableAction->setCheckable(true);
         onStartupEnableAction->setChecked(false);
     }
#endif

     QAction * openLogAction = contextMenu->addAction("Open log file");
     connect(openLogAction, SIGNAL(triggered()), this, SLOT(openLogFile()));

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


void TaskWidget::blockingSettingChange(bool enable)
{
    SettingsController::getInstance()->setValue("blocking/enable", enable);
}

void TaskWidget::onStartupSettingChange(bool enable)
{
    SettingsController::getInstance()->setValue("application/onstartup", enable);
}

void TaskWidget::openLogFile()
{
    QDesktopServices::openUrl(QUrl("file://"+SettingsController::getInstance()->getValue<QString>("log/file", SettingsController::getInstance()->getDataPath()+DEFAULT_LOGFILE),QUrl::TolerantMode));
}


bool TaskWidget::event(QEvent * event)
{
    bool ret = QWidget::event(event);
    if(QEvent::WinIdChange==event->type())
    {
        setVisibleAllDesktops();
        qDebug("WinIdChange event, setVisibleAllDesktops flag readded");
        return ret;
    }
    return ret;
}


TaskWidget *TaskWidget::getInstance()
{
    static TaskWidget * taskWidgetInstance = 0;
    if(!taskWidgetInstance)
    {
        taskWidgetInstance = new TaskWidget();
    }
    return taskWidgetInstance;
}
