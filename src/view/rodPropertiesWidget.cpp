#include "rodPropertiesWidget.h"
#include "../widgets/qDoubleSpinBoxFine.h"

RodPropertiesWidget::RodPropertiesWidget()
    : QTableWidget()
{
    setColumnCount(2);
    setRowCount(4);

    verticalHeader()->setVisible(false);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setVisible(false);

    setCellWidget(0, 0, new QLabel(tr("Длина (м)")));
    doubleSpinBoxLength = new QDoubleSpinBoxFine();
    doubleSpinBoxLength->setMinimum(qPow(0.1, 6));
    doubleSpinBoxLength->setMaximum(qPow(100, 6));
    doubleSpinBoxLength->setDecimals(6);
    setCellWidget(0, 1, doubleSpinBoxLength);

    setCellWidget(1, 0, new QLabel(tr("<p>Сечение (м<sup>2</sup>)</p>")));
    doubleSpinBoxSectionalArea = new QDoubleSpinBoxFine();
    doubleSpinBoxSectionalArea->setMinimum(qPow(0.1, 6));
    doubleSpinBoxSectionalArea->setMaximum(qPow(100, 6));
    doubleSpinBoxSectionalArea->setDecimals(6);
    setCellWidget(1, 1, doubleSpinBoxSectionalArea);

    setCellWidget(2, 0, new QLabel(tr("Модуль упруг. (Па)")));
    doubleSpinBoxElasticUnit = new QDoubleSpinBoxFine();
    doubleSpinBoxElasticUnit->setMinimum(qPow(0.1, 8));
    doubleSpinBoxElasticUnit->setMaximum(qPow(100, 8));
    doubleSpinBoxElasticUnit->setDecimals(8);
    setCellWidget(2, 1, doubleSpinBoxElasticUnit);

    setCellWidget(3, 0, new QLabel(tr("Напряжение (Па)")));
    doubleSpinBoxAllowableStrain = new QDoubleSpinBoxFine();
    doubleSpinBoxAllowableStrain->setMinimum(qPow(0.1, 8));
    doubleSpinBoxAllowableStrain->setMaximum(qPow(100, 8));
    doubleSpinBoxAllowableStrain->setDecimals(8);
    setCellWidget(3, 1, doubleSpinBoxAllowableStrain);

    for (int i = 0; i < rowCount(); ++i) {
        cellWidget(i, 0)->setStyleSheet("padding-left: 3px; padding-right: 3px;");
    }

    // connections
    connect(doubleSpinBoxLength, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &RodPropertiesWidget::rodDataChanged);
    connect(doubleSpinBoxSectionalArea, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &RodPropertiesWidget::rodDataChanged);
    connect(doubleSpinBoxElasticUnit, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &RodPropertiesWidget::rodDataChanged);
    connect(doubleSpinBoxAllowableStrain, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &RodPropertiesWidget::rodDataChanged);
}

void RodPropertiesWidget::bind(int index, QVector<Rod> *rods)
{
    index_ = -1;
    rods_ = rods;

    doubleSpinBoxLength->setValue(rods->at(index).length());
    doubleSpinBoxSectionalArea->setValue(rods->at(index).sectionalArea());
    doubleSpinBoxElasticUnit->setValue(rods->at(index).elasticUnit());
    doubleSpinBoxAllowableStrain->setValue(rods->at(index).allowableStrain());

    index_ = index;
}

void RodPropertiesWidget::rodDataChanged()
{
    if (index_ == -1)
        return;

    qDebug() << "Edit " << index_;

    (*rods_)[index_].setLength(doubleSpinBoxLength->value());
    (*rods_)[index_].setSectionalArea(doubleSpinBoxSectionalArea->value());
    (*rods_)[index_].setElasticUnit(doubleSpinBoxElasticUnit->value());
    (*rods_)[index_].setAllowableStrain(doubleSpinBoxAllowableStrain->value());

    emit rodChanged();
}
