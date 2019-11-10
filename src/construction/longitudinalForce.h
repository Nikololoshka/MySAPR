#ifndef LONGITUDINALFORCE_H
#define LONGITUDINALFORCE_H

#include "constructionElement.h"

/*!
  Сосредаточенная нагрузка.
*/
class LongitudinalForce : public ConstructionElement
{
public:
    /*!
      Конструктор по умолчанию.
    */
    LongitudinalForce();

    /*!
      Загрузка нагрузки из JSON файла.
    */
    void load(QJsonObject obj) override;
    /*!
      Сохранение нагрузки в JSON файл.
    */
    QJsonObject save() const override;

    /*!
      Возвращает значение нагрузки.
    */
    double force() const;
    /*!
      Устанавливает значение нагрузки.
    */
    void setForce(double force);

    /*!
      Возвращает номер узла, к которому приложена нагрузка.
    */
    int nodeId() const;
    /*!
      Устанавливает номер узла, к которому приложена нагрузка.
    */
    void setNodeId(int nodeId);

private:
    double force_;  /*!< Значение нагрузки. */
    int nodeId_;    /*!< Номер узла. */
};

#endif // LONGITUDINALFORCE_H
