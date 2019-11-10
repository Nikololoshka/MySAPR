#ifndef CONSTRUCTIONGRAPHICSSCENE_H
#define CONSTRUCTIONGRAPHICSSCENE_H

#include <QtWidgets>

#include "pillarGraphicsItem.h"
#include "rodGraphicsItem.h"
#include "longitudinalForceGraphicsItem.h"
#include "sectionalForceGraphicsItem.h"

#include "labels/rodNumberGraphicsItem.h"
#include "labels/nodeNumberGraphicsItem.h"
#include "labels/rodLengthGraphicsItem.h"


/*!
  Графическая сцена, на которой отображается конструкция.
*/
class ConstructionGraphicsScene : public QGraphicsScene
{
public:
    /*!
      Перечисление с возможными вариантами размещения заделки.
    */
    enum PillarSetup {
        Left,
        Right,
        LeftAndRight
    };

    /*!
      Конструктор графической сцены.
     \param parent - родительский объект.
    */
    explicit ConstructionGraphicsScene(QObject *parent = nullptr);
    /*!
      Деструктор по умолчанию.
    */
    virtual ~ConstructionGraphicsScene() = default;

    /*!
      Добавляет стержень на сцену с заданными параметрами.
     \param length - длина.
     \param sectionalArea - площадь поперечного сечения.
    */
    void appendRod(double length, double sectionalArea);
    /*!
      Обновляет параметры стержня по его индексу.
     \param index - индекс.
     \param length - длина.
     \param sectionalArea - площадь поперечного сечения.
    */
    void updateRod(int index, double length, double sectionalArea);
    /*!
      Меняет стержни местами.
     \param first - индекс первого стержня.
     \param second - индекс второго стержня.
    */
    void swapRods(int first, int second);
    /*!
      Удаляет стержень.
     \param index - индекс стержня.
    */
    void removeRod(int index);

    /*!
      Добавляет сосредаточенную нагрузку с заданными параметрами.
     \param id - ID нагрузки.
     \param nodeId - узел, к которому приложена нагрузка.
     \param force - значение силы нагрузки.
    */
    void appendLongitudinalForce(size_t id, int nodeId, double force);
    /*!
      Обновляет параметры сосредаточенной нагрузки по её ID.
     \param id - ID нагрузки.
     \param nodeId - узел, к которому приложена нагрузка.
     \param force - значение силы нагрузки.
    */
    void updateLongitudinalForce(size_t id, int nodeId, double force);
    /*!
      Удаляет сосредаточенную нагрузку.
     \param id - ID нагрузки.
    */
    void removeLongitudinalForce(size_t id);

    /*!
      Добавляет распределенную нагрузку с заданными параметрами.
     \param id - ID нагрузки.
     \param rodId - стержень, к которому приложена нагрузка.
     \param force - значение силы нагрузки.
    */
    void appendSectionalForce(size_t id, int rodId, double force);
    /*!
      Обновляет параметры распределенной нагрузки по её ID.
     \param id - ID нагрузки.
     \param rodId - стержень, к которому приложена нагрузка.
     \param force - значение силы нагрузки.
    */
    void updateSectionalForce(size_t id, int rodId, double force);
    /*!
      Удаляет распределенную нагрузку.
     \param id - ID нагрузки.
    */
    void removeSectionalForce(size_t id);

    /*!
      Обновляет размеры сцены.

     \param width - ширина.
     \param height - высота.
    */
    virtual void updateSize(int width, int height);

    /*!
      Устанавливает значение отображаемых заделок.
     \param pillarSetup - значение размещения
    */
    void setPillarSetup(const PillarSetup &pillarSetup);
    /*!
      Отчистить все графические элементы с сцены.
    */
    virtual void clearAll();

