#include "postProcessorWindow.h"

PostProcessorWindow::PostProcessorWindow(QSharedPointer<Processor> &processor,
                                         ConstructionGraphicsScene *scene,
                                         QWidget *parent)
    : QDialog(parent),
      processor(processor),
      currentTableComponent(-1),
      currentTableDisplayedRod(SHOW_ALL),
      currentPlotComponent(-1),
      currentPlotDisplayedRod(SHOW_ALL)
{
    setWindowTitle(tr("Результаты расчёта"));
    setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setMinimumSize(600, 600);

    tabWidgetMode = new QTabWidget();

    initGraphicMode(scene);
    initPlottingMode();
    initTableMode();

    // menu
    QMenuBar *menuBar = new QMenuBar();
    QMenu *menuFile = menuBar->addMenu(tr("Файл"));
    menuFile->addAction(tr("Сохранить расчет"),
                        this, &PostProcessorWindow::saveComputeResultClicked);

    // layout setup
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(tabWidgetMode);

    setLayout(mainLayout);

    // connections
    connect(tabWidgetMode, &QTabWidget::currentChanged,
            this, &PostProcessorWindow::currentModeChanged);

    connect(spinBoxScale, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &PostProcessorWindow::updateViewArea);

    connect(comboBoxTableComponents, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PostProcessorWindow::changeComponentTable);
    connect(comboBoxTableRod, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PostProcessorWindow::changeTableDisplayedRodComponent);
    connect(spinBoxCountTable, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &PostProcessorWindow::updateComponentTableValues);

    connect(comboBoxPlotComponents, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PostProcessorWindow::changeComponentPlot);
    connect(comboBoxPlotRod, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PostProcessorWindow::changePlotDisplayedRodComponent);
    connect(doubleSpinBoxSectionValue, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &PostProcessorWindow::sectionalValueChanged);
    connect(plot, &QCustomPlot::mouseMove,
            this, &PostProcessorWindow::showPointToolTip);
}

void PostProcessorWindow::resizeEvent(QResizeEvent *event)
{
    if (tabWidgetMode->currentIndex() == GRAPHIC_MODE) {
        QSize size = graphicView->size() * 0.9;
        graphicScene->updateSize(size.width(), size.height());
    }

    QDialog::resizeEvent(event);
}

void PostProcessorWindow::saveComputeResultClicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Сохранение результатов расчета"), ".",
                                                "JSON files (*.json) ;; All files (*.*)");

    if (path.isEmpty())
        return;

    try {
        processor->save(path);
    } catch (...) {
        QMessageBox::critical(this, tr("Ошибка сохранения"),
                              tr("При сохранении результатов расчета произошла ошибка."));
        return;
    }
    QMessageBox::information(this, tr("Информация"),
                             tr("Результаты расчета успешно сохранены."));
}

void PostProcessorWindow::currentModeChanged(int index)
{
    switch (index) {
    case GRAPHIC_MODE: {
        QSize size = graphicView->size() * 0.9;
        graphicScene->updateSize(size.width(), size.height());
        break;
    }
    case TABLE_MODE:
        if (currentTableComponent != comboBoxTableComponents->currentIndex()) {
            changeComponentTable(comboBoxTableComponents->currentIndex());
        }
        break;
    case PLOTTING_MODE:
        if (currentPlotComponent != comboBoxPlotComponents->currentIndex()) {
            changeComponentPlot(comboBoxPlotComponents->currentIndex());
        }
        break;
    }
}

void PostProcessorWindow::updateViewArea(int scale)
{
    double scaleFactor = static_cast<double>(scale - scale_) / scale_;
    graphicView->scale(1 + scaleFactor, 1 + scaleFactor);
    scale_ = scale;
}

void PostProcessorWindow::changeComponentTable(int index)
{
    currentTableComponent = index - 1;
    updateComponentTableValues(spinBoxCountTable->value());
}

void PostProcessorWindow::changeTableDisplayedRodComponent(int index)
{
    currentTableDisplayedRod = index - 1;
    updateComponentTableValues(spinBoxCountTable->value());
}

