#include "mySAPRWindow.h"
#include "processor/processor.h"
#include "postprocessor/postProcessorWindow.h"

MySAPRWindow::MySAPRWindow(QWidget *parent)
    : QMainWindow(parent),
      rodIdPool(1),
      longitudinalIdPool(1),
      sectionalIdPool(1),
      isLongitudinalChecked(false),
      isSectionalChecked(false),
      hasChange(false)
{
    setWindowTitle(qAppName());
    setMinimumSize(800, 400);

    graphicScene = new ConstructionGraphicsScene();
    graphicView = new QGraphicsViewBlocked(graphicScene);
    graphicView->setRenderHint(QPainter::Antialiasing, true);
    graphicView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QSize size = graphicView->size() * 0.9;
    graphicScene->updateSize(size.width(), size.height());

    comboBoxFulcrum = new QComboBox();
    comboBoxFulcrum->addItem(tr("Слева"), ConstructionGraphicsScene::Left);
    comboBoxFulcrum->addItem(tr("Справа"), ConstructionGraphicsScene::Right);
    comboBoxFulcrum->addItem(tr("Слева и справа"), ConstructionGraphicsScene::LeftAndRight);

    // creatre tabs with construction elements
    tabWidget = new QTabWidget();

    auto propertiesSetter = [](QTreeWidget *widget) {
        widget->setHeaderHidden(true);
        widget->setStyleSheet("QTreeView::branch:has-siblings:!adjoins-item {"
                              "    border-image: url(:/stylesheet/vline.png) 0;"
                              "}"
                              "QTreeView::branch:has-siblings:adjoins-item {"
                              "    border-image: url(:/stylesheet/branch-more.png) 0;"
                              "}"
                              "QTreeView::branch:!has-children:!has-siblings:adjoins-item {"
                              "    border-image: url(:/stylesheet/branch-end.png) 0;"
                              "}");
        widget->setEditTriggers(widget->editTriggers() | QTreeWidget::NoEditTriggers);
    };

    treeWidgetRods = new QTreeWidget();
    propertiesSetter(treeWidgetRods);
    tabWidget->addTab(treeWidgetRods, tr("Стержни"));

    treeWidgetLongitudinal = new QTreeWidget();
    propertiesSetter(treeWidgetLongitudinal);
    tabWidget->addTab(treeWidgetLongitudinal, tr("Нагрузки F"));

    treeWidgetSectional = new QTreeWidget();
    propertiesSetter(treeWidgetSectional);
    tabWidget->addTab(treeWidgetSectional, tr("Нагрузки q"));

    if (tabWidget->tabBar()->sizeHint().width() < tabWidget->sizeHint().width()) {
        tabWidget->tabBar()->setFixedWidth(tabWidget->sizeHint().width());
    } else {
        tabWidget->setFixedWidth(tabWidget->tabBar()->sizeHint().width());
    }

    // creatre table with properties
    labelProperties = new QLabel(tr("Свойства:"));
    labelProperties->setVisible(false);

    propertiesWidget = new QStackedWidget();
    propertiesWidget->insertWidget(NOTHING_ELEMENT, new QWidget());

    rodPropertiesWidget = new RodPropertiesWidget();
    propertiesWidget->insertWidget(ROD_ELEMENT, rodPropertiesWidget);

    longitudinalForcePropertiesWidget = new LongitudinalForcePropertiesWidget();
    propertiesWidget->insertWidget(LONGITUDINAL_FORCE, longitudinalForcePropertiesWidget);

    sectionalForcePropertiesWidget = new SectionalForcePropertiesWidget();
    propertiesWidget->insertWidget(SECTIONAL_FORCE, sectionalForcePropertiesWidget);

    // up menu bar
    pushButtonCompute = new QPushButton(tr("Рассчитать"));

    // button menu
    pushButtonAdd = new QPushButton();
    pushButtonAdd->setIcon(QIcon(":icon/list-add.png"));
    pushButtonRemove = new QPushButton();
    pushButtonRemove->setIcon(QIcon(":icon/list-remove.png"));
    pushButtonUp = new QPushButton();
    pushButtonUp->setIcon(QIcon(":icon/go-up.png"));
    pushButtonDown = new QPushButton();
    pushButtonDown->setIcon(QIcon(":icon/go-down.png"));

//    auto pixmap = QIcon::fromTheme("document-save-as").pixmap(32, 32);
//    pixmap.save("document-save-as.png");

    // menu bar
    QMenu *menuFile = menuBar()->addMenu(tr("&Файл"));
    menuFile->addAction(tr("&Новый проект"), this, &MySAPRWindow::newProjectClicked,
                        QKeySequence(Qt::CTRL + Qt::Key_N));

    QMenu *menuOpen = menuFile->addMenu(tr("&Открыть..."));
    menuOpen->addAction(tr("Открыть проект"), this,
                        &MySAPRWindow::openProjectClicked,
                        QKeySequence(Qt::CTRL + Qt::Key_O));
    menuOpen->addAction(tr("Открыть расчет"), this,
                        &MySAPRWindow::openComputeResultClicked,
                        QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_O));

    menuFile->addAction(tr("&Сохранить"), this, &MySAPRWindow::saveProjectClicked,
                        QKeySequence(Qt::CTRL + Qt::Key_S));
    menuFile->addAction(tr("Сохранить &как..."), this, &MySAPRWindow::saveAsProjectClicked);
    menuFile->addSeparator();
    menuFile->addAction(tr("&Выход"), this, &MySAPRWindow::close,
                        QKeySequence(Qt::CTRL + Qt::Key_Q));

    QMenu* menuView = menuBar()->addMenu(tr("&Отображение"));
    menuView->addAction(tr("Показать номера &стержней"), graphicScene,
                        &ConstructionGraphicsScene::showRodLabels,
                        QKeySequence(Qt::CTRL + Qt::Key_R))->setCheckable(true);
    menuView->addAction(tr("Показать номера &узлов"), graphicScene,
                        &ConstructionGraphicsScene::showNodeLabels,
                        QKeySequence(Qt::CTRL + Qt::Key_T))->setCheckable(true);
    menuView->addAction(tr("Показать значения F нагрузок"), graphicScene,
                       &ConstructionGraphicsScene::showLongitudinalLabels,
                        QKeySequence(Qt::CTRL + Qt::Key_Y))->setCheckable(true);
    menuView->addAction(tr("Показать значения q нагрузок"), graphicScene,
                       &ConstructionGraphicsScene::showSectionalLabels,
                        QKeySequence(Qt::CTRL + Qt::Key_U))->setCheckable(true);
    menuView->addAction(tr("Показать длину стержней"), graphicScene,
                       &ConstructionGraphicsScene::showRodLengths,
                        QKeySequence(Qt::CTRL + Qt::Key_I))->setCheckable(true);
    menuView->addSeparator();
    menuView->addAction(tr("Нагрузки F к узлу"), graphicScene,
                       &ConstructionGraphicsScene::longitudinalforceToNode,
                        QKeySequence(Qt::CTRL + Qt::Key_B))->setCheckable(true);

    // layout setup
    QVBoxLayout *addButtonLayout = new QVBoxLayout();
    addButtonLayout->addSpacing(tabWidget->tabBar()->height());
    addButtonLayout->addWidget(pushButtonAdd);
    addButtonLayout->addWidget(pushButtonRemove);
    addButtonLayout->addWidget(pushButtonUp);
    addButtonLayout->addWidget(pushButtonDown);
    addButtonLayout->addStretch(1);

    QHBoxLayout *tabWidgetLayout = new QHBoxLayout();
    tabWidgetLayout->addWidget(tabWidget);
    tabWidgetLayout->addLayout(addButtonLayout);

    QHBoxLayout *upRightMenu = new QHBoxLayout();
    upRightMenu->addWidget(new QLabel(tr("Заделка")), 0);
    upRightMenu->addWidget(comboBoxFulcrum, 1);

    QVBoxLayout *rightMenu = new QVBoxLayout();
    rightMenu->addLayout(upRightMenu);
    rightMenu->addLayout(tabWidgetLayout);
    rightMenu->addWidget(labelProperties);
    rightMenu->addWidget(propertiesWidget);

    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addWidget(graphicView, 1);
    centerLayout->addLayout(rightMenu, 0);

    QHBoxLayout *upBarMenu = new QHBoxLayout();
    upBarMenu->addStretch(1);
    upBarMenu->addWidget(pushButtonCompute);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(upBarMenu);
    mainLayout->addLayout(centerLayout);

    QWidget *mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);

    // connections
    connect(pushButtonCompute, &QPushButton::clicked,
            this, &MySAPRWindow::computeButtonClicked);

    connect(comboBoxFulcrum, &QComboBox::currentTextChanged,
            this, &MySAPRWindow::comboBoxFulcrumChanged);

    connect(pushButtonAdd, &QPushButton::clicked,
            this, &MySAPRWindow::addButtonClicked);
    connect(pushButtonRemove, &QPushButton::clicked,
            this, &MySAPRWindow::removeButtonClicked);
    connect(pushButtonUp, &QPushButton::clicked,
            this, &MySAPRWindow::upButtonClicked);
    connect(pushButtonDown, &QPushButton::clicked,
            this, &MySAPRWindow::downButtonClicked);

    connect(tabWidget, &QTabWidget::currentChanged,
            this, &MySAPRWindow::tabWidgetSelected);

    connect(treeWidgetRods, &QTreeWidget::itemSelectionChanged,
            this, &MySAPRWindow::treeWidgetItemSelected);
    connect(treeWidgetLongitudinal, &QTreeWidget::itemSelectionChanged,
            this, &MySAPRWindow::treeWidgetItemSelected);
    connect(treeWidgetSectional, &QTreeWidget::itemSelectionChanged,
            this, &MySAPRWindow::treeWidgetItemSelected);

    connect(treeWidgetRods, &QTreeWidget::itemDoubleClicked,
            this, &MySAPRWindow::treeWidgetItemDoubleClicked);
    connect(treeWidgetLongitudinal, &QTreeWidget::itemDoubleClicked,
            this, &MySAPRWindow::treeWidgetItemDoubleClicked);
    connect(treeWidgetSectional, &QTreeWidget::itemDoubleClicked,
            this, &MySAPRWindow::treeWidgetItemDoubleClicked);

    connect(treeWidgetRods, &QTreeWidget::itemChanged,
            this, &MySAPRWindow::treeWidgetItemEdited);
    connect(treeWidgetLongitudinal, &QTreeWidget::itemChanged,
            this, &MySAPRWindow::treeWidgetItemEdited);
    connect(treeWidgetSectional, &QTreeWidget::itemChanged,
            this, &MySAPRWindow::treeWidgetItemEdited);

    connect(rodPropertiesWidget, &RodPropertiesWidget::rodChanged,
            this, &MySAPRWindow::requiresRedrawing);
    connect(longitudinalForcePropertiesWidget, &LongitudinalForcePropertiesWidget::forceChanged,
            this, &MySAPRWindow::requiresRedrawing);
    connect(sectionalForcePropertiesWidget, &SectionalForcePropertiesWidget::forceChanged,
            this, &MySAPRWindow::requiresRedrawing);
}

