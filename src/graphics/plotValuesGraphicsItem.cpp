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

        painter->setBrush(Qt::VerPattern);
        painter->drawPolygon(polygon);

        if (displyValues_) {
            const double VALUE_WIDTH = minDrawStep * 4 / 10;
            const double VALUE_HEIGHT = minDrawStep / 4;

            // значения на левой части
            double dyLeft = - minDrawStep / 3;
            if (i > 0) {
                if (qAbs(parts_[i - 1].right - parts_[i].left) > 1e-10) {
                    if (parts_[i - 1].right > parts_[i].left) {
                        dyLeft = minDrawStep / 3;
                    }
                    if (polygon[1].y() > y && dyLeft > 0) {
                        dyLeft /= 3;
                    }

                    auto res = createText(QRectF(x,
                                                 polygon[1].y() + dyLeft,
                                                 VALUE_WIDTH,
                                                 VALUE_HEIGHT).toRect(),
                                          Qt::AlignCenter,
                                          QString::number(parts_[i].left, 'g', 4));
                    painter->setFont(res.font);
                    painter->setBrush(Qt::white);
                    painter->drawRect(res.rect);
                    painter->drawText(res.rect, Qt::AlignCenter, res.text);
                }
            } else {
                auto res = createText(QRectF(x,
                                             polygon[1].y() + dyLeft,
                                             VALUE_WIDTH,
                                             VALUE_HEIGHT).toRect(),
                                      Qt::AlignCenter,
                                      QString::number(parts_[i].left, 'g', 4));
                painter->setFont(res.font);
                painter->setBrush(Qt::white);
                painter->drawRect(res.rect);
                painter->drawText(res.rect, Qt::AlignCenter, res.text);
            }

            x += partLenghts_[i];

            // значения на правой части
            double dyRight = - minDrawStep / 3;
            if (i < parts_.size() - 1 && parts_[i].right < parts_[i + 1].left) {
                dyRight = minDrawStep / 3;
            }

            if (polygon[polygon.size() - 2].y() > y && dyRight > 0) {
                dyRight /= 3;
            }

            auto res = createText(QRectF(x - minDrawStep / 2,
                                         polygon[polygon.size() - 2].y() + dyRight,
                                         VALUE_WIDTH,
                                         VALUE_HEIGHT).toRect(),
                                  Qt::AlignCenter,
                                  QString::number(parts_[i].right, 'g', 4));
            painter->setFont(res.font);
            painter->setBrush(Qt::white);
            painter->drawRect(res.rect);
            painter->drawText(res.rect, Qt::AlignCenter, res.text);
        } else {
            x += partLenghts_[i];
        }
    }

    // отметка
    auto res = createText(QRectF(minDrawStep / 4, y - minDrawStep / 2,
                                 minDrawStep / 2, minDrawStep / 2).toRect(),
                          Qt::AlignCenter, label_);
    painter->setFont(res.font);
    painter->drawText(res.rect, Qt::AlignCenter, res.text);
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

PlotValuesGraphicsItem::LabelResult PlotValuesGraphicsItem::createText(const QRect &rect,
                                                                       int flags,
                                                                       QString text) const
{
    QFont correctFont = QApplication::font();

    QRect tempRect;
    for (int i = 1; i < 100; ++i) {
        correctFont.setPixelSize(i);
        tempRect = QFontMetrics(correctFont).boundingRect(rect, flags, text);
        if (tempRect.height() > rect.height() || tempRect.width() > rect.width()) {
            correctFont.setPixelSize(i - 1);
            if (text.size() <= 2) {
                tempRect.setX(tempRect.x() - 1);
                tempRect.setWidth(tempRect.width() + 2);
            }
            return {correctFont, tempRect, text};
        }
    }
    return {correctFont, tempRect, text};
}

void PlotValuesGraphicsItem::setDisplyValues(bool displyValues)
{
    displyValues_ = displyValues;
    update();
}

QString PlotValuesGraphicsItem::label() const
{
    return label_;
}

void PlotValuesGraphicsItem::setLabel(const QString &label)
{
    label_ = label;
}