void PostProcessorWindow::updateComponentTableValues(int rows)
{
    const QVector<CalcMethod> methods = {
        &Processor::movement,
        &Processor::longitudinalForce,
        &Processor::normalStrain
    };

    QStringList defaultColumns = {
        tr("Номер стерженя"),
        tr("Номер части")
    };

    const QStringList otherColumns = {
        tr("Перемещение"),
        tr("Продольная сила"),
        tr("Нормальное напряжение")
    };

    tableWidgetResult->clearSpans();

    // установка ограничений на показываемые
    // стержни и компоненты

    int minPart = 0;
    int maxPart = processor->parts() - 1;

    int minComponent = 0;
    int maxComponent = methods.size() - 1;

    if (currentTableDisplayedRod == SHOW_ALL) {
        tableWidgetResult->setRowCount(rows * processor->parts());
    } else {
        tableWidgetResult->setRowCount(rows);
        minPart = maxPart = currentTableDisplayedRod;
    }

    if (currentTableComponent == SHOW_ALL) {
        defaultColumns.append(otherColumns);
    } else {
        defaultColumns.append(otherColumns[currentTableComponent]);
        minComponent = maxComponent = currentTableComponent;
    }

    tableWidgetResult->setColumnCount(defaultColumns.size());
    tableWidgetResult->setHorizontalHeaderLabels(defaultColumns);

    int realRow = 0;
    for (int i = minPart; i <= maxPart; ++i) {
        // Для каждой части
        double step = processor->partLength(i) / (rows - 1);
        QTableWidgetItem *itemRod = tableWidgetResult->item(realRow, 0);
        if (itemRod == nullptr) {
            itemRod = new QTableWidgetItem();
            tableWidgetResult->setItem(realRow, 0, itemRod);
        }
        itemRod->setText(QString::number(i + 1));

        tableWidgetResult->setSpan(realRow, 0, rows, 1);

        for (int j = 0; j < rows; ++j) {
            // Для каждого разбиения части
            QTableWidgetItem *itemNumber = tableWidgetResult->item(realRow + j, 1);
            if (itemNumber == nullptr) {
                itemNumber = new QTableWidgetItem();
                tableWidgetResult->setItem(realRow + j, 1, itemNumber);
            }
            itemNumber->setText(QString::number(j + 1));

            int column = 2;
            for (int k = minComponent; k <= maxComponent; ++k) {
                // Для каждой компоненты
                QTableWidgetItem *itemComponent = tableWidgetResult->item(realRow + j, column);
                if (itemComponent == nullptr) {
                    itemComponent = new QTableWidgetItem();
                    tableWidgetResult->setItem(realRow + j, column, itemComponent);
                }

                auto value = ((*processor).*methods[k])(i, j * step);
                itemComponent->setText(QString::number(value));

                if (methods[k] == &Processor::normalStrain && qAbs(value) > processor->maxAllowableStrain(i)) {
                    itemComponent->setBackground(QColor(255, 0, 0, 128));
                } else {
                    itemComponent->setBackground(Qt::white);
                }

                ++column;
            }
        }
        realRow += rows;
    }
}

void PostProcessorWindow::changeComponentPlot(int index)
{
    currentPlotComponent = index - 1;
    updateComponentPlotValues();
}

void PostProcessorWindow::changePlotDisplayedRodComponent(int index)
{
    currentPlotDisplayedRod = index - 1;
    updateComponentPlotValues();
}