void MySAPRWindow::addButtonClicked()
{
    int type = tabWidget->currentIndex() + ROD_ELEMENT;

    switch (type) {
    case ROD_ELEMENT: {
        Rod rod;
        rod.setName(tr("Стержень_%1").arg(rodIdPool));
        rod.setId(rodIdPool++);
        rods.append(rod);

        addRodToScene(rod);
        break;
    }
    case LONGITUDINAL_FORCE: {
        LongitudinalForce force;
        force.setName(tr("Сосредоточенная_%1").arg(longitudinalIdPool));
        force.setId(longitudinalIdPool++);
        longitudinalForces.append(force);

        addLongitudinalForceToScene(force);
        checkLongitudinalForceErrors();
        break;
    }
    case SECTIONAL_FORCE: {
        SectionalForce force;
        force.setName(tr("Распределенная_%1").arg(sectionalIdPool));
        force.setId(sectionalIdPool++);
        sectionalForces.append(force);

        addSectionalForceToScene(force);
        checkSectionalForceErrors();
        break;
    }
    default:
        break;
    }
    updateButtons();
    hasChange = true;
}

void MySAPRWindow::removeButtonClicked()
{
    QTreeWidget *treeWidget = static_cast<QTreeWidget*>(tabWidget->currentWidget());

    QTreeWidgetItem *item = treeWidget->currentItem();
    if (item == nullptr)
        return;

    int index = treeWidget->currentIndex().row();
    int type = item->data(0, TYPE_ELEMENT).toInt();

    switch (type) {
    case ROD_ELEMENT:
        rods.remove(index);
        graphicScene->removeRod(index);
        break;
    case LONGITUDINAL_FORCE:
        graphicScene->removeLongitudinalForce(longitudinalForces.takeAt(index).id());
        break;
    case SECTIONAL_FORCE:
        graphicScene->removeSectionalForce(sectionalForces.takeAt(index).id());
        break;
    default:
        return;
    }
    delete treeWidget->takeTopLevelItem(index);
    changePropertiesTable(treeWidget, treeWidget->currentItem());
    updateButtons();
    hasChange = true;
}

