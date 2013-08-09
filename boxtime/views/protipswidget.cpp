#include "protipswidget.h"
#include "ui_protipswidget.h"
#include "taskwidget.h"
#include "ui_taskwidget.h"

ProTipsWidget::ProTipsWidget(QWidget *parent) :
    GenericWidget(parent),
    ui(new Ui::ProTipsWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint |  Qt::Tool | Qt::FramelessWindowHint);

    roundCorners(30);
    setVisibleAllDesktops();
    TaskWidget * taskWidget = TaskWidget::getInstance();
    taskWidget->ui->taskLineEdit->installEventFilter(this);
}

ProTipsWidget::~ProTipsWidget()
{
    delete ui;
}

void ProTipsWidget::show()
{
     GenericWidget::show();
     QPoint taskWidgetPosition = TaskWidget::getInstance()->pos();
     taskWidgetPosition.setY(taskWidgetPosition.y()+TaskWidget::getInstance()->height());
     move(taskWidgetPosition);
     qDebug() << "Show protips";
}

bool ProTipsWidget::eventFilter(QObject *, QEvent *event)
{
    switch(event->type())
    {
    case QEvent::FocusIn:
        show();
        break;
    case QEvent::FocusOut:
        hide();
        break;
    default:
        break;
    }
    return false;
}

