#ifndef RODGRAPHICSITEM_H
#define RODGRAPHICSITEM_H

#include <QtWidgets>

/*!
  Графический объект стержня.
*/
class RodGraphicsItem : public QGraphicsItem
{
public:
    /*!
      Конструктор по умолчанию.
    */
    RodGraphicsItem();
    /*!
      Конструктор с параметрами.
     \param drawStep - минимальный размер отрисовки.
    */
    explicit RodGraphicsItem(int minDrawStep);
    /*!
      Конструктор с параметрами.
     \param drawStep - минимальный размер отрисовки.
     \param length - длина.
     \param sectionalArea - площадь поперечного сечения.
    */
    RodGraphicsItem(int minDrawStep, double length, double sectionalArea);
    /*!
      Конструктор по умолчанию.
     \param rodItem - объект стержня.
    */
    RodGraphicsItem(const RodGraphicsItem *rodItem);

    /*!
      Обновляет значения стержня.
     \param length - длина.
     \param sectionalArea - площадь поперечного сечения.
    */
    void updateValues(double length, double sectionalArea);

    /*!
     Возвращает ширину (на сцене).
    */
    double width() const;
    /*!
     Устанавливает ширину (на сцене).
    */
    void setWidth(double width);

    /*!
     Возвращает высоту (на сцене).
    */
    double height() const;
    /*!
     Устанавливает высоту (на сцене).
    */
    void setHeight(double height);

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
     Устанавливает минимальный размер отрисовки.
    */
    void setDrawStep(int minDrawStep);

    /*!
     Возвращает длину.
    */
    double length() const;
    /*!
     Устанавливает длину.
    */
    void setLength(double length);

    /*!
     Возвращает площадь поперечного сечения.
    */
    double sectionalArea() const;
    /*!
     Устанавливает площадь поперечного сечения.
    */
    void setSectionalArea(double sectionalArea);

private:
    int minDrawStep;        /*!< Минимальный размер отрисовки. */
    double length_;         /*!< Длина. */
    double width_;          /*!< Ширина. */
    double height_;         /*!< Высота. */
    double sectionalArea_;  /*!< Площадь поперечного сечения. */
};

#endif // RODGRAPHICSITEM_H
