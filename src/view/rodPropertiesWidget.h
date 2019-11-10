#ifndef RODPROPERTIESWIDGET_H
#define RODPROPERTIESWIDGET_H

#include <QtWidgets>
#include "../construction/rod.h"

/*!
  Виджет для редактирование стержня.
*/
class RodPropertiesWidget : public QTableWidget
{
    Q_OBJECT

public:
    /*!
      Конструктор по умолчанию.
    */
    RodPropertiesWidget();
    /*!
      Присоединяет стержнь к виджету для редактирования.
     \param index - индекс стержня.
     \param rods - стержни.
    */
    void bind(int index, QVector<Rod> *rods);

signals:
    /*!
      Стержнь был изменен.
    */
    void rodChanged();

private slots:
    /*!
      Вызывается при изменение значений стержня.
    */
    void rodDataChanged();

private:
    QDoubleSpinBox *doubleSpinBoxLength;            /*!< Виджет длины. */
    QDoubleSpinBox *doubleSpinBoxSectionalArea;     /*!< Виджет площади поперечного сечения. */
    QDoubleSpinBox *doubleSpinBoxElasticUnit;       /*!< Виджет модуля упругости. */
    QDoubleSpinBox *doubleSpinBoxAllowableStrain;   /*!< Виджет допускаемого напряжения. */

    int index_;             /*!< Индекс. */
    QVector<Rod> *rods_;    /*!< Стержни. */
};

#endif // RODPROPERTIESWIDGET_H