void PostProcessorWindow::updateComponentPlotValues()
{
    const QVector<CalcMethod> methods = {
            &Processor::movement,
            &Processor::longitudinalForce,
            &Processor::normalStrain
    };

    const QVector<Qt::GlobalColor> colors = {
        Qt::magenta,
        Qt::blue,
        Qt::green
    };

    const QVector<QString> labels = {
        tr("Перемещение"),
        tr("Продольная сила"),
        tr("Нормальное напряжение")
    };

    plot->clearGraphs();

    // установка ограничений на показываемые
    // стержни и компоненты

    int minPart = 0;
    int maxPart = processor->parts() - 1;

    if (currentPlotDisplayedRod != SHOW_ALL) {
        minPart = maxPart = currentPlotDisplayedRod;
    }

    int minMethod = 0;
    int maxMethod = methods.size() - 1;

    if (currentPlotComponent != SHOW_ALL) {
        minMethod = maxMethod = currentPlotComponent;
    }

    QMap<QString, QCPGraph*> showedMethos;

    double realX = 0;
    for (int i = minPart; i <= maxPart; ++i) {
        // Для каждой части
        double width = processor->partLength(i);
        double step = width / (width * 10);

        for (int j = minMethod; j <= maxMethod; ++j) {
            // Для каждого метода
            QCPGraph *graph = this->plot->addGraph();

            QVector<double> xs, ys;
            for (double k = 0; k < width + step / 2; k += step) {
                xs.append(realX + k);
                ys.append(((*this->processor).*methods[j])(i, k));
            }

            showedMethos[labels[j]] = graph;

            graph->setPen(QPen(colors[j], 2));
            graph->setName(labels[j]);
            graph->setData(xs, ys);
        }

        realX += width;
    }

    // Установка легенды
    plot->legend->clearItems();
    for (auto &graph : showedMethos) {
        QCPPlottableLegendItem *item = new QCPPlottableLegendItem(plot->legend, graph);
        plot->legend->addItem(item);
    }

    plot->xAxis->rescale(true);
    plot->yAxis->rescale(true);
    plot->replot();
}

void PostProcessorWindow::sectionalValueChanged(double value)
{
    if (value >= 0) {
        for (int i = 0; i < processor->parts(); ++i) {
            if (value - processor->partLength(i) <= 0) {
                auto m = processor->movement(i, value);
                auto n = processor->longitudinalForce(i, value);
                auto s = processor->normalStrain(i, value);
                labelSectionValues->setText(tr("U<sub>x</sub>: %1   "
                                               "N<sub>x</sub>: %2   "
                                               "&sigma;<sub>x</sub>: %3")
                                            .arg(m).arg(n).arg(s));
                return;
            }
            value -= processor->partLength(i);
        }
    }
}

void PostProcessorWindow::showPointToolTip(QMouseEvent *event)
{
    if (currentPlotDisplayedRod == SHOW_ALL) {
        double value = plot->xAxis->pixelToCoord(event->pos().x());
        doubleSpinBoxSectionValue->setValue(value);
    }
}

