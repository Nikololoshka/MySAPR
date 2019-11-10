#include "constructionPlotGraphicsScene.h"

ConstructionPlotGraphicsScene::ConstructionPlotGraphicsScene(QSharedPointer<Processor> &processor)
    : ConstructionGraphicsScene(),
      PLOTS_HEIGHT(400)
{
    processor_ = processor;
}

void ConstructionPlotGraphicsScene::addPlot(const QString &label, CalcMethod method)
{
    PlotValuesGraphicsItem *item = new PlotValuesGraphicsItem(MIN_DRAW_STEP);
    item->setValues(processor_, method);
    item->setLabel(label);
    addItem(item);
    plots_.append(item);
}

void ConstructionPlotGraphicsScene::updateSize(int width, int height)
{
    setSceneRect(0, 0, width, height + PLOTS_HEIGHT);
    updateDrawing();
}

void ConstructionPlotGraphicsScene::updateDrawing()
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

    auto replyHeight = computeHeight((sceneRect().height() - PLOTS_HEIGHT) / 2 * 0.9 );
    QVector<double> resHight = replyHeight.first;
    double maxHeight = replyHeight.second;

    QPointF center = sceneRect().center();
    center.setY(maxHeight / 2 + 2 * MIN_DRAW_STEP);

    // обновить отрисовку
    double x = center.x() - commonWidth / 2;
    double y = center.y();

    // отрисовка эпюров
    double plotHeight = PLOTS_HEIGHT * 0.9 / plots_.size();
    for (int i = 0; i < plots_.size(); ++i) {
        plots_[i]->setWidth(commonWidth + 2 * MIN_DRAW_STEP);
        plots_[i]->setHeight(plotHeight);
        plots_[i]->setPartLenghts(resWidth);
        plots_[i]->setX(x - MIN_DRAW_STEP);
        plots_[i]->setY(y + maxHeight / 2 + MIN_DRAW_STEP * 1.2 +   // координата отображения длины
                        plotHeight * i);                            // + высота эпюра
    }

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
        if (i >= separateLines.size()) {
            separateLines.append(new QGraphicsLineItem());
            addItem(separateLines.last());
        }
        separateLines[i]->setLine(x, y, x, y + maxHeight / 2 +
                                  MIN_DRAW_STEP * 1.2 + plotHeight * plots_.size());
        separateLines[i]->setVisible(true);

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
