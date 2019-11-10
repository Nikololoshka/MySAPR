#ifndef SECTIONALFORCE_H
#define SECTIONALFORCE_H

#include "constructionElement.h"

/*!
  Распределенная нагрузка.
*/
class SectionalForce : public ConstructionElement
{
public:
    /*!
      Конструктор по умолчанию.
    */
    SectionalForce();

    /*!
      Загрузка нагрузки из JSON файла.
     \param obj - JSON файл.
    */
    void load(QJsonObject obj) override;
    /*!
      Сохранение нагрузки в JSON файлу.
     \return QJsonObject - JSON файл.
    */
    QJsonObject save() const override;

    /*!
      Возвражает значение силы нагрузки.
    */
    double force() const;
    /*!
      Устанавливает значение силы нагрузки.
     \param force
    */
    void setForce(double force);

    /*!
      Возвращает номер стержня, к которой приложена нагрузка.
    */
    int rodId() const;
    /*!
      Устанавливает номер стержня, к которой приложена нагрузка.
    */
    void setRodId(int rodId);

private:
    double force_;  /*!< Значение силы. */
    int rodId_;     /*!< Номер стержня. */
};

#endif // SECTIONALFORCE_H