void MySAPRWindow::upButtonClicked()
{
    QTreeWidget *treeWidget = static_cast<QTreeWidget*>(tabWidget->currentWidget());
    QTreeWidgetItem *currectItem = treeWidget->currentItem();

    if (currectItem == nullptr) {
        return;
    }

    int type = currectItem->data(0, TYPE_ELEMENT).toInt();
    int index = treeWidget->currentIndex().row();

    if (index <= 0) {
        return;
    }

    switch (type) {
    case ROD_ELEMENT:
        qSwap(rods[index], rods[index - 1]);
        graphicScene->swapRods(index, index - 1);
        break;
    case LONGITUDINAL_FORCE:
        qSwap(longitudinalForces[index], longitudinalForces[index - 1]);
        break;
    case SECTIONAL_FORCE:
        qSwap(sectionalForces[index], sectionalForces[index - 1]);
        break;
    }

    auto item = treeWidget->takeTopLevelItem(index);
    treeWidget->insertTopLevelItem(index - 1, item);
    treeWidget->setCurrentItem(item);
    hasChange = true;
}

void MySAPRWindow::downButtonClicked()
{
    QTreeWidget *treeWidget = static_cast<QTreeWidget*>(tabWidget->currentWidget());
    QTreeWidgetItem *currectItem = treeWidget->currentItem();

    if (currectItem == nullptr) {
        return;
    }

    int type = currectItem->data(0, TYPE_ELEMENT).toInt();
    int index = treeWidget->currentIndex().row();

    if (index == treeWidget->topLevelItemCount() - 1) {
        return;
    }

    switch (type) {
    case ROD_ELEMENT:
        qSwap(rods[index], rods[index + 1]);
        graphicScene->swapRods(index, index + 1);
        break;
    case LONGITUDINAL_FORCE:
        qSwap(longitudinalForces[index], longitudinalForces[index + 1]);
        break;
    case SECTIONAL_FORCE:
        qSwap(sectionalForces[index], sectionalForces[index + 1]);
        break;
    }

    auto item = treeWidget->takeTopLevelItem(index);
    treeWidget->insertTopLevelItem(index + 1, item);
    treeWidget->setCurrentItem(item);
    hasChange = true;
}