void PostProcessorWindow::initGraphicMode(ConstructionGraphicsScene *scene)
{
    graphicScene = new ConstructionPlotGraphicsScene(processor);
    processor->bind(graphicScene);

    graphicView = new QGraphicsView(graphicScene);
    graphicView->setRenderHint(QPainter::Antialiasing, true);
    graphicView->setFocusPolicy(Qt::NoFocus);
    // graphicView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    graphicScene->addPlot("U" + QString::fromUtf8("\u2093"), &Processor::movement);
    graphicScene->addPlot("N" + QString::fromUtf8("\u2093"), &Processor::longitudinalForce);
    graphicScene->addPlot(QString::fromUtf8("\u03C3\u2093"), &Processor::normalStrain);

    QMenu *menuView = new QMenu();
    QAction *actionRodLabels =
            menuView->addAction(tr("Показать номера &стержней"),
                                graphicScene,
                                &ConstructionPlotGraphicsScene::showRodLabels,
                                QKeySequence(Qt::CTRL + Qt::Key_R));
    actionRodLabels->setCheckable(true);

    QAction *actionNodeLabels =
            menuView->addAction(tr("Показать номера &узлов"),
                                graphicScene,
                                &ConstructionPlotGraphicsScene::showNodeLabels,
                                QKeySequence(Qt::CTRL + Qt::Key_T));
    actionNodeLabels ->setCheckable(true);

    QAction *actionLongitudinalLabels =
            menuView->addAction(tr("Показать значения F нагрузок"),
                                graphicScene,
                                &ConstructionPlotGraphicsScene::showLongitudinalLabels,
                                QKeySequence(Qt::CTRL + Qt::Key_Y));
    actionLongitudinalLabels->setCheckable(true);

    QAction *actionSectionalLabels =
            menuView->addAction(tr("Показать значения q нагрузок"),
                                graphicScene,
                                &ConstructionPlotGraphicsScene::showSectionalLabels,
                                QKeySequence(Qt::CTRL + Qt::Key_U));
    actionSectionalLabels->setCheckable(true);

    QAction *actionRodLengths =
            menuView->addAction(tr("Показать длину стержней"),
                                graphicScene,
                                &ConstructionPlotGraphicsScene::showRodLengths,
                                QKeySequence(Qt::CTRL + Qt::Key_I));
    actionRodLengths->setCheckable(true);

    menuView->addSeparator();

    QAction *actionlongitudinalforceToNode =
            menuView->addAction(tr("Нагрузки F к узлу"),
                                graphicScene,
                                &ConstructionPlotGraphicsScene::longitudinalforceToNode,
                                QKeySequence(Qt::CTRL + Qt::Key_B));
    actionlongitudinalforceToNode->setCheckable(true);

    menuView->addSeparator();
    QAction *actionPlotsValues =
            menuView->addAction(tr("Отображать значения эпюра"),
                                graphicScene,
                                &ConstructionPlotGraphicsScene::showPlotsValues,
                                QKeySequence(Qt::CTRL + Qt::Key_V));
    actionPlotsValues->setCheckable(true);

    QToolButton *buttonView = new QToolButton();
    buttonView->setText(tr("Настройка отображения"));
    buttonView->setPopupMode(QToolButton::InstantPopup);
    buttonView->setMenu(menuView);

    // перенос настроек с сцены
    if (scene != nullptr) {
        graphicScene->showRodLabels(scene->displayRodNumbers());
        graphicScene->showNodeLabels(scene->displayNodeNumbers());
        graphicScene->showRodLengths(scene->displyRodLengths());
        graphicScene->showLongitudinalLabels(scene->displayLongitudinalLabels());
        graphicScene->showSectionalLabels(scene->displaySectionalLabels());
        graphicScene->longitudinalforceToNode(scene->isForceToNode());

        actionRodLabels->setChecked(scene->displayRodNumbers());
        actionNodeLabels->setChecked(scene->displayNodeNumbers());
        actionLongitudinalLabels->setChecked(scene->displyRodLengths());
        actionSectionalLabels->setChecked(scene->displayLongitudinalLabels());
        actionRodLengths->setChecked(scene->displaySectionalLabels());
        actionlongitudinalforceToNode->setChecked(scene->isForceToNode());
    }

    spinBoxScale = new QSpinBox();
    spinBoxScale->setRange(50, 500);
    spinBoxScale->setValue(100);
    spinBoxScale->setSingleStep(5);
    spinBoxScale->setSuffix("%");

    scale_ = 100;

    QHBoxLayout *upGraphicLayout = new QHBoxLayout();
    upGraphicLayout->addWidget(buttonView);
    upGraphicLayout->addStretch(1);
    upGraphicLayout->addWidget(new QLabel(tr("Масштаб")));
    upGraphicLayout->addWidget(spinBoxScale);

    QVBoxLayout *graphicLayout = new QVBoxLayout();
    graphicLayout->addLayout(upGraphicLayout);
    graphicLayout->addWidget(graphicView);

    QWidget *graphicWidget = new QWidget();
    graphicWidget->setLayout(graphicLayout);

    tabWidgetMode->insertTab(GRAPHIC_MODE, graphicWidget, tr("Конструкция с эпюрами"));

    QSize size = graphicView->size() * 0.9;
    graphicScene->updateSize(size.width(), size.height());
}

