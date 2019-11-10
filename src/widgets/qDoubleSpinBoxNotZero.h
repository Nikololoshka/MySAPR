#ifndef QSPINBOXNOTZERO_H
#define QSPINBOXNOTZERO_H

#include <QtWidgets>
#include "qDoubleSpinBoxFine.h"

/*!
  QDoubleSpinBoxFine не допускающий ввода нуля.
*/
class QDoubleSpinBoxNotZero : public QDoubleSpinBoxFine
{
public:
    QDoubleSpinBoxNotZero();
    QValidator::State validate(QString &input, int &pos) const override;

private slots:
    void onValueChanged(double val);
};

#endif // QSPINBOXNOTZERO_H
