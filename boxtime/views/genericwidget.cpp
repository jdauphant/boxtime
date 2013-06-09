#include "genericwidget.h"

#ifdef Q_WS_X11 //only define on 4.X Q_OS_UNIX
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

GenericWidget::GenericWidget(QWidget *parent) :
    QWidget(parent)
{

}

void GenericWidget::roundCorners(int r)
{
    this->setMask(roundedCorners(this->rect(), r));
}

QRegion GenericWidget::roundedCorners(const QRect& rect, int r)
{
    QRegion region;
    // middle and borders
    region += rect.adjusted(r, 0, -r, 0);
    region += rect.adjusted(0, r, 0, -r);
    // top left
    QRect corner(rect.topLeft(), QSize(r*2, r*2));
    region += QRegion(corner, QRegion::Ellipse);
    // top right
    corner.moveTopRight(rect.topRight());
    region += QRegion(corner, QRegion::Ellipse);
    // bottom left
    corner.moveBottomLeft(rect.bottomLeft());
    region += QRegion(corner, QRegion::Ellipse);
    // bottom right
    corner.moveBottomRight(rect.bottomRight());
    region += QRegion(corner, QRegion::Ellipse);
    return region;
}

/* Only X11 implementation */
void GenericWidget::setVisibleAllDesktops()
{
#ifdef Q_WS_X11 //only define on 4.X Q_OS_UNIX
    unsigned long data = 0xFFFFFFFF;
    XChangeProperty (QX11Info::display(),
                     winId(),
                     XInternAtom(QX11Info::display(), "_NET_WM_DESKTOP", False),
                     XA_CARDINAL,
                     32,
                     PropModeReplace,
                     reinterpret_cast<unsigned char *>(&data), // all desktop
                     1);
    // more information http://standards.freedesktop.org/wm-spec/1.4/ar01s05.html
#endif
}
