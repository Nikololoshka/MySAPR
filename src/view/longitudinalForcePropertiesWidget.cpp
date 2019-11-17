#include "longitudinalForcePropertiesWidget.h"

#include "../widgets/qDoubleSpinBoxNotZero.h"
#include <limits>

LongitudinalForcePropertiesWidget::LongitudinalForcePropertiesWidget()
    : QTableWidget()
{
    setColumnCount(2);
    setRowCount(2);

    verticalHeader()->setVisible(false);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setVisible(false);

    setCellWidget(0, 0, new QLabel(tr("Номер узла")));
    spinBoxNodeId = new QSpinBox();
    spinBoxNodeId->setMinimum(1);
    setCellWidget(0, 1, spinBoxNodeId);

    setCellWidget(1, 0, new QLabel(tr("F (Н)")));
    doubleSpinBoxLongitudinalForce = new QDoubleSpinBoxNotZero();
    doubleSpinBoxLongitudinalForce->setMinimum(-qPow(100, 6));
    doubleSpinBoxLongitudinalForce->setMaximum(qPow(100, 6));
    doubleSpinBoxLongitudinalForce->setDecimals(6);
    setCellWidget(1, 1, doubleSpinBoxLongitudinalForce);

    for (int i = 0; i < rowCount(); ++i) {
        cellWidget(i, 0)->setStyleSheet("padding-left: 3px; padding-right: 3px;");
    }

    // connections
    connect(spinBoxNodeId, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &LongitudinalForcePropertiesWidget::longitudinalForceChanged);
    connect(doubleSpinBoxLongitudinalForce, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &LongitudinalForcePropertiesWidget::longitudinalForceChanged);

}

void LongitudinalForcePropertiesWidget::bind(int index, QVector<LongitudinalForce> *forces)
{
    index_ = -1;
    forces_ = forces;

    spinBoxNodeId->setValue(forces->at(index).nodeId());
    doubleSpinBoxLongitudinalForce->setValue(forces->at(index).force());

    index_ = index;
}

void LongitudinalForcePropertiesWidget::longitudinalForceChanged()
{
    if (index_ == -1)
        return;

    // qDebug() << "Edit " << index_;

    (*forces_)[index_].setNodeId(spinBoxNodeId->value());
    (*forces_)[index_].setForce(doubleSpinBoxLongitudinalForce->value());

    emit forceChanged();
}
