#include "sectionalForcePropertiesWidget.h"

#include "../widgets/qDoubleSpinBoxNotZero.h"

SectionalForcePropertiesWidget::SectionalForcePropertiesWidget()
    : QTableWidget()
{
    setColumnCount(2);
    setRowCount(2);

    verticalHeader()->setVisible(false);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setVisible(false);

    setCellWidget(0, 0, new QLabel(tr("Номер стержня")));
    spinBoxRodId = new QSpinBox();
    spinBoxRodId->setMinimum(1);
    setCellWidget(0, 1, spinBoxRodId);

    setCellWidget(1, 0, new QLabel(tr("q (Н/м)")));
    doubleSpinBoxSectionalForce = new QDoubleSpinBoxNotZero();
    doubleSpinBoxSectionalForce->setMinimum(-qPow(100, 6));
    doubleSpinBoxSectionalForce->setMaximum(qPow(100, 6));
    doubleSpinBoxSectionalForce->setDecimals(6);
    setCellWidget(1, 1, doubleSpinBoxSectionalForce);

    for (int i = 0; i < rowCount(); ++i) {
        cellWidget(i, 0)->setStyleSheet("padding-left: 3px; padding-right: 3px;");
    }

    // connections
    connect(spinBoxRodId, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SectionalForcePropertiesWidget::sectionalForceChanged);
    connect(doubleSpinBoxSectionalForce, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SectionalForcePropertiesWidget::sectionalForceChanged);
}

void SectionalForcePropertiesWidget::bind(int index, QVector<SectionalForce> *forces)
{
    index_ = -1;
    forces_ = forces;

    spinBoxRodId->setValue(forces->at(index).rodId());
    doubleSpinBoxSectionalForce->setValue(forces->at(index).force());

    index_ = index;
}

void SectionalForcePropertiesWidget::sectionalForceChanged()
{
    if (index_ == -1)
        return;

    qDebug() << "Edit " << index_;

    (*forces_)[index_].setRodId(spinBoxRodId->value());
    (*forces_)[index_].setForce(doubleSpinBoxSectionalForce->value());

    emit forceChanged();
}
