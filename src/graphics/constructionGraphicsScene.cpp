#include "constructionGraphicsScene.h"

ConstructionGraphicsScene::ConstructionGraphicsScene(QObject *parent)
    : QGraphicsScene(parent),
      MIN_DRAW_STEP(40),
      displayRodNumbers_(false),
      displayNodeNumbers_(false),
      displayLongitudinalLabels_(false),
      displaySectionalLabels_(false),
      displyRodLengths_(false),
      isForceToNode_(false)
{
    leftPillar = new PillarGraphicsItem(MIN_DRAW_STEP, PillarGraphicsItem::Left);
    leftPillar->setVisible(false);

    rightPillar = new PillarGraphicsItem(MIN_DRAW_STEP, PillarGraphicsItem::Right);
    rightPillar->setVisible(false);

    pillarSetup = PillarSetup::Left;
    addItem(leftPillar);
    addItem(rightPillar);
}

void ConstructionGraphicsScene::appendRod(double length, double sectionalArea)
{
    auto *rodItem = new RodGraphicsItem(MIN_DRAW_STEP, length, sectionalArea);
    rods.append(rodItem);
    addItem(rodItem);
    updateDrawing();
}

void ConstructionGraphicsScene::updateRod(int index, double length, double sectionalArea)
{
    rods[index]->updateValues(length, sectionalArea);
    updateDrawing();
}

void ConstructionGraphicsScene::swapRods(int first, int second)
{
    qSwap(rods[first], rods[second]);
    updateDrawing();
}

void ConstructionGraphicsScene::removeRod(int index)
{
    removeItem(rods.takeAt(index));
    updateDrawing();
}

void ConstructionGraphicsScene::appendLongitudinalForce(size_t id, int nodeId, double force)
{
    auto *item = new LongitudinalForceGraphicsItem(MIN_DRAW_STEP, nodeId, force);
    longitudinalForces[id] = item;
    addItem(item);
    updateDrawing();
}

void ConstructionGraphicsScene::updateLongitudinalForce(size_t id, int nodeId, double force)
{
    longitudinalForces[id]->updateValues(nodeId, force);
    updateDrawing();
}

void ConstructionGraphicsScene::removeLongitudinalForce(size_t id)
{
    removeItem(longitudinalForces.take(id));
    updateDrawing();
}

void ConstructionGraphicsScene::appendSectionalForce(size_t id, int rodId, double force)
{
    auto *item = new SectionalForceGraphicsItem(MIN_DRAW_STEP, rodId, force);
    sectionalForces[id] = item;
    addItem(item);
    updateDrawing();
}

void ConstructionGraphicsScene::updateSectionalForce(size_t id, int rodId, double force)
{
    // qDebug() << id << "\t" << rodId << "\t" << force;
    sectionalForces[id]->updateValues(rodId, force);
    updateDrawing();
}

void ConstructionGraphicsScene::removeSectionalForce(size_t id)
{
    removeItem(sectionalForces.take(id));
    updateDrawing();
}

void ConstructionGraphicsScene::updateSize(int width, int height)
{
    setSceneRect(0, 0, width, height);
    updateDrawing();
}


