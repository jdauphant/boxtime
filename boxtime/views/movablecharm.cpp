#include "movablecharm.h"
#include <QtGui>
#include <QtCore>
#include "settingscontroller.h"

MovableCharm::MovableCharm(QWidget *parent) :
    QObject(parent)
{
    connect(QApplication::desktop(),SIGNAL(workAreaResized(int)),this,SLOT(restorePositionFromSettings()));
}

void MovableCharm::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QApplication::setOverrideCursor(Qt::ClosedHandCursor);
        dragPosition = event->globalPos() - charmedObject->frameGeometry().topLeft();
        asMove = false;
        event->accept();
    }
}
void MovableCharm::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint destination = event->globalPos() - dragPosition;
        move(destination);
        asMove = true;
        event->accept();
    }
    else
    {
        QApplication::setOverrideCursor(Qt::OpenHandCursor);
    }
}

void MovableCharm::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        QApplication::restoreOverrideCursor();
        if(asMove==true)
        {
            QPoint position = charmedObject->pos();
            QSize desktopSize = QApplication::desktop()->size();
            SettingsController::getInstance()->setValue("ui/"+QString::number(desktopSize.width())+"x"+QString::number(desktopSize.height())+"_x", position.x());
            SettingsController::getInstance()->setValue("ui/"+QString::number(desktopSize.width())+"x"+QString::number(desktopSize.height())+"_y", position.y());
            qDebug() << "New position save" << position;
        }
        event->accept();
    }
}

void MovableCharm::move(int x, int y)
{
    QSize desktopSize = QApplication::desktop()->size();
    int maxX = desktopSize.width()-charmedObject->width();
    int maxY = desktopSize.height()-charmedObject->height();
    if(x<0)  x = 0;
    else if(x>maxX) x = maxX;
    if(y<0) y = 0;
    else if(y>maxY) y = maxY;
    charmedObject->move(x,y);
}

bool MovableCharm::eventFilter(QObject *dist, QEvent *event)
{
    switch(event->type())
    {
    case QEvent::MouseButtonPress:
        mousePressEvent(static_cast<QMouseEvent*>(event));
        break;
    case QEvent::MouseButtonRelease:
        mouseReleaseEvent(static_cast<QMouseEvent*>(event));
        break;
    case QEvent::MouseMove:
        mouseMoveEvent(static_cast<QMouseEvent*>(event));
        break;
    default:
        break;
    }
    return false;
}

void MovableCharm::move(const QPoint & qpoint)
{
    this->move(qpoint.x(),qpoint.y());
}

void MovableCharm::activateOn(QWidget *widget)
{
    charmedObject = widget;
    charmedObject->installEventFilter(this);
    foreach(QObject * child, charmedObject->children())
        child->installEventFilter(this);
    restorePositionFromSettings();
}

void MovableCharm::deactivateFrom(QWidget *widget)
{
    charmedObject->removeEventFilter(widget);
}

void MovableCharm::restorePositionFromSettings()
{
    QSize desktopSize = QApplication::desktop()->size();
    QString desktopSizeString = QString::number(desktopSize.width())+"x"+QString::number(desktopSize.height());
    int x = SettingsController::getInstance()->getValue<int>("ui/"+desktopSizeString+"_x", (QApplication::desktop()->width() / 2) - (charmedObject->width() / 2));
    int y = SettingsController::getInstance()->getValue<int>("ui/"+desktopSizeString+"_y", 0);
    move(x,y);
    qDebug() << "Position restore for resolution" << desktopSizeString << " to x=" << x << " and y=" << y;
    qDebug() << "Desktop size:" << desktopSizeString << "availablegeometry:" << QApplication::desktop()->availableGeometry()
             << "isVirtualDesktop:" << QApplication::desktop()->isVirtualDesktop() << "screenNumber" << QApplication::desktop()->screenCount();
}
