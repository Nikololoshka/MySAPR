#include "pillarGraphicsItem.h"

PillarGraphicsItem::PillarGraphicsItem()
    : QGraphicsItem(),
      minDrawStep(0),
      extra_(0),
      height_(0),
      direction(Direction::None)
{
}

PillarGraphicsItem::PillarGraphicsItem(int drawStep, PillarGraphicsItem::Direction direction)
    : QGraphicsItem(),
      minDrawStep(drawStep),
      extra_(drawStep / 12),
      height_(drawStep),
      direction(direction)
{

}


QRectF PillarGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, minDrawStep, height_ + 2 * extra_);
}

void PillarGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    if (direction == Direction::Left) {
        painter->translate(minDrawStep, height_ + 2 * extra_);
        painter->rotate(180);
    }

    painter->setBrush(Qt::black);

    const int offset = 3;
    painter->drawRect(QRectF(0, 0, offset, height_ + 2 * extra_));

    const double step = minDrawStep / 10;

    int i = 1;
    double width = step;
    while (width < (height_ + extra_ / 2)) {
        painter->drawLine(QPointF(offset, extra_ + (i - 1) * step),
                          QPointF(offset + step * 2, extra_ + step + i * step));
        width += step;
        ++i;
    }
}

int PillarGraphicsItem::extra() const
{
    return extra_;
}

void PillarGraphicsItem::setHeight(double height)
{
    height_ = height;
    extra_ = static_cast<int>(height / 12);
    if (extra_ > minDrawStep / 4)
        extra_ = minDrawStep / 4;

    update();
}

double PillarGraphicsItem::height() const
{
    return height_;
}