bool MySAPRWindow::newProjectClicked()
{
    if (hasChange) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Проект изменен."));
        msgBox.setIcon(QMessageBox::Icon::Question);
        msgBox.setText(tr("Вы хотите сохранить внесенные изменения?"));
        msgBox.setInformativeText(tr("Ваши изменения будут потеряны, если вы не сохраните их."));
        msgBox.addButton(tr("Сохранить"), QMessageBox::AcceptRole);
        msgBox.addButton(tr("Отмена"), QMessageBox::RejectRole);
        msgBox.addButton(tr("Не сохранять"), QMessageBox::DestructiveRole);

        switch(msgBox.exec()) {
        case QMessageBox::AcceptRole:
            if (!saveProjectClicked()) {
                return false;
            }
            break;
        case QMessageBox::DestructiveRole:
            break;
        default:
            return false;
        }
    }

    clearAll();
    hasChange = false;
    projectFileInfo.setFile("");
    setWindowTitle(qAppName());
    return true;
}

bool MySAPRWindow::saveProjectClicked()
{
    if (projectFileInfo.filePath().isEmpty()) {
        return saveAsProjectClicked();
    }

    QJsonArray rodArray;
    for (auto& rod : rods) {
        rodArray.append(rod.save());
    }
    QJsonArray sectionalForcesArray;
    for (auto& force : sectionalForces) {
        sectionalForcesArray.append(force.save());
    }
    QJsonArray longitudinalForcesArray;
    for (auto& force : longitudinalForces) {
        longitudinalForcesArray.append(force.save());
    }

    QJsonObject mainObj;
    mainObj["rods"] = rodArray;
    mainObj["sectionalForces"] = sectionalForcesArray;
    mainObj["longitudinalForces"] = longitudinalForcesArray;
    mainObj["pillars"] = comboBoxFulcrum->currentData().toInt();

    QJsonDocument doc(mainObj);

    QFile file(projectFileInfo.filePath());
    if (!file.open(QIODevice::WriteOnly)) {
        // qWarning() << "Couldn't save project file";
        QMessageBox::critical(this, tr("Ошибка при сохранении"),
                              tr("Не удалось открыть файл на запись."));
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    projectFileInfo.setFile(file);
    setWindowTitle(qAppName() + QString(" [%1]").arg(projectFileInfo.absoluteFilePath()));
    statusBar()->showMessage(tr("Файл сохранён: ") + projectFileInfo.absoluteFilePath(), 3000);
    hasChange = false;

    return true;
}

bool MySAPRWindow::saveAsProjectClicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Сохранение проекта"), ".",
                                                "JSON files (*.json) ;; All files (*.*)");
    if (path.isEmpty())
        return false;

    const QString SUFFIX = ".json";
    if (!path.endsWith(SUFFIX))
        path += SUFFIX;

    projectFileInfo.setFile(path);
    return saveProjectClicked();
}