void ConstructionGraphicsScene::updateDrawing()
{
    QVector<QGraphicsItem *> forceItems;
    for (auto &el : sectionalForces)
        forceItems.append(el);

    for (auto &el : longitudinalForces)
        forceItems.append(el);

    // если нечего отображать
    if (rods.empty()) {
        for (auto &el : rods)
            forceItems.append(el);

        for (auto &el : forceItems)
            el->setVisible(false);

        for (auto &el : rodNumbers)
            el->setVisible(false);

        for (auto &el : nodeNumbers)
            el->setVisible(false);

        for (auto &el : separateLines)
            el->setVisible(false);

        leftPillar->setVisible(false);
        rightPillar->setVisible(false);

        return;
    }

    // рассчитать размеры для конструкции
    auto replyWidth = computeWidth(sceneRect().width() * 0.9);
    QVector<double> resWidth = replyWidth.first;
    double commonWidth = replyWidth.second;

    auto replyHeight = computeHeight(sceneRect().height() * 0.9);
    QVector<double> resHight = replyHeight.first;
    double maxHeight = replyHeight.second;

    QPointF center = sceneRect().center();

    // обновить отрисовку
    double x = center.x() - commonWidth / 2;
    double y = center.y();

    QGraphicsView* view = views().first();
    double fitInViewWidth = commonWidth + 4 * MIN_DRAW_STEP;
    if (fitInViewWidth >= sceneRect().width())
        fitInViewWidth = sceneRect().width();

    // два MIN_DRAW_STEP запасом
    view->fitInView(x - 2 * MIN_DRAW_STEP,
                    y - maxHeight / 2 - 2 * MIN_DRAW_STEP,
                    fitInViewWidth,
                    maxHeight + 4 * MIN_DRAW_STEP,
                    Qt::KeepAspectRatio);

    // начало
    if (!rods.isEmpty() && (pillarSetup != PillarSetup::Right)) {
        leftPillar->setHeight(resHight.first());
        leftPillar->setX(x - MIN_DRAW_STEP);
        leftPillar->setY(y - leftPillar->height() / 2 - leftPillar->extra());
        leftPillar->setVisible(true);
    } else {
        leftPillar->setVisible(false);
    }

    for (int i = 0; i < rods.size() + 1; ++i) {
        // отметка узла
        if (displayNodeNumbers_) {
            if (i >= nodeNumbers.size()) {
                nodeNumbers.append(new NodeNumberGraphicsItem());
                nodeNumbers.last()->setZValue(1);
                addItem(nodeNumbers.last());
            }
            nodeNumbers[i]->setSize(MIN_DRAW_STEP / 2);
            nodeNumbers[i]->setNumber(i + 1);
            nodeNumbers[i]->setX(x - nodeNumbers[i]->size() / 2);
            nodeNumbers[i]->setY(y + maxHeight / 2 + nodeNumbers[i]->size() - MIN_DRAW_STEP / 3);
            nodeNumbers[i]->setVisible(true);
        }

        // отметка линий
        if (displyRodLengths_) {
            if (i >= separateLines.size()) {
                separateLines.append(new QGraphicsLineItem());
                addItem(separateLines.last());
            }
            separateLines[i]->setLine(x, y, x, y + maxHeight / 2 + MIN_DRAW_STEP * 1.2);
            separateLines[i]->setVisible(true);
        }

        // сосредаточенная нагрузка
        for (auto &item : longitudinalForces) {
            if (item->nodeId() == i + 1) {
                if (isForceToNode_) {
                    item->setX(x + (item->value() > 0 ? -MIN_DRAW_STEP : 0));
                } else {
                    item->setX(x + (item->value() < 0 ? -MIN_DRAW_STEP : 0));
                }
                item->setY(y - MIN_DRAW_STEP / 2);
                item->showLabel(displayLongitudinalLabels_);
                item->setVisible(true);
                forceItems.removeOne(item);
                break;
            }
        }

        // стержень
        if (i < rods.size()) {
            rods[i]->setWidth(resWidth[i]);
            rods[i]->setHeight(resHight[i]);
            rods[i]->setX(x);
            rods[i]->setY(y - rods[i]->height() / 2);

            // отметка стержня
            if (displayRodNumbers_) {
                if (i >= rodNumbers.size()) {
                    rodNumbers.append(new RodNumberGraphicsItem());
                    rodNumbers.last()->setZValue(1);
                    addItem(rodNumbers.last());
                }
                rodNumbers[i]->setSize(MIN_DRAW_STEP / 2);
                rodNumbers[i]->setNumber(i + 1);
                rodNumbers[i]->setX(x + rods[i]->width() / 2 - rodNumbers[i]->size() / 2);
                rodNumbers[i]->setY(y - maxHeight / 2 - rodNumbers[i]->size() - MIN_DRAW_STEP / 3);
                rodNumbers[i]->setVisible(true);
            }

            // отметка длины стержня
            if (displyRodLengths_) {
                if (i >= rodLengths.size()) {
                    rodLengths.append(new RodLengthGraphicsItem());
                    addItem(rodLengths.last());
                }
                rodLengths[i]->setHeight(MIN_DRAW_STEP);
                rodLengths[i]->setWidth(rods[i]->width());
                rodLengths[i]->setValue(rods[i]->length());
                rodLengths[i]->setX(x);
                rodLengths[i]->setY(y + maxHeight / 2 + MIN_DRAW_STEP / 3);
                rodLengths[i]->setVisible(true);
            }

            // распределенная нагрузка
            for (auto &item : sectionalForces) {
                if (item->rodId() == i + 1) {
                    item->setX(x);
                    item->setY(y - MIN_DRAW_STEP / 2);
                    item->setWidth(rods[i]->width());
                    item->showLabel(displaySectionalLabels_);
                    item->setVisible(true);
                    forceItems.removeOne(item);
                    break;
                }
            }

            x += rods[i]->width();
        }
    }

    for (int i = rods.size(); i < rodNumbers.size(); ++i)
        rodNumbers[i]->setVisible(false);

    for (int i = rods.size(); i < rodLengths.size(); ++i)
        rodLengths[i]->setVisible(false);

    for (int i = rods.size() + 1; i < nodeNumbers.size(); ++i)
        nodeNumbers[i]->setVisible(false);

    for (int i = rods.size() + 1; i < separateLines.size(); ++i)
        separateLines[i]->setVisible(false);

    // конец
    if (!rods.isEmpty() && (pillarSetup != PillarSetup::Left)) {
        rightPillar->setHeight(resHight.last());
        rightPillar->setX(x);
        rightPillar->setY(y - rightPillar->height() / 2 - rightPillar->extra());
        rightPillar->setVisible(true);
    } else {
        rightPillar->setVisible(false);
    }

    // скрыть отставшиеся нагрузки
    for (auto &el: forceItems)
        el->setVisible(false);
}

