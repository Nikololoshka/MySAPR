#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QtWidgets>

#include "../construction/rod.h"
#include "../construction/longitudinalForce.h"
#include "../construction/sectionalForce.h"

#include "../graphics/constructionGraphicsScene.h"

class Processor;

/*!
  Тип вычислений.
*/
using CalcType = double;
/*!
  Тип метода для вычисления компонеты.
*/
using CalcMethod = CalcType (Processor::*)(int, CalcType) const;


/*!
  Обеспечивает расчёт компонент напряжённо-деформированного состояния конструкции.
*/
class Processor
{
public:
    /*!
      Конструктор по умолчанию.
    */
    Processor();
    /*!
      Метод для вычисления состояния.
     \param rods - стержни.
     \param longitudinalForces - сосредаточенные нагрузки.
     \param sectionalForces - распределенные нагрузки.
     \param pillars - заделки.
    */
    void compute(const QVector<Rod> &rods,
                 const QVector<LongitudinalForce> &longitudinalForces,
                 const QVector<SectionalForce> &sectionalForces,
                 const QVector<bool> &pillars);

    /*!
      Сохраняет вычисление.
     \param path - путь.
    */
    void save(QString path);
    /*!
      Загружает вычисления.
     \param path - путь.
    */
    void load(QString path);

    /*!
      Добавляет рассчитанную конструкцию на сцену.
     \param scene - сцена.
     */
    void bind(ConstructionGraphicsScene *scene) const;

    /*!
      Возращает результат вычисления.
    */
    QVector<CalcType> result() const;
    /*!
     Кол-во частей (стержней) в конструкции.
    */
    int parts() const;
    /*!
      Длина части (стержня).
     \param part - номер части.
    */
    double partLength(int part) const;
    /*!
      Максимальное напряжение на часть (стержень).
     \param part - часть.
    */
    double maxAllowableStrain(int part) const;

    /*!
      Вычисляет значение компоненты перемещения.
     \param part - часть (стержень).
     \param x - значение длины части.
    */
    CalcType movement(int part, CalcType x) const;
    /*!
      Вычисляет значение компоненты продольной силы.
     \param part - часть (стержень).
     \param x - значение длины части.
    */
    CalcType longitudinalForce(int part, CalcType x) const;
    /*!
      Вычисляет значение компоненты нормального напряжения.
     \param part - часть (стержень).
     \param x - значение длины части.
    */
    CalcType normalStrain(int part, CalcType x) const;

private:
    QVector<CalcType> result_; /*!< Результат вычисления. */

    QVector<Rod> rods_;                             /*!< Стержни. */
    QVector<LongitudinalForce> longitudinalForces_; /*!< Сосредаточенные нагрузки. */
    QMap<int, SectionalForce> sectionalForces_;     /*!< Распределенные нагрузки. */
    QVector<bool> pillars_;                         /*!< Заделки. */
};

#endif // PROCESSOR_H
