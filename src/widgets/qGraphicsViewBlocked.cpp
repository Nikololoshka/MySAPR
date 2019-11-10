#include "qGraphicsViewBlocked.h"

QGraphicsViewBlocked::QGraphicsViewBlocked(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
}

void QGraphicsViewBlocked::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

