#ifndef RODLENGTHGRAPHICSITEM_H
#define RODLENGTHGRAPHICSITEM_H

#include <QtWidgets>

/*!
  Графический объект отметки длины стержня.
*/
class RodLengthGraphicsItem : public QGraphicsItem
{
public:
    /*!
      Конструктор по умолчанию.
    */
    RodLengthGraphicsItem();

    /*!
      Переопределение метода.
     \see QGraphicsItem::boundingRect
    */
    QRectF boundingRect() const override;
    /*!
      Переопределение метода.
     \see QGraphicsItem::paint
    */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /*!
     Возвращает ширину.
    */
    double width() const;
    /*!
     Устанавливает ширину.
    */
    void setWidth(double width);

    /*!
     Возвращает высоту.
    */
    double height() const;
    /*!
     Устанавливает высоту.
    */
    void setHeight(double height);

    /*!
     Возвращает значения длины.
    */
    double value() const;
    /*!
     Устанавливает значения длины.
    */
    void setValue(double value);

private:
    /*!
      Структура с данными для отрисовки значения длины.
    */
    struct LabelResult {
        QFont font;     /*!< Шрифт. */
        QString text;   /*!< Текст. */
        QRect rect;     /*!< Прямоугольник для отрисоки. */
    };

    /*!
      Создает данные для отрисовки значения длины.
     \param rect - максимальная область для отображения.
     \param flags - параметры отображения.
     \return LabelResult - данные для отрисовки.
    */
    LabelResult createText(const QRect &rect, int flags) const;

private:
    double width_;  /*!< Ширина. */
    double height_; /*!< Высота. */
    double value_;  /*!< Значение длины. */
};

#endif // RODLENGTHGRAPHICSITEM_H
