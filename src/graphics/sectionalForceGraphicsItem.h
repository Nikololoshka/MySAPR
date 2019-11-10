#ifndef SECTIONALFORCEGRAPHICSITEM_H
#define SECTIONALFORCEGRAPHICSITEM_H

#include <QtWidgets>

/*!
  Графический объект распределенной нагрузки.
*/
class SectionalForceGraphicsItem : public QGraphicsItem
{
public:
    /*!
      Конструктор по умолчанию.
    */
    SectionalForceGraphicsItem();
    /*!
      Конструктор с параметрами.
     \param drawStep - минимальный размер отрисовки.
     \param rodId - номер стержня.
     \param value - значение нагрузки.
    */
    SectionalForceGraphicsItem(int minDrawStep, int rodId, double value);
    /*!
      Конструктор копирования.
     \param force - объект распределенной нагрузки.
    */
    SectionalForceGraphicsItem(const SectionalForceGraphicsItem *force);

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
      Обновляет значения нагрузки.
     \param rodId - номер стержня.
     \param value - значени нагрузки.
    */
    void updateValues(int rodId, double value);
    /*!
      Устанавливает, отображать ли значение нагрузки.
     \param show - параметр показа.
    */
    void showLabel(bool show);

    /*!
     Возвращает номер стержня.
    */
    int rodId() const;
    /*!
     Устанавливает номер стержня.
    */
    void setRodId(int rodId);

    /*!
     Возвращает ширину.
    */
    double width() const;
    /*!
     Устанавливает ширину.
    */
    void setWidth(double width);

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
      Структура с данными для отрисовки значения нагрузки.
      \see SectionalForceGraphicsItem::showLabel
    */
    struct LabelResult {
        QFont font;     /*!< Шрифт. */
        QString text;   /*!< Текст. */
        QRect rect;     /*!< Прямоугольник для отрисоки. */
    };

    /*!
      Создает данные для отрисовки значения нагрузки.
     \param rect - максимальная область для отображения.
     \param flags - параметры отображения.
     \return LabelResult - данные для отрисовки.
    */
    LabelResult createText(const QRect &rect, int flags) const;

private:
    int minDrawStep;    /*!< Минимальный размер для отрисовки. */
    int rodId_;         /*!< Номер стержня. */
    double width_;      /*!< Ширина. */
    double value_;      /*!< Значение нагрузки. */
    bool isLabelShow;   /*!< Отвечает за отображение значения нагрузки. */
};

#endif // SECTIONALFORCEGRAPHICSITEM_H