bool ConstructionGraphicsScene::isForceToNode() const
{
    return isForceToNode_;
}

bool ConstructionGraphicsScene::displyRodLengths() const
{
    return displyRodLengths_;
}

bool ConstructionGraphicsScene::displaySectionalLabels() const
{
    return displaySectionalLabels_;
}

bool ConstructionGraphicsScene::displayLongitudinalLabels() const
{
    return displayLongitudinalLabels_;
}

bool ConstructionGraphicsScene::displayNodeNumbers() const
{
    return displayNodeNumbers_;
}

bool ConstructionGraphicsScene::displayRodNumbers() const
{
    return displayRodNumbers_;
}

void ConstructionGraphicsScene::setPillarSetup(const PillarSetup &pillarSetup)
{
    this->pillarSetup = pillarSetup;
    updateDrawing();
}

void ConstructionGraphicsScene::clearAll()
{
    for (auto &el : rods)
        removeItem(el);
    rods.clear();

    for (auto &el : sectionalForces)
        removeItem(el);
    sectionalForces.clear();

    for (auto &el : longitudinalForces)
        removeItem(el);
    longitudinalForces.clear();

    for (auto &el : rodNumbers)
        removeItem(el);
    rodNumbers.clear();

    for (auto &el : rodLengths)
        removeItem(el);
    rodLengths.clear();

    for (auto &el : nodeNumbers)
        removeItem(el);
    nodeNumbers.clear();

    for (auto &el : separateLines)
        removeItem(el);
    separateLines.clear();

    updateDrawing();
}

void ConstructionGraphicsScene::showRodLabels(bool show)
{
    displayRodNumbers_ = show;
    if (!show) {
        for (auto &el : rodNumbers) {
            el->setVisible(false);
        }
    }
    updateDrawing();
}

