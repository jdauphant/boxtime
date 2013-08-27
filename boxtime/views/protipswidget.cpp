#include "protipswidget.h"
#include "ui_protipswidget.h"
#include "ui_taskwidget.h"

using namespace settings;

ProTipsWidget::ProTipsWidget(QWidget *parent) :
    GenericWidget("protips", DEFAULT_PROTIPS_ENABLE,parent),
    ui(new Ui::ProTipsWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint |  Qt::Tool | Qt::FramelessWindowHint);

    roundCorners(30);
    setVisibleAllDesktops();
    taskWidget = TaskWidget::getInstance();
}

ProTipsWidget::~ProTipsWidget()
{
    delete ui;
}

void ProTipsWidget::show()
{
    if(isHidden())
    {
         GenericWidget::show();
         putback();
         setVisibleAllDesktops();
    }
}

void ProTipsWidget::hide()
{
    if(isVisible())
    {
         GenericWidget::hide();
    }
}

void ProTipsWidget::putback()
{
    if(false==isHidden())
    {
        QPoint taskWidgetPosition = taskWidget->pos();

        QPoint proTipsWidgetPosition = taskWidgetPosition;

        int newY = taskWidgetPosition.y()+taskWidget->height();
        int appHeight = taskWidget->height()+height();
        const QRect desktopAvailableGeometry = QApplication::desktop()->availableGeometry(proTipsWidgetPosition);
        int maxY = desktopAvailableGeometry.height()+desktopAvailableGeometry.y()-appHeight;
        if(newY > maxY)
           newY = proTipsWidgetPosition.y() - height();

        proTipsWidgetPosition.setY(newY);
        move(proTipsWidgetPosition);
        qDebug() << "Show protips position=" << proTipsWidgetPosition << "maxY=" << maxY << "desktopAvailableGeometry=" << desktopAvailableGeometry;
    }
}

void ProTipsWidget::load()
{
    taskWidget->ui->taskLineEdit->installEventFilter(this);
    taskWidget->installEventFilter(this);
}

void ProTipsWidget::unload()
{
    taskWidget->removeEventFilter(this);
    taskWidget->ui->taskLineEdit->removeEventFilter(this);
    hide();
}

bool ProTipsWidget::eventFilter(QObject * sender, QEvent *event)
{
    if(sender==taskWidget)
    {
        switch(event->type())
        {
        case QEvent::Move:
            putback();
            break;
        default:
            break;
        }
    }
    else if(sender==taskWidget->ui->taskLineEdit)
    {
        switch(event->type())
        {
        case QEvent::FocusIn:
            show();
            return true;
            break;
        case QEvent::FocusOut:
            hide();
            return true;
            break;
        default:
            break;
        }
    }
    return false;
}