bool MySAPRWindow::openProjectClicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Открытие проекта"), ".",
                                                "JSON files (*.json) ;; All files (*.*)");
    if (path.isEmpty())
        return false;

    if (!newProjectClicked()) {
        return false;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        // qWarning() << "Couldn't open project file";
        QMessageBox::critical(this, tr("Ошибка при открытии"),
                              tr("Не удалось открыть файл на чтение"));
        return false;
    }

    try {
        QByteArray saveData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(saveData);

        QJsonObject mainObj = doc.object();
        QJsonArray rodArray = mainObj["rods"].toArray();
        for (auto it = rodArray.begin(); it != rodArray.end(); ++it) {
            Rod rod;
            rod.load(it->toObject());
            rod.setId(rodIdPool++);
            rods.append(rod);
            addRodToScene(rod);
        }

        QJsonArray sectionalForcesArray = mainObj["sectionalForces"].toArray();
        for (auto it = sectionalForcesArray.begin(); it != sectionalForcesArray.end(); ++it) {
            SectionalForce force;
            force.load(it->toObject());
            force.setId(sectionalIdPool++);
            sectionalForces.append(force);
            addSectionalForceToScene(force);
        }

        QJsonArray longitudinalForcesArray = mainObj["longitudinalForces"].toArray();
        for (auto it = longitudinalForcesArray.begin(); it != longitudinalForcesArray.end(); ++it) {
            LongitudinalForce force;
            force.load(it->toObject());
            force.setId(longitudinalIdPool++);
            longitudinalForces.append(force);
            addLongitudinalForceToScene(force);
        }

        int value = mainObj["pillars"].toInt();
        for (int i = 0; i < comboBoxFulcrum->count(); ++i) {
            if (value == comboBoxFulcrum->itemData(i).toInt()) {
                comboBoxFulcrum->setCurrentIndex(i);
                graphicScene->setPillarSetup(static_cast<ConstructionGraphicsScene::PillarSetup>(value));
            }
        }

        projectFileInfo.setFile(file);
    } catch (QJsonParseError &ex) {
        QMessageBox::critical(this, tr("Ошибка при загрузки проекта."),
                              tr("Не удалось прочитать файл проекта.")
                              + "\n\n"
                              + ex.errorString());
        return false;
    }
    setWindowTitle(qAppName() + QString(" [%1]").arg(projectFileInfo.absoluteFilePath()));
    statusBar()->showMessage(tr("Файл открыт: ") + projectFileInfo.absoluteFilePath(), 3000);

    checkLongitudinalForceErrors();
    checkSectionalForceErrors();
    return true;
}

bool MySAPRWindow::openComputeResultClicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Открытие расчета"), ".",
                                                "JSON files (*.json) ;; All files (*.*)");
    if (path.isEmpty())
        return false;

    try {
        QSharedPointer<Processor> proc = QSharedPointer<Processor>::create();
        proc->load(path);

        PostProcessorWindow *postProcessor = new PostProcessorWindow(proc, graphicScene, this);
        postProcessor->exec();
    } catch (...) {
        QMessageBox::critical(this, tr("Ошибка при открытии"),
                              tr("Не удалось открыть файл расчетов."));
        return false;
    }
    return true;
}

