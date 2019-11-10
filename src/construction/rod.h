#ifndef ROD_H
#define ROD_H

#include "constructionElement.h"

/*!
  Класс стержня конструкции.
*/
class Rod : public ConstructionElement
{
public:
    /*!
      Конструктор по умолчанию.
    */
    Rod();

    /*!
      Загружает информацию о стержне из JSON файла.
    */
    void load(QJsonObject obj) override;
    /*!
      Сохраняет информацию о стержне в JSON файл.
    */
    QJsonObject save() const override;

    /*!
      Возвращает длину стержня.
    */
    double length() const;
    /*!
      Устанавливает длину стержня.
    */
    void setLength(double value);

    /*!
      Возвращает площадь поперечного сечения стержня.
    */
    double sectionalArea() const;
    /*!
      Устанавливает площадь поперечного сечения стержня.
    */
    void setSectionalArea(double value);

    /*!
      Возвращает модуль упругости стержня.
    */
    double elasticUnit() const;
    /*!
      Устанавливает модуль упругости стержня.
    */
    void setElasticUnit(double value);

    /*!
      Возвращает допускаемое напряжение стержня.
    */
    double allowableStrain() const;
    /*!
      Устанавливает допускаемое напряжение стержня.
    */
    void setAllowableStrain(double value);

private:
    double length_;             /*!< Длина. */
    double sectionalArea_;      /*!< Площадь поперечного сечения. */
    double elasticUnit_;        /*!< Модуль упругости. */
    double allowableStrain_;    /*!< Допускаемое напряжение. */
};

#endif // ROD_H
