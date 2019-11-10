#ifndef QGRAPHICSVIEWBLOCKED_H
#define QGRAPHICSVIEWBLOCKED_H

#include <QtWidgets>

/*!
  QGraphicsView с запретом на перемещение с помощью курсора.
*/
class QGraphicsViewBlocked : public QGraphicsView
{
public:
    explicit QGraphicsViewBlocked(QGraphicsScene *scene, QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // QGRAPHICSVIEWBLOCKED_H
