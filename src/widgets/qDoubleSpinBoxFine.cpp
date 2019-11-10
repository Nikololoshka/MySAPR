#include "qDoubleSpinBoxFine.h"

QDoubleSpinBoxFine::QDoubleSpinBoxFine()
    : QDoubleSpinBox()
{
    QDoubleValidator *validator = new QDoubleValidator();
    validator->setNotation(QDoubleValidator::ScientificNotation);
    lineEdit()->setValidator(validator);
}

QString QDoubleSpinBoxFine::textFromValue(double val) const
{
    return locale().toString(val);
}