void MySAPRWindow::treeWidgetItemSelected()
{
    QTreeWidget *treeWidget = static_cast<QTreeWidget*>(tabWidget->currentWidget());
    QTreeWidgetItem *item = treeWidget->currentItem();
    changePropertiesTable(treeWidget, item);
}

void MySAPRWindow::treeWidgetItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QTreeWidget *treeWidget = static_cast<QTreeWidget*>(tabWidget->currentWidget());
    treeWidget->editItem(item, column);
}

void MySAPRWindow::treeWidgetItemEdited(QTreeWidgetItem *item, int column)
{
    int type = item->data(column, TYPE_ELEMENT).toInt();
    int index = static_cast<QTreeWidget*>(tabWidget->currentWidget())->currentIndex().row();

    if (index == -1) {
        return;
    }

    switch (type) {
    case ROD_ELEMENT:
        rods[index].setName(item->text(column));
        break;
    case LONGITUDINAL_FORCE:
        if (!isLongitudinalChecked) {
            longitudinalForces[index].setName(item->text(column));
            checkLongitudinalForceErrors();
        }
        break;
    case SECTIONAL_FORCE:
        if (!isSectionalChecked) {
            sectionalForces[index].setName(item->text(column));
            checkSectionalForceErrors();
        }
        break;
    }
}

void MySAPRWindow::tabWidgetSelected()
{
    QTreeWidget *treeWidget = static_cast<QTreeWidget*>(tabWidget->currentWidget());
    updateButtons();
    QTreeWidgetItem *item = treeWidget->currentItem();
    changePropertiesTable(treeWidget, item);
}

void MySAPRWindow::requiresRedrawing()
{
    QTreeWidget *treeWidget = static_cast<QTreeWidget*>(tabWidget->currentWidget());
    QTreeWidgetItem *item = treeWidget->currentItem();
    if (item == nullptr)
        return;

    int index = treeWidget->currentIndex().row();
    int type = item->data(0, TYPE_ELEMENT).toInt();

    hasChange = true;

    switch (type) {
    case ROD_ELEMENT: {
        auto& rod = rods[index];
        graphicScene->updateRod(index, rod.length(), rod.sectionalArea());
        break;
    }
    case LONGITUDINAL_FORCE: {
        auto& force = longitudinalForces[index];
        graphicScene->updateLongitudinalForce(force.id(), force.nodeId(), force.force());
        checkLongitudinalForceErrors();
        break;
    }
    case SECTIONAL_FORCE: {
        auto& force = sectionalForces[index];
        graphicScene->updateSectionalForce(force.id(), force.rodId(), force.force());
        checkSectionalForceErrors();
        break;
    }
    default:
        break;
    }
}

void MySAPRWindow::comboBoxFulcrumChanged()
{
    auto value = static_cast<ConstructionGraphicsScene::PillarSetup>(comboBoxFulcrum->currentData().toInt());
    graphicScene->setPillarSetup(value);

    checkLongitudinalForceErrors();
    updateButtons();
}

void MySAPRWindow::computeButtonClicked()
{
    if (rods.isEmpty()) {
        QMessageBox::warning(this,
                             tr("Ошибка начала расчёта"),
                             tr("Невозможно начать расчет, т.к. не задано не одного стержня."));
        return;
    }

    checkLongitudinalForceErrors();
    checkSectionalForceErrors();

    if (!longitudinalErrors.isEmpty() || !sectionalErrors.isEmpty()) {
        QString info;
        for (auto &el : longitudinalErrors) {
            info += el + "\n";
        }
        for (auto &el : sectionalErrors) {
            info += el + "\n";
        }
        QMessageBox::critical(this,
                              tr("Ошибка начала расчёта"),
                              info);
        return;
    }

    auto value = static_cast<ConstructionGraphicsScene::PillarSetup>(comboBoxFulcrum->currentData().toInt());
    QVector<bool> pillars(rods.size() + 1);
    pillars.first() = (value != ConstructionGraphicsScene::Right);
    pillars.last() = (value != ConstructionGraphicsScene::Left);

    QSharedPointer<Processor> proc = QSharedPointer<Processor>::create();
    proc->compute(rods, longitudinalForces, sectionalForces, pillars);

    PostProcessorWindow *postProcessor = new PostProcessorWindow(proc, graphicScene, this);
    postProcessor->exec();
}

