#include "qDoubleSpinBoxNotZero.h"

QDoubleSpinBoxNotZero::QDoubleSpinBoxNotZero()
    : QDoubleSpinBoxFine()
{
    connect(this, QOverload<double>::of(&QDoubleSpinBoxNotZero::valueChanged),
            this, &QDoubleSpinBoxNotZero::onValueChanged);
}


QValidator::State QDoubleSpinBoxNotZero::validate(QString &input, int &pos) const
{
    if (abs(QString(input).replace(",", ".").toDouble()) < qPow(0.1, decimals())) {
        return QValidator::State::Intermediate;
    }

    return QDoubleSpinBox::validate(input, pos);
}

void QDoubleSpinBoxNotZero::onValueChanged(double val)
{
    if (qAbs(val) < qPow(0.1, decimals())) {
        setValue(qPow(0.1, decimals()));
    }
}