void PostProcessorWindow::initPlottingMode()
{
    plot = new QCustomPlot();
    plot->legend->setVisible(true);
    plot->setAutoAddPlottableToLegend(false);
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->xAxis->setLabel(tr("Длина стержня"));
    plot->yAxis->setLabel(tr("Значение"));

    comboBoxPlotComponents = new QComboBox();
    comboBoxPlotComponents->addItem(tr("Все"));
    comboBoxPlotComponents->addItem(tr("Перемещения"));
    comboBoxPlotComponents->addItem(tr("Продольные силы"));
    comboBoxPlotComponents->addItem(tr("Нормальные напряжения"));

    comboBoxPlotRod = new QComboBox();
    comboBoxPlotRod->addItem(tr("Все"));
    for (int i = 0; i < processor->parts(); ++i) {
        comboBoxPlotRod->addItem(tr("Стержень %1").arg(i + 1));
    }

    doubleSpinBoxSectionValue = new QDoubleSpinBox();
    doubleSpinBoxSectionValue->setDecimals(6);

    double commonWidth = 0;
    for (int i = 0; i < processor->parts(); ++i) {
        commonWidth += processor->partLength(i);
    }
    doubleSpinBoxSectionValue->setRange(0, commonWidth);

    labelSectionValues = new QLabel(tr("U<sub>x</sub>: ---   "
                                       "N<sub>x</sub>: ---   "
                                       "&sigma;<sub>x</sub>: ---"));

    QHBoxLayout *upPlotLayout = new QHBoxLayout();
    upPlotLayout->addWidget(new QLabel(tr("Компонента")));
    upPlotLayout->addWidget(comboBoxPlotComponents);
    upPlotLayout->addWidget(new QLabel(tr("Стержень")));
    upPlotLayout->addWidget(comboBoxPlotRod);
    upPlotLayout->addStretch(1);

    QHBoxLayout *upPlotSectionLayout = new QHBoxLayout();
    upPlotSectionLayout->addWidget(new QLabel(tr("Значения в сечении")));
    upPlotSectionLayout->addWidget(doubleSpinBoxSectionValue);
    upPlotSectionLayout->addWidget(labelSectionValues);
    upPlotSectionLayout->addStretch(1);

    QHBoxLayout *centerPlotLayout = new QHBoxLayout();
    centerPlotLayout->addWidget(plot);

    QVBoxLayout *mainPlotLayout = new QVBoxLayout();
    mainPlotLayout->addLayout(upPlotLayout);
    mainPlotLayout->addLayout(upPlotSectionLayout);
    mainPlotLayout->addLayout(centerPlotLayout, 1);

    QWidget *plotWidget = new QWidget();
    plotWidget->setLayout(mainPlotLayout);

    tabWidgetMode->insertTab(PLOTTING_MODE, plotWidget, tr("Графическое представление компонент"));
}

void PostProcessorWindow::initTableMode()
{
    tableWidgetResult = new QTableWidget();
    tableWidgetResult->setColumnCount(3);
    tableWidgetResult->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidgetResult->verticalHeader()->setVisible(false);
    tableWidgetResult->setEditTriggers(QTableWidget::NoEditTriggers);

    spinBoxCountTable = new QSpinBox();
    spinBoxCountTable->setRange(2, 1e6);

    comboBoxTableComponents = new QComboBox();
    comboBoxTableComponents->addItem(tr("Все"));
    comboBoxTableComponents->addItem(tr("Перемещения"));
    comboBoxTableComponents->addItem(tr("Продольные силы"));
    comboBoxTableComponents->addItem(tr("Нормальные напряжения"));

    comboBoxTableRod = new QComboBox();
    comboBoxTableRod->addItem(tr("Все"));
    for (int i = 0; i < processor->parts(); ++i) {
        comboBoxTableRod->addItem(tr("Стержень %1").arg(i + 1));
    }

    QHBoxLayout *upTableLayout = new QHBoxLayout();
    upTableLayout->addWidget(new QLabel(tr("Компонента")));
    upTableLayout->addWidget(comboBoxTableComponents);
    upTableLayout->addWidget(new QLabel(tr("Стержень")));
    upTableLayout->addWidget(comboBoxTableRod);
    upTableLayout->addStretch(1);
    upTableLayout->addWidget(new QLabel(tr("Количество частей")));
    upTableLayout->addWidget(spinBoxCountTable);

    QHBoxLayout *centerTableLayout = new QHBoxLayout();
    centerTableLayout->addWidget(tableWidgetResult);

    QVBoxLayout *mainTableLayout = new QVBoxLayout();
    mainTableLayout->addLayout(upTableLayout);
    mainTableLayout->addLayout(centerTableLayout);

    QWidget *tableWidget = new QWidget();
    tableWidget->setLayout(mainTableLayout);

    tabWidgetMode->insertTab(TABLE_MODE, tableWidget, tr("Табличное представление компонент"));
}
