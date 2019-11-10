#ifndef CONSTRUCTIONELEMENT_H
#define CONSTRUCTIONELEMENT_H

#include <QString>
#include <QJsonObject>

/*!
  Базовый класс для элементов стержневой конструкции.
*/
class ConstructionElement
{
public:
    /*!
      Деструктор по умолчанию.
    */
    virtual ~ConstructionElement() = default;

    /*!
      Загрузка элемента из JSON файла.
    */
    virtual void load(QJsonObject obj) = 0;
    /*!
      Сохранение объекта в JSON файл.
    */
    virtual QJsonObject save() const = 0;

    /*!
      Возвращает название элемента.
    */
    virtual QString name() const;
    /*!
      Устанавливает название элемента.
    */
    virtual void setName(const QString &name);

    /*!
      Возвращает ID элемента.
    */
    virtual size_t id() const;
    /*!
      Устанавливает ID элемента.
    */
    virtual void setId(size_t id);

protected:
    QString name_;  /*!< Имя элемента. */
    size_t id_;     /*!< ID элемента. */
};

#endif // CONSTRUCTIONELEMENT_H
