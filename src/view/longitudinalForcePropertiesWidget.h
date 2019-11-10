#ifndef LONGITUDINALFORCEPROPERTIESWIDGET_H
#define LONGITUDINALFORCEPROPERTIESWIDGET_H

#include <QtWidgets>
#include "../construction/longitudinalForce.h"

/*!
  Виджет для редактирование сосредоточенной нагрузки.
*/
class LongitudinalForcePropertiesWidget : public QTableWidget
{
    Q_OBJECT

public:
    /*!
      Конструктор по умолчанию.
    */
    LongitudinalForcePropertiesWidget();
    /*!
      Присоединяет нагрузку к виджету для редактирования.
     \param index - индекс нагрузки.
     \param forces - нагрузки.
    */
    void bind(int index, QVector<LongitudinalForce> *forces);

signals:
    /*!
      Нагрузка была изменена.
    */
    void forceChanged();

private slots:
    /*!
      Вызывается при изменение значений нагрузки.
    */
    void longitudinalForceChanged();

private:
    QSpinBox *spinBoxNodeId;                        /*!< Номер узла. */
    QDoubleSpinBox *doubleSpinBoxLongitudinalForce; /*!< Значение нагрузки. */

    int index_;                             /*!< Индекс нагрузки.*/
    QVector<LongitudinalForce> *forces_;    /*!< Нагрузки. */
};

#endif // LONGITUDINALFORCEPROPERTIESWIDGET_H
