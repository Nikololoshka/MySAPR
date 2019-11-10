#include "nodeNumberGraphicsItem.h"

NodeNumberGraphicsItem::NodeNumberGraphicsItem()
    : QGraphicsItem(),
      size_(0),
      number_(0)
{
}

QRectF NodeNumberGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, size_, size_);
}

void NodeNumberGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setBrush(Qt::white);
    painter->drawRect(boundingRect());
    painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(number_));
}

void NodeNumberGraphicsItem::setSize(double size)
{
    size_ = size;
}

void NodeNumberGraphicsItem::setNumber(int number)
{
    number_ = number;
}

double NodeNumberGraphicsItem::size() const
{
    return size_;
}
