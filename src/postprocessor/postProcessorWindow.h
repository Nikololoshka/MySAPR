#ifndef POSTPROCESSORWINDOW_H
#define POSTPROCESSORWINDOW_H

#include <QtWidgets>

#include "qcustomplot.h"

#include "../processor/processor.h"
#include "../graphics/constructionGraphicsScene.h"
#include "../graphics/constructionPlotGraphicsScene.h"


/*!
  Окно постпроцессора (результатов расчета).
*/
class PostProcessorWindow : public QDialog
{
    Q_OBJECT

public:
    /*!
      Конструктор с параметрами.
     \param processor - процессор с вычислениями.
     \param scene - сцена для взятия настроек.
     \param parent - родительский виджет.
    */
    PostProcessorWindow(QSharedPointer<Processor> &processor,
                        ConstructionGraphicsScene *scene,
                        QWidget *parent = nullptr);

protected:
    /*!
      Переопрееленный метод.
      \see QDialog::resizeEvent
    */
    void resizeEvent(QResizeEvent *event) override;

private slots:

    /*!
      Вызывается при сохранении результатов расчета.
    */
    void saveComputeResultClicked();
    /*!
      Вызывается при смене формы отображения разультатов.
     \param index
    */
    void currentModeChanged(int index);

    /*!
      Изменяет масштаб сцены с эпюрами.
     \param scale - масштаб.
    */
    void updateViewArea(int scale);

    /*!
      Вызывается при смене отображаемой компоненты в таблице.
     \param index - индекс компонента.
    */
    void changeComponentTable(int index);
    /*!
      Вызывается при смене отображаемого стержня в таблице.
     \param index - индекс стержня.
    */
    void changeTableDisplayedRodComponent(int index);
    /*!
      Обновляет содержимое таблицы.
     \param rows - кол-во строк.
    */
    void updateComponentTableValues(int rows);

    /*!
      Вызывается при смене отображаемой компоненты на графике.
     \param index - индекс компонента.
    */
    void changeComponentPlot(int index);
    /*!
      Вызывается при смене отображаемого стержня на графике.
     \param index - индекс стержня.
    */
    void changePlotDisplayedRodComponent(int index);
    /*!
      Обновляет содержимое графика.
    */
    void updateComponentPlotValues();
    /*!
      Вызывается при определении компонент в заданном сечении.
     \param value - сечение.
    */
    void sectionalValueChanged(double value);
    /*!
      Вызывается при движении курсора на графике.
    */
    void showPointToolTip(QMouseEvent *event);

private:
    /*!
      Инициализирует конструкцию с эпюрами.
     \param scene - сцена для взятия настроек.
    */
    void initGraphicMode(ConstructionGraphicsScene *scene);
    /*!
      Инициализирует представление в виде графиков.
    */
    void initPlottingMode();
    /*!
      Инициализирует табличное предстваление.
    */
    void initTableMode();

private:
    const static int GRAPHIC_MODE = 0;  /*!< Режим конструкции с эпюрами. */
    const static int PLOTTING_MODE = 1; /*!< Режим графиков. */
    const static int TABLE_MODE = 2;    /*!< Табличный режим. */

    const static int SHOW_ALL = -1;     /*!< Показать все компоненты/стержни. */

    QSharedPointer<Processor> processor; /*!< Процессор. */

    int currentTableComponent;      /*!< Текущая отображаемая компонента в таблице. */
    int currentTableDisplayedRod;   /*!< Текущий отображаемый стержень в таблице. */

    int currentPlotComponent;       /*!< Текущая отображаемая компонента на графике. */
    int currentPlotDisplayedRod;    /*!< Текущий отображаемый стержень на графике. */

    QTabWidget *tabWidgetMode;      /*!< Виджет для переключение форм отображения. */

    ConstructionPlotGraphicsScene *graphicScene; /*!< Сцена для конструкции с эпюрами. */
    QGraphicsView *graphicView;                  /*!< Отображает сцену. */
    QSpinBox *spinBoxScale;                      /*!< Для выбора масштаба. */
    int scale_;                                  /*!< Масштаб. */

    QComboBox *comboBoxPlotComponents;          /*!< Выбор компоненты для отображения на графике. */
    QComboBox *comboBoxPlotRod;                 /*!< Выбор стержня для отображения на графике. */
    QCustomPlot *plot;                          /*!< График. */
    QLabel *labelSectionValues;                 /*!< Значения в сечении. */
    QDoubleSpinBox *doubleSpinBoxSectionValue;  /*!< Для установки конкретного сечения. */

    QSpinBox *spinBoxCountTable;        /*!< Кол-во частей в таблице на стержень. */
    QComboBox *comboBoxTableComponents; /*!< Выбор компоненты для отображения в таблице. */
    QComboBox *comboBoxTableRod;        /*!< Выбор стержня для отображения в таблице. */
    QTableWidget *tableWidgetResult;    /*!< Таблица. */
};

#endif // POSTPROCESSORWINDOW_H
