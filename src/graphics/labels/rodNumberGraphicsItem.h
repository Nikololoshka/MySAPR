#ifndef RODNUMBERGRAPHICSITEM_H
#define RODNUMBERGRAPHICSITEM_H

#include <QtWidgets>

/*!
  Графический объект для отметки номера стержня.
*/
class RodNumberGraphicsItem : public QGraphicsItem
{
public:
    /*!
      Конструктор по умолчанию.
    */
    RodNumberGraphicsItem();

    /*!
     \Переопределение метода.
     \see QGraphicsItem::boundingRect
    */
    QRectF boundingRect() const override;
    /*!
      Переопределение метода.
     \see QGraphicsItem::paint
    */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /*!
     Возвращает размер объекта.
    */
    double size() const;
    /*!
     Устанавливает размер объекта.
    */
    void setSize(double size);
    /*!
     Устанавливает отображаемый номер.
    */
    void setNumber(int number);

private:
    double size_;   /*!< Размер. */
    int number_;    /*!< Номер. */
};

#endif // RODNUMBERGRAPHICSITEM_H
