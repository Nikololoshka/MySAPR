#ifndef QDOUBLESPINBOXFINE_H
#define QDOUBLESPINBOXFINE_H

#include <QtWidgets>

/*!
  QDoubleSpinBox с сворачиваем в экспоненциальную форму число
  и возможность ввода экспоненциальной формы.
*/
class QDoubleSpinBoxFine : public QDoubleSpinBox
{
public:
    QDoubleSpinBoxFine();
    QString textFromValue(double val) const override;
};

#endif // QDOUBLESPINBOXFINE_H
