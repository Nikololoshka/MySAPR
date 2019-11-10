#ifndef SECTIONALFORCEPROPERTIESWIDGET_H
#define SECTIONALFORCEPROPERTIESWIDGET_H

#include <QtWidgets>
#include "../construction/sectionalForce.h"

/*!
  Виджет для редактирование распределенной нагрузки.
*/
class SectionalForcePropertiesWidget : public QTableWidget
{
    Q_OBJECT

public:
    /*!
      Конструктор по умолчанию.
    */
    SectionalForcePropertiesWidget();
    /*!
      Присоединяет нагрузку к виджету для редактирования.
     \param index - индекс нагрузки.
     \param forces - нагрузки.
    */
    void bind(int index, QVector<SectionalForce> *forces);

signals:
    /*!
      Нагрузка была изменена.
    */
    void forceChanged();

private slots:
    /*!
       Вызывается при изменение значений нагрузки.
    */
    void sectionalForceChanged();

private:
    QSpinBox *spinBoxRodId;                         /*!< Номер стержня */
    QDoubleSpinBox *doubleSpinBoxSectionalForce;    /*!< Значение силы */

    int index_;                         /*!< Индекс нагрузки.*/
    QVector<SectionalForce> *forces_;   /*!< Нагрузки. */
};

#endif // SECTIONALFORCEPROPERTIESWIDGET_H