void MySAPRWindow::resizeEvent(QResizeEvent *event)
{
    QSize size = graphicView->size() * 0.9;
    graphicScene->updateSize(size.width(), size.height());

    QMainWindow::resizeEvent(event);
}

void MySAPRWindow::updateButtons()
{
    switch (tabWidget->currentIndex()) {
    case 0: // rods
        pushButtonAdd->setEnabled(true);
        break;
    case 1: // F
        if (rods.size() + 1 - pillarCount() > treeWidgetLongitudinal->topLevelItemCount()) {
            pushButtonAdd->setEnabled(true);
        } else {
            pushButtonAdd->setEnabled(false);
        }
        break;
    case 2: // q
        if (rods.size() > treeWidgetSectional->topLevelItemCount()) {
            pushButtonAdd->setEnabled(true);
        } else {
            pushButtonAdd->setEnabled(false);
        }
        break;
    }
}

int MySAPRWindow::pillarCount()
{
    auto value = static_cast<ConstructionGraphicsScene::PillarSetup>(comboBoxFulcrum->currentData().toInt());

    switch (value) {
    case ConstructionGraphicsScene::Left:
        return 1;
    case ConstructionGraphicsScene::Right:
        return 1;
    case ConstructionGraphicsScene::LeftAndRight:
        return 2;
    }

    return 0;
}

void MySAPRWindow::changePropertiesTable(QTreeWidget *treeWidget, QTreeWidgetItem *item)
{
    if (item == nullptr) {
        propertiesWidget->setCurrentIndex(NOTHING_ELEMENT);
        labelProperties->setVisible(false);
        return;
    }

    labelProperties->setVisible(true);
    int index = treeWidget->currentIndex().row();
    int type = item->data(0, TYPE_ELEMENT).toInt();

    // qDebug() << "Bind " << index;

    switch (type) {
    case ROD_ELEMENT:
        if (index >= rods.size())
            break;

        propertiesWidget->setCurrentIndex(ROD_ELEMENT);
        rodPropertiesWidget->bind(index, &rods);
        return;
    case LONGITUDINAL_FORCE:
        if (index >= longitudinalForces.size())
            break;

        propertiesWidget->setCurrentIndex(LONGITUDINAL_FORCE);
        longitudinalForcePropertiesWidget->bind(index, &longitudinalForces);
        return;
    case SECTIONAL_FORCE:
        if (index >= sectionalForces.size())
            break;

        propertiesWidget->setCurrentIndex(SECTIONAL_FORCE);
        sectionalForcePropertiesWidget->bind(index, &sectionalForces);
        return;
    }
    propertiesWidget->setCurrentIndex(NOTHING_ELEMENT);
    labelProperties->setVisible(false);
}

void MySAPRWindow::checkLongitudinalForceErrors()
{
    isLongitudinalChecked = true;
    longitudinalErrors.clear();

    auto value = static_cast<ConstructionGraphicsScene::PillarSetup>(comboBoxFulcrum->currentData().toInt());
    const int min = value != ConstructionGraphicsScene::Right ? 1 : 0;
    const int max = rods.size() + 1 - (value != ConstructionGraphicsScene::Left ? 1 : 0);
    const QColor errorColor(220, 0, 0);
    const QColor defaultColor(Qt::black);

    // qDebug() << "Min " << min << " max " << max;

    for (int i = 0; i < longitudinalForces.size(); ++i) {
        for (int j = i + 1; j < longitudinalForces.size(); ++j) {
            if (longitudinalForces[i].nodeId() == longitudinalForces[j].nodeId()) {
                longitudinalErrors[i] += tr("Сосредоточенные нагрузки \"%1\" и \"%2\" приложены к одному узлу. ")
                        .arg(longitudinalForces[i].name()).arg(longitudinalForces[j].name());
                longitudinalErrors[j] += tr("Сосредоточенные нагрузки \"%1\" и \"%2\" приложены к одному узлу. ")
                        .arg(longitudinalForces[j].name()).arg(longitudinalForces[i].name());

                QTreeWidgetItem* item1 = treeWidgetLongitudinal->topLevelItem(i);
                item1->setToolTip(0, longitudinalErrors[i]);
                item1->setForeground(0, errorColor);

                QTreeWidgetItem* item2 = treeWidgetLongitudinal->topLevelItem(j);
                item2->setToolTip(0, longitudinalErrors[j]);
                item2->setForeground(0, errorColor);
            }
        }

        if (longitudinalForces[i].nodeId() <= min || longitudinalForces[i].nodeId() > max) {
            longitudinalErrors[i] += tr("Сосредоточенную нагрузку \"%1\" невозможно приложить к узлу \"%2\". ")
                    .arg(longitudinalForces[i].name()).arg(QString::number(longitudinalForces[i].nodeId()));

            QTreeWidgetItem* item = treeWidgetLongitudinal->topLevelItem(i);
            item->setToolTip(0, longitudinalErrors[i]);
            item->setForeground(0, errorColor);
        }

        if (longitudinalErrors.isEmpty()) {
            QTreeWidgetItem* item = treeWidgetLongitudinal->topLevelItem(i);
            item->setToolTip(0, "");
            item->setForeground(0, defaultColor);
        }
    }
    isLongitudinalChecked = false;
}

