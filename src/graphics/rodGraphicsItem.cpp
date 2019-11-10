#include "rodGraphicsItem.h"

RodGraphicsItem::RodGraphicsItem()
    : QGraphicsItem(),
      minDrawStep(0),
      length_(1),
      width_(0),
      height_(0),
      sectionalArea_(1)
{
}

RodGraphicsItem::RodGraphicsItem(const RodGraphicsItem *rodItem)
    : RodGraphicsItem(rodItem->minDrawStep, rodItem->length_, rodItem->sectionalArea_)
{
}

void RodGraphicsItem::updateValues(double length, double sectionalArea)
{
    this->length_ = length;
    this->sectionalArea_ = sectionalArea;
    update();
}

RodGraphicsItem::RodGraphicsItem(int drawStep, double length, double sectionalArea)
    : QGraphicsItem(),
      minDrawStep(drawStep),
      length_(length),
      width_(drawStep),
      height_(drawStep),
      sectionalArea_(sectionalArea)
{
}

double RodGraphicsItem::height() const
{
    return height_;
}

double RodGraphicsItem::width() const
{
    return width_;
}

QRectF RodGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, width(), height());
}

void RodGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
    painter->drawRect(boundingRect());
}

void RodGraphicsItem::setDrawStep(int drawStep)
{
    this->minDrawStep = drawStep;
}

double RodGraphicsItem::length() const
{
    return length_;
}

void RodGraphicsItem::setLength(double length)
{
    length_ = length;
}

double RodGraphicsItem::sectionalArea() const
{
    return sectionalArea_;
}

void RodGraphicsItem::setSectionalArea(double sectionalArea)
{
    sectionalArea_ = sectionalArea;
}

void RodGraphicsItem::setHeight(double height)
{
    height_ = height;
}

void RodGraphicsItem::setWidth(double width)
{
    width_ = width;
}
