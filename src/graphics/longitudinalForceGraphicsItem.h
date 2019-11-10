#ifndef LONGITUDINALFORCEGRAPHICSITEM_H
#define LONGITUDINALFORCEGRAPHICSITEM_H

#include <QtWidgets>

/*!
  Графический объект сосредаточенной нагрузки.
*/
class LongitudinalForceGraphicsItem : public QGraphicsItem
{
public:
    /*!
      Конструктор по умолчанию.
    */
    LongitudinalForceGraphicsItem();
    /*!
      Конструктор с параметрами.
     \param drawStep - минимальный размер отрисоки.
     \param nodeId - номер узла.
     \param value - значение нагрузки.
    */
    LongitudinalForceGraphicsItem(int minDrawStep, int nodeId, double value);
    /*!
      Конструктор копирования.
     \param force - сосредаточенная нагрузка.
    */
    LongitudinalForceGraphicsItem(const LongitudinalForceGraphicsItem *force);

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
      Устанавливает минамальный размер отрисовки.
     \param drawStep - размер отрисоки.
    */
    void setDrawStep(int minDrawStep);

    /*!
      Обновляет параметры нагрузки.
     \param nodeId - номер узла.
     \param value - значение силы.
    */
    void updateValues(int nodeId, double value);
    /*!
      Устанавливает, отображать ли значение нагрузки.
     \param show - параметр показа.
    */
    void showLabel(bool show);

    /*!
      Возвращает номер узла/
    */
    int nodeId() const;
    /*!
     Устанавливает номер узла.
    */
    void setNodeId(int nodeId);

    /*!
     Возвращает значение нагрузки.
    */
    double value() const;
    /*!
     Устанавливает значение нагрузки.
    */
    void setValue(double value);

private:
    /*!
      Создает "голову" отображаемой стрелки.
    */
    void createHead();

    /*!
      Структура с данными для отрисовки значения нагрузки.
      \see LongitudinalForceGraphicsItem::showLabel
    */
    struct LabelResult {
        QFont font;     /*!< Шрифт. */
        QString text;   /*!< Текст. */
        QRect rect;     /*!< Прямоугольник, где отображать. */
    };

    /*!
      Создает данные для отрисовки значения нагрузки.
     \param rect - максимальная область для отображения.
     \param flags - параметры отображения.
     \return LabelResult - данные для отрисовки.
    */
    LabelResult createText(const QRect &rect, int flags) const;

private:
    int minDrawStep;    /*!< Минимальный размер отрисовки. */
    int nodeId_;        /*!< Номер узла. */
    double value_;      /*!< Значение нагрузки. */
    bool isLabelShow;   /*!< Отвечает за отображение значения нагрузки. */

    QPolygonF arrowHead;    /*!< Полигон с "головой" стрелки. */
};

#endif // LONGITUDINALFORCEGRAPHICSITEM_H
