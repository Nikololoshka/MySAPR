#ifndef CONSTRUCTIONPLOTGRAPHICSSCENE_H
#define CONSTRUCTIONPLOTGRAPHICSSCENE_H

#include <QtWidgets>
#include "constructionGraphicsScene.h"
#include "plotValuesGraphicsItem.h"
#include "../processor/processor.h"


/*!
  Графическая сцена с эпюрами.
*/
class ConstructionPlotGraphicsScene : public ConstructionGraphicsScene
{
public:
    /*!
      Конструктор.
     \param processor - процессор с вычисленииями.
     \param scene - сцена с настройками отображения.
    */
    ConstructionPlotGraphicsScene(QSharedPointer<Processor> &processor);

    /*!
      Добаляет эпюру.
     \param label - название эпюра.
     \param method - метод, вычисляемый значения эпюра.
    */
    void addPlot(const QString &label, CalcMethod method);

    /*!
      Переопределенный метод.
      \see ConstructionGraphicsScene::updateSize
    */
    void updateSize(int width, int height) override;

public slots:
    /*!
      Устанавливает параметр отображения значений эпюра.
     \param show - параметр.
    */
    void showPlotsValues(bool show);

protected:
    /*!
      Переопределенный метод.
      \see ConstructionGraphicsScene::updateDrawing
    */
    void updateDrawing() override;

private:
    int PLOTS_HEIGHT; /*!< Общая высота всех эпюров. */

    bool displayPlotsValues; /*!< Отвечает за отображение значений на эпюрах. */

    QSharedPointer<Processor> processor_;       /*!< Процессор. */
    QVector<PlotValuesGraphicsItem*> plots_;    /*!< Массив с эпюрами. */
};

#endif // CONSTRUCTIONPLOTGRAPHICSSCENE_H