void ConstructionGraphicsScene::showNodeLabels(bool show)
{
    displayNodeNumbers_ = show;
    if (!show) {
        for (auto &el : nodeNumbers) {
            el->setVisible(false);
        }
    }
    updateDrawing();
}

void ConstructionGraphicsScene::showLongitudinalLabels(bool show)
{
    displayLongitudinalLabels_ = show;
    updateDrawing();
}

void ConstructionGraphicsScene::showSectionalLabels(bool show)
{
    displaySectionalLabels_ = show;
    updateDrawing();
}

void ConstructionGraphicsScene::showRodLengths(bool show)
{
    displyRodLengths_ = show;
    updateDrawing();
}

void ConstructionGraphicsScene::longitudinalforceToNode(bool forceToNode)
{
    isForceToNode_ = forceToNode;
    updateDrawing();
}

QPair<QVector<double>, double> ConstructionGraphicsScene::computeWidth(const double maxLength)
{
    // подготовка
    QVector<double> values;
    double min = rods.first()->length();
    for (auto &el : rods) {
        values.append(el->length());
        if (el->length() < min) {
            min = el->length();
        }
    }

    // нормализация
    QVector<double> result;
    double commonLengths = 0;
    double valueCount = 0;

    const double drawStep = 1.75 * MIN_DRAW_STEP;
    for (int i = 0; i < rods.size(); ++i) {
        if (min >= 1) {
            result.append(values[i] * drawStep);
            commonLengths += values[i] * drawStep;
            values[i] /= min;
        } else {
            values[i] /= min;
            result.append(values[i] * drawStep);
            commonLengths += values[i] * drawStep;
        }
        valueCount += values[i];
    }

    // перерасчет

    // values - относительные части конструкции
    // valueCount - сумма этих частей

    QVector<bool> check(result.size(), false);
    int checked = 0;

    double unallocatedLength = maxLength;
    while ((commonLengths - maxLength) >= 1 && (checked != result.size())) {
        int diffCount = 0;
        double diffLength = 0;
        commonLengths = 0;

        for (int i = 0; i < result.size(); ++i) {
            if (!check[i]) {
                result[i] = values[i] * unallocatedLength / valueCount;
                if (result[i] < MIN_DRAW_STEP) {
                    result[i] = MIN_DRAW_STEP;

                    diffLength += result[i];
                    diffCount += values[i];

                    check[i] = true;
                    ++checked;
                }
            }
            commonLengths += result[i];
        }
        valueCount -= diffCount;
        unallocatedLength -= diffLength;
    }

    return {result, commonLengths};
}

QPair<QVector<double>, double> ConstructionGraphicsScene::computeHeight(const double maxLenght)
{
    // подготовка
    QVector<double> values;
    double min = rods.first()->sectionalArea();
    for (auto &el : rods) {
        values.append(el->sectionalArea());
        if (el->sectionalArea() < min) {
            min = el->sectionalArea();
        }
    }

    // нормализация
    QVector<double> result;
    double valueCount = 0;
    double max = 0;

    const double drawStep = 1.25 * MIN_DRAW_STEP;
    for (int i = 0; i < rods.size(); ++i) {
        if (min >= 1) {
            result.append(values[i] * drawStep);
            values[i] /= min;
        } else {
            values[i] /= min;
            result.append(values[i] * drawStep);
        }
        if (max < result.last()) {
            max = result.last();
        }
        valueCount += values[i];
    }

    // values - относительные части конструкции
    // valueCount - сумма этих частей

    if (max > maxLenght) {
        max = 0;
        for (int i = 0; i < result.size(); ++i) {
            result[i] = values[i] * maxLenght / valueCount;
            if (result[i] < MIN_DRAW_STEP) {
                result[i] = MIN_DRAW_STEP;
            }

            if (max < result[i]) {
                max = result[i];
            }
        }
    }

    return {result, max};
}

