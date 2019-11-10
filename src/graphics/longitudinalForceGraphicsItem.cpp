#include "longitudinalForceGraphicsItem.h"

LongitudinalForceGraphicsItem::LongitudinalForceGraphicsItem()
    : QGraphicsItem(),
      minDrawStep(0),
      nodeId_(-1),
      value_(0),
      isLabelShow(false)
{
    createHead();
}

LongitudinalForceGraphicsItem::LongitudinalForceGraphicsItem(int drawStep, int nodeId,
                                                             double value)
    : QGraphicsItem(),
      minDrawStep(drawStep),
      nodeId_(nodeId),
      value_(value),
      isLabelShow(false)
{
    createHead();
}

LongitudinalForceGraphicsItem::LongitudinalForceGraphicsItem(const LongitudinalForceGraphicsItem *force)
    : LongitudinalForceGraphicsItem(force->minDrawStep, force->nodeId_, force->value_)
{
}

QRectF LongitudinalForceGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, minDrawStep, minDrawStep);
}

void LongitudinalForceGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    if (value_ > 0) {
        painter->translate(minDrawStep, minDrawStep);
        painter->rotate(180);
    }

    painter->setBrush(Qt::black);
    painter->drawPolygon(arrowHead);
    painter->setPen(QPen(Qt::black, 2));
    painter->drawLine(QPointF(minDrawStep / 3, minDrawStep / 2),
                      QPointF(minDrawStep - 1, minDrawStep / 2));

    // label
    if (isLabelShow) {
        if (value_ > 0) {
            painter->translate(minDrawStep, minDrawStep);
            painter->rotate(180);
        }

        QRect rect(0, 0, static_cast<int>(minDrawStep * 3 / 4 * 0.95), static_cast<int>(minDrawStep / 4 * 0.95));
        auto res = createText(rect, Qt::AlignCenter);
        painter->setFont(res.font);
        painter->setPen(Qt::black);
        painter->setBrush(Qt::NoBrush);
        painter->drawText((minDrawStep - res.rect.width()) / 2, 0,
                          res.rect.width(), res.rect.height(), Qt::AlignCenter, res.text);

        painter->setPen(Qt::darkYellow);
        painter->drawRect((minDrawStep - res.rect.width()) / 2, 0,
                          res.rect.width(), res.rect.height());
    }
}

void LongitudinalForceGraphicsItem::setDrawStep(int drawStep)
{
    this->minDrawStep = drawStep;
}

void LongitudinalForceGraphicsItem::updateValues(int nodeId, double value)
{
    nodeId_ = nodeId;
    value_ = value;
    update();
}

void LongitudinalForceGraphicsItem::showLabel(bool show)
{
    isLabelShow = show;
}

void LongitudinalForceGraphicsItem::createHead()
{
    double angle = 20;
    double width = minDrawStep / 3;
    double height = minDrawStep / 2;

    QPointF up(width, width * qTan((angle * M_PI) / 180) + height);
    QPointF down(width, - width * qTan((angle * M_PI) / 180) + height);

    arrowHead.clear();
    arrowHead << QPointF(0, height) << up << down;
}

double LongitudinalForceGraphicsItem::value() const
{
    return value_;
}

void LongitudinalForceGraphicsItem::setValue(double value)
{
    value_ = value;
}

int LongitudinalForceGraphicsItem::nodeId() const
{
    return nodeId_;
}

void LongitudinalForceGraphicsItem::setNodeId(int nodeId)
{
    nodeId_ = nodeId;
}

LongitudinalForceGraphicsItem::LabelResult LongitudinalForceGraphicsItem::createText(const QRect &rect, int flags) const
{
    QFont correctFont = QApplication::font();
    QString text = QString::number(value_, 'g', static_cast<int>(minDrawStep / 10)) + " Н";

    QRect tempRect;
    for (int i = 1; i < 100; ++i) {
        correctFont.setPixelSize(i);
        tempRect = QFontMetrics(correctFont).boundingRect(rect, flags, text);
        if (tempRect.height() > rect.height() || tempRect.width() > rect.width()) {
            correctFont.setPixelSize(i - 1);
            return {correctFont, text, tempRect};
        }
    }
    return {correctFont, text, tempRect};
}
