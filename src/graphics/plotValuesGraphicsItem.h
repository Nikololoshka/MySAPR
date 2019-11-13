#ifndef PLOTVALUESGRAPHICSITEM_H
#define PLOTVALUESGRAPHICSITEM_H

#include <QtWidgets>
#include "../processor/processor.h"


/*!
  Графический объект эпюра.
*/
class PlotValuesGraphicsItem : public QGraphicsItem
{
public:
    /*!
      Конструктор с параметрами.
     \param drawStep - минимальный размер отрисовки.
    */
    PlotValuesGraphicsItem(double minDrawStep);

    /*!
      Устанавливает и вычисляет значения для отображения.
     \param processor - процессор, для вычисления.
     \param method - метод для вычислений.
    */
    void setValues(QSharedPointer<Processor> processor, CalcMethod method);

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
      Возвращает массив с длиннами каждой части (стержня) конструкции.
    */
    QVector<double> partLenghts() const;
    /*!
     Устанавливает массив с длиннами каждой части (стержня) конструкции.
    */
    void setPartLenghts(const QVector<double> &partLenghts);

    /*!
     Возвращает подпись эпюра.
    */
    QString label() const;
    /*!
     Устанавливает подпись эпюра.
    */
    void setLabel(const QString &label);

private:
    /*!
      Структура с данными для отрисовки подписи.
    */
    struct LabelResult {
        QFont font; /*! Шрифт. */
        QRect rect; /*!< Прямоугольник для отрисоки. */
        QString text;   /*!< Текст. */
    };

    /*!
      Создает данные для отрисовки подписи.
     \param rect - максимальная область для отображения.
     \param flags - параметры отображения.
     \param text - текст.
     \return LabelResult - данные для отрисовки.
    */
    LabelResult createText(const QRect &rect, int flags, QString text) const;

private:
    double minDrawStep; /*!< Минимальный размер отрисовки. */
    double width_;      /*!< Ширина. */
    double height_;     /*!< Высота. */

    double maxValue_;   /*!< Максимальное значения на эпюре. */
    QString label_;     /*!< Подпись эпюра. */

    /*!
      Часть (т.е. стержень) отображаемого юпюра конструкции.
    */
    struct Part {
        double left;            /*!< Значение слева. */
        double right;           /*!< Значение справа. */
        QVector<double> values; /*!< Значения для отрисовки. */
    };

    QVector<Part> parts_;           /*!< Части эпюра конструкции. */
    QVector<double> partLenghts_;   /*!< Длины частей. */
};

#endif // PLOTVALUESGRAPHICSITEM_H
