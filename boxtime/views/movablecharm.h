#ifndef MOVABLECHARM_H
#define MOVABLECHARM_H

#include <QWidget>

class MovableCharm : public QObject
{
    Q_OBJECT
public:
    explicit MovableCharm(QWidget *parent = 0);
    bool eventFilter( QObject *dist, QEvent *event );
    void activateOn(QWidget *widget);
    void deactivateFrom(QWidget *widget);

protected:
     void mouseMoveEvent(QMouseEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void move(int x, int y);
     void move(const QPoint & qpoint);

public slots:
     void restorePositionFromSettings();

private:
     QPoint dragPosition;
     bool asMove;
     QWidget * charmedObject;
};

#endif // MOVABLECHARM_H