void MySAPRWindow::checkSectionalForceErrors()
{
    isSectionalChecked = true;
    sectionalErrors.clear();

    const QColor errorColor(220, 0, 0);
    const QColor defaultColor(Qt::black);

    for (int i = 0; i < sectionalForces.size(); ++i) {
        for (int j = i + 1; j < sectionalForces.size(); ++j) {
            if (sectionalForces[i].rodId() == sectionalForces[j].rodId()) {
                sectionalErrors[i] += tr("Распределенные нагрузки \"%1\" и \"%2\" приложены к одному стержню. ")
                        .arg(sectionalForces[i].name()).arg(sectionalForces[j].name());
                sectionalErrors[j] += tr("Распределенные нагрузки \"%1\" и \"%2\" приложены к одному стержню. ")
                        .arg(sectionalForces[j].name()).arg(sectionalForces[i].name());

                QTreeWidgetItem* item1 = treeWidgetSectional->topLevelItem(i);
                item1->setToolTip(0, sectionalErrors[i]);
                item1->setForeground(0, errorColor);

                QTreeWidgetItem* item2 = treeWidgetSectional->topLevelItem(j);
                item2->setToolTip(0, sectionalErrors[j]);
                item2->setForeground(0, errorColor);
            }
        }

        if (sectionalForces[i].rodId() < 0 || sectionalForces[i].rodId() > rods.size()) {
            sectionalErrors[i] += tr("Распределенную нагрузку \"%1\" невозможно приложить к стержню \"%2\". ")
                    .arg(sectionalForces[i].name()).arg(QString::number(sectionalForces[i].rodId()));

            QTreeWidgetItem* item = treeWidgetSectional->topLevelItem(i);
            item->setToolTip(0, sectionalErrors[i]);
            item->setForeground(0, errorColor);
        }

        if (sectionalErrors.isEmpty()) {
            QTreeWidgetItem* item = treeWidgetSectional->topLevelItem(i);
            item->setToolTip(0, "");
            item->setForeground(0, defaultColor);
        }
    }
    isSectionalChecked = false;
}

void MySAPRWindow::addRodToScene(Rod &rod)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, rod.name());
    item->setData(0, TYPE_ELEMENT, ROD_ELEMENT);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    treeWidgetRods->addTopLevelItem(item);

    graphicScene->appendRod(rod.length(), rod.sectionalArea());
}

void MySAPRWindow::addLongitudinalForceToScene(LongitudinalForce &force)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, force.name());
    item->setData(0, TYPE_ELEMENT, LONGITUDINAL_FORCE);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    treeWidgetLongitudinal->addTopLevelItem(item);

    graphicScene->appendLongitudinalForce(force.id(), force.nodeId(), force.force());
}

void MySAPRWindow::addSectionalForceToScene(SectionalForce &force)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, force.name());
    item->setData(0, TYPE_ELEMENT, SECTIONAL_FORCE);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    treeWidgetSectional->addTopLevelItem(item);

    graphicScene->appendSectionalForce(force.id(), force.rodId(), force.force());
}

void MySAPRWindow::clearAll()
{
    rodIdPool = 1;
    longitudinalIdPool = 1;
    sectionalIdPool = 1;

    comboBoxFulcrum->setCurrentIndex(0);

    rods.clear();
    sectionalForces.clear();
    longitudinalForces.clear();

    treeWidgetRods->clear();
    treeWidgetSectional->clear();
    treeWidgetLongitudinal->clear();

    graphicScene->clearAll();
    changePropertiesTable(nullptr, nullptr);
}
