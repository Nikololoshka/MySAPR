#include "sectionalForceGraphicsItem.h"

SectionalForceGraphicsItem::SectionalForceGraphicsItem()
    : QGraphicsItem(),
      minDrawStep(0),
      rodId_(-1),
      width_(0),
      value_(0),
      isLabelShow(false)
{
}

SectionalForceGraphicsItem::SectionalForceGraphicsItem(int drawStep, int rodId, double value)
    : QGraphicsItem(),
      minDrawStep(drawStep),
      rodId_(rodId),
      width_(drawStep),
      value_(value),
      isLabelShow(false)
{
}

SectionalForceGraphicsItem::SectionalForceGraphicsItem(const SectionalForceGraphicsItem *force)
    : SectionalForceGraphicsItem(force->minDrawStep, force->rodId_, force->value_)
{
}

QRectF SectionalForceGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, width_, minDrawStep);
}

void SectionalForceGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    if (value_ < 0) {
        painter->translate(width_, minDrawStep);
        painter->rotate(180);
    }

    painter->drawLine(QPointF(0, minDrawStep / 2), QPointF(width_, minDrawStep / 2));

    const int count = static_cast<int>(width_ / 10);
    const double step = width_ / count;
    const int angle = 20;
    for (int i = 1; i < count; ++i) {
        painter->drawLine(QPointF(step * i + step / 2, minDrawStep / 2),
                          QPointF(step * i  - step / 2, minDrawStep / 2 + step * qTan((angle * M_PI) / 180)));
        painter->drawLine(QPointF(step * i + step / 2, minDrawStep / 2),
                          QPointF(step * i  - step / 2, minDrawStep / 2 - step * qTan((angle * M_PI) / 180)));
    }

    // label
    if (isLabelShow) {
        if (value_ < 0) {
            painter->translate(width_, minDrawStep);
            painter->rotate(180);
        }
        QRect rect(0, static_cast<int>(minDrawStep * 3 / 4),
                   static_cast<int>(width_ * 0.95), static_cast<int>(minDrawStep / 4 * 0.95));
        auto res = createText(rect, Qt::AlignCenter);
        painter->setFont(res.font);
        painter->drawText(rect, Qt::AlignCenter, res.text);

        painter->setPen(Qt::blue);
        painter->drawRect(res.rect);
    }
}

void SectionalForceGraphicsItem::updateValues(int rodId, double value)
{
    rodId_ = rodId;
    value_ = value;
    update();
}

void SectionalForceGraphicsItem::showLabel(bool show)
{
    isLabelShow = show;
}

int SectionalForceGraphicsItem::rodId() const
{
    return rodId_;
}

void SectionalForceGraphicsItem::setRodId(int rodId)
{
    rodId_ = rodId;
}

void SectionalForceGraphicsItem::setWidth(double width)
{
    width_ = width;
}

void SectionalForceGraphicsItem::setValue(double value)
{
    value_ = value;
}

double SectionalForceGraphicsItem::value() const
{
    return value_;
}

double SectionalForceGraphicsItem::width() const
{
    return width_;
}

SectionalForceGraphicsItem::LabelResult SectionalForceGraphicsItem::createText(const QRect &rect, int flags) const
{
    QFont correctFont = QApplication::font();
    QString text = QString::number(value_, 'g', static_cast<int>(width_ / 10)) + " Н/м";

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
