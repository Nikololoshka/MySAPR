#include "rodNumberGraphicsItem.h"

RodNumberGraphicsItem::RodNumberGraphicsItem()
    : QGraphicsItem(),
      size_(0),
      number_(0)
{
}

QRectF RodNumberGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, size_, size_);
}

void RodNumberGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setBrush(Qt::white);
    painter->drawEllipse(boundingRect());
    painter->drawText(boundingRect(), Qt::AlignCenter , QString::number(number_));
}

void RodNumberGraphicsItem::setSize(double size)
{
    size_ = size;
}

void RodNumberGraphicsItem::setNumber(int number)
{
    number_ = number;
}

double RodNumberGraphicsItem::size() const
{
    return size_;
}
