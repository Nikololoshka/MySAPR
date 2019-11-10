#include "plotValuesGraphicsItem.h"

PlotValuesGraphicsItem::PlotValuesGraphicsItem(double drawStep)
    : QGraphicsItem(),
      minDrawStep(drawStep),
      width_(0),
      height_(0)
{
}

void PlotValuesGraphicsItem::setValues(QSharedPointer<Processor> processor, CalcMethod method)
{
    const int VALUE_COUNT = 40;
    parts_.clear();
    maxValue_ = 1e-11;

    for (int i = 0; i < processor->parts(); ++i) {
        parts_.append(Part());
        const double STEP = processor->partLength(i) / VALUE_COUNT;

        for (int j = 0; j <= VALUE_COUNT; ++j) {
            double value = ((*processor).*method)(i, j * STEP);
            parts_[i].values.append(value);

            if (j == 0) {
                parts_[i].left = value;
            }

            if (j == VALUE_COUNT) {
                parts_[i].right = value;
            }

            if (qAbs(value) > maxValue_) {
                maxValue_ = qAbs(value);
            }
        }
    }
}

QRectF PlotValuesGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, width_, height_);
}

void PlotValuesGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    const double MAX_HEIGHT = height_ - minDrawStep;
    double x = minDrawStep;
    double y = height_ / 2;

    painter->setBrush(Qt::BDiagPattern);
    painter->setRenderHint(QPainter::Antialiasing, true);

    // оси
    painter->drawLine(QPointF(0, y), QPointF(width_, y));

    for (int i = 0; i < parts_.size(); ++i) {
        // для каждой части конструкции
        double step = partLenghts_[i] / (parts_[i].values.size() - 1);

        QPolygonF polygon;
        polygon << QPointF(x, y);
        for (int j = 0; j < parts_[i].values.size(); ++j) {
            // '-' в высоте, т.к. ось Y направлена вниз
            polygon << QPointF(x + j * step,
                               y - (parts_[i].values[j] * (MAX_HEIGHT / 2) / maxValue_));
        }
        polygon << QPointF(x + partLenghts_[i], y);

        painter->drawPolygon(polygon);
        x += partLenghts_[i];
    }

    // отметка
    auto res = createText(QRect(static_cast<int>(minDrawStep / 4), static_cast<int>(y - minDrawStep / 2),
                                static_cast<int>(minDrawStep / 2), static_cast<int>(minDrawStep / 2)),
                          Qt::AlignCenter);
    painter->setFont(res.font);
    painter->drawText(res.rect, label_);
}

double PlotValuesGraphicsItem::width() const
{
    return width_;
}

void PlotValuesGraphicsItem::setWidth(double width)
{
    width_ = width;
}

double PlotValuesGraphicsItem::height() const
{
    return height_;
}

void PlotValuesGraphicsItem::setHeight(double height)
{
    height_ = height;
}

QVector<double> PlotValuesGraphicsItem::partLenghts() const
{
    return partLenghts_;
}

void PlotValuesGraphicsItem::setPartLenghts(const QVector<double> &partLenghts)
{
    partLenghts_ = partLenghts;
}

PlotValuesGraphicsItem::LabelResult PlotValuesGraphicsItem::createText(const QRect &rect, int flags) const
{
    QFont correctFont = QApplication::font();

    QRect tempRect;
    for (int i = 1; i < 100; ++i) {
        correctFont.setPixelSize(i);
        tempRect = QFontMetrics(correctFont).boundingRect(rect, flags, label_);
        if (tempRect.height() > rect.height() || tempRect.width() > rect.width()) {
            correctFont.setPixelSize(i - 1);
            return {correctFont, tempRect};
        }
    }
    return {correctFont, tempRect};
}

QString PlotValuesGraphicsItem::label() const
{
    return label_;
}

void PlotValuesGraphicsItem::setLabel(const QString &label)
{
    label_ = label;
}