    /*!
      Возвращает параметр показа номеров стержней.
    */
    bool displayRodNumbers() const;
    /*!
      Возвращает параметр показа номеров узлов.
    */
    bool displayNodeNumbers() const;
    /*!
      Возвращает параметр показа значений сосредаточенных нагрузок.
    */
    bool displayLongitudinalLabels() const;
    /*!
      Возвращает параметр показа значений распределенных нагрузок.
    */
    bool displaySectionalLabels() const;
    /*!
      Возвращает параметр показа длин стержней.
    */
    bool displyRodLengths() const;
    /*!
      Возвращает параметр показа сосредоточенный силы к узлу.
    */
    bool isForceToNode() const;

public slots:
    /*!
      Устанавливает параметр показа номеров стержней.
     \param show - параметр.
    */
    virtual void showRodLabels(bool show);
    /*!
      Устанавливает параметр показа номеров узлов.
     \param show - параметр.
    */
    virtual void showNodeLabels(bool show);
    /*!
      Устанавливает параметр показа значения сосредаточенных нагрузок.
     \param show - параметр.
    */
    virtual void showLongitudinalLabels(bool show);
    /*!
      Устанавливает параметр показа распределенных нагрузок.
     \param show - параметр.
    */
    virtual void showSectionalLabels(bool show);
    /*!
      Устанавливает параметр показа длин стержня.
     \param show - параметр.
    */
    virtual void showRodLengths(bool show);

    /*!
      Устанавливает параметр показа сосредоточенный силы к узлу.
     \param forceToNode - параметр.
    */
    virtual void longitudinalforceToNode(bool forceToNode);

protected:
    /*!
      Вычисляет длины конструкции для отображения на сцене.
     \param maxLength - максимальная длина сцены
     \return QPair<QVector<double>, double> - пара значений: длины для стержней, общая длина конструкции.
    */
    virtual QPair<QVector<double>, double> computeWidth(const double maxLength);
    /*!
      Высисляет высоту конструкции для отображения на сцене.
     \param maxLenght - максимальная высота сцены.
     \return QPair<QVector<double>, double> - пара значений: значения высот для стержней, самая большая высота.
    */
    virtual QPair<QVector<double>, double> computeHeight(const double maxLenght);

    /*!
      Обновляет все графические элементы на сцене.
    */
    virtual void updateDrawing();

protected:
    const int MIN_DRAW_STEP; /*!< Минимальный размер отрисовываемого элемента. */

    bool displayRodNumbers_;         /*!< Отвечает за отрисовку номеров стержней. */
    bool displayNodeNumbers_;        /*!< Отвечает за отрисовку номеров узлов. */
    bool displayLongitudinalLabels_; /*!< Отвечает за отрисовку значений сосредоточенных сил. */
    bool displaySectionalLabels_;    /*!< Отвечает за отрисовку значений распределенных сил. */
    bool displyRodLengths_;          /*!< Отвечает за отрисовку длин стержней. */

    bool isForceToNode_; /*!< Отвечает за отрисовку сосредоточенных сил к узлу. */

    QVector<RodNumberGraphicsItem*> rodNumbers;     /*!< Массив с номерами стержней. */
    QVector<NodeNumberGraphicsItem*> nodeNumbers;   /*!< Массив с номерами узлов. */
    QVector<RodLengthGraphicsItem*> rodLengths;     /*!< Массив с длинами стержней. */
    QVector<QGraphicsLineItem*> separateLines;      /*!< Массив с разделительными линиями */

    QVector<RodGraphicsItem*> rods;                                 /*!< Массив с стержнями. */
    QMap<size_t, LongitudinalForceGraphicsItem*> longitudinalForces;/*!< Массив с сосредаточенными нагрузками. */
    QMap<size_t, SectionalForceGraphicsItem*> sectionalForces;      /*!< Массив с распределенными нагрузками. */

    PillarGraphicsItem *leftPillar;     /*!< Левая заделка. */
    PillarGraphicsItem *rightPillar;    /*!< Правая заделка. */

    PillarSetup pillarSetup; /*!< Вариант отображения заделкок. */
};

#endif // CONSTRUCTIONGRAPHICSSCENE_H
