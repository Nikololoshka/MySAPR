#ifndef PILLARGRAPHICSITEM_H
#define PILLARGRAPHICSITEM_H

#include <QtWidgets>

/*!
  Графический объект заделки.
*/
class PillarGraphicsItem : public QGraphicsItem
{
public:
    /*!
     Перечисление с направлением приложения заделки.
    */
    enum Direction {
        None,
        Right,
        Left
    };

    /*!
      Конструктор по умолчанию.
    */
    PillarGraphicsItem();
    /*!
      Конструктор с параметрами.
     \param drawStep - минимальный шаг отрисовки.
     \param direction - направление приложения.
    */
    PillarGraphicsItem(int minDrawStep, Direction direction);

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
      Возвращает размер дополнительного выхода заделки за пределы высота стержня.

      \details Используется для красивого отображения заделки с небольшими
               выходами за пределы сверзу и снизу.
    */
    int extra() const;
    /*!
      Возвращает высоту заделки.
    */
    double height() const;
    /*!
      Устанавливает высоту заделки.
    */
    void setHeight(double height);

private:
    int minDrawStep;        /*!< Минимальный размер отрисовки. */
    int extra_;             /*!< Дополнительный выход заделки. */
    double height_;         /*!< Высота. */
    Direction direction;    /*!< Направление приложения. */
};

#endif // PILLARGRAPHICSITEM_H
