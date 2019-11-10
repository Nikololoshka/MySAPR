#include "rodLengthGraphicsItem.h"

RodLengthGraphicsItem::RodLengthGraphicsItem()
    : QGraphicsItem(),
      width_(0),
      height_(0),
      value_(0)
{
}

QRectF RodLengthGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, width_, height_);
}

void RodLengthGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->drawLine(QPointF(0, height_ * 3 / 4),
                      QPointF(width_, height_ * 3 / 4));

    const double arrowSize = width_ / (width_ / 5);
    const int angle = 20;

    // начало
    painter->drawLine(QPointF(0, height_ * 3 / 4),
                      QPointF(arrowSize, height_ * 3 / 4 + arrowSize * qTan((angle * M_PI) / 180)));
    painter->drawLine(QPointF(0, height_ * 3 / 4),
                      QPointF(arrowSize, height_ * 3 / 4 - arrowSize * qTan((angle * M_PI) / 180)));

    // конец
    painter->drawLine(QPointF(width_, height_ * 3 / 4),
                      QPointF(width_ - arrowSize, height_ * 3 / 4 + arrowSize * qTan((angle * M_PI) / 180)));
    painter->drawLine(QPointF(width_, height_ * 3 / 4),
                      QPointF(width_ - arrowSize, height_ * 3 / 4 - arrowSize * qTan((angle * M_PI) / 180)));

    // отметка
    QRect rect(0, static_cast<int>(height_ * 2 / 7),
               static_cast<int>(width_ * 0.95), static_cast<int>(height_  * 2 / 5));
    auto res = createText(rect, Qt::AlignCenter);
    painter->setFont(res.font);
    painter->drawText(rect, Qt::AlignCenter, res.text);
}

double RodLengthGraphicsItem::width() const
{
    return width_;
}

void RodLengthGraphicsItem::setWidth(double width)
{
    width_ = width;
}

double RodLengthGraphicsItem::height() const
{
    return height_;
}

void RodLengthGraphicsItem::setHeight(double height)
{
    height_ = height;
}

double RodLengthGraphicsItem::value() const
{
    return value_;
}

void RodLengthGraphicsItem::setValue(double value)
{
    value_ = value;
}

RodLengthGraphicsItem::LabelResult RodLengthGraphicsItem::createText(const QRect &rect, int flags) const
{
    QFont correctFont = QApplication::font();
    QString text = QString::number(value_, 'g', static_cast<int>(width_ / 10)) + " м";

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
