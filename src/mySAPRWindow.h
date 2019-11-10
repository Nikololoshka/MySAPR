#ifndef MYSAPRWINDOW_H
#define MYSAPRWINDOW_H

#include <QtWidgets>

#include "construction/rod.h"
#include "construction/longitudinalForce.h"
#include "construction/sectionalForce.h"

#include "graphics/constructionGraphicsScene.h"

#include "view/rodPropertiesWidget.h"
#include "view/longitudinalForcePropertiesWidget.h"
#include "view/sectionalForcePropertiesWidget.h"

#include "widgets/qGraphicsViewBlocked.h"


/*!
  Главное окно программы MySAPR.
*/
class MySAPRWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
      Конструктор.
     \param parent - родительский виджет.
    */
    explicit MySAPRWindow(QWidget *parent = nullptr);

private slots:
    /*!
      Вызывается при нажатии на кнопку "добавить".
    */
    void addButtonClicked();
    /*!
      Вызывается при нажатии на кнопку "удалить".
    */
    void removeButtonClicked();
    /*!
      Вызывается при нажатии на кнопку "вверх".
    */
    void upButtonClicked();
    /*!
      Вызывается при нажатии на кнопку "вниз".
    */
    void downButtonClicked();

    /*!
      Вызывается при нажатии на кнопку "новый проект".
     \return bool - успех выполнения.
    */
    bool newProjectClicked();
    /*!
      Вызывается при нажатии на кнопку "открыть проект".
     \return bool - успех выполнения.
    */
    bool openProjectClicked();
    /*!
      Вызывается при нажатии на кнопку "открыть расчет".
     \return bool - успех выполнения.
    */
    bool openComputeResultClicked();
    /*!
      Вызывается при нажатии на кнопку "сохранить".
     \return bool - успех выполнения.
    */
    bool saveProjectClicked();
    /*!
      Вызывается при нажатии на кнопку "сохранить как".
     \return bool - успех выполнения.
    */
    bool saveAsProjectClicked();

    /*!
      Вызывается при выборе элемента в списках.
    */
    void treeWidgetItemSelected();
    /*!
      Вызывается при двойном нажатии на элемент в списке.
     \param item - элемент.
     \param column - нажатый столбец.
    */
    void treeWidgetItemDoubleClicked(QTreeWidgetItem * item, int column);
    /*!
      Вызывается при завершении редактирования элемента в списке.
     \param item - элемент.
     \param column - редактируемый столбец.
    */
    void treeWidgetItemEdited(QTreeWidgetItem * item, int column);

    /*!
      Вызывается при переключении между списками.
    */
    void tabWidgetSelected();
    /*!
      Вызывается при необходимости перерисовки сцены.
    */
    void requiresRedrawing();
    /*!
      Вызывается при смене варианта размещения заделки.
    */
    void comboBoxFulcrumChanged();

    /*!
      Вызывается при нажатии на кнопку "рассчитать".
    */
    void computeButtonClicked();

protected:
    /*!
      Переопределенный метод.
      \see QMainWindow::resizeEvent
    */
    void resizeEvent(QResizeEvent *event) override;

private:
    /*!
      Обновляет кнопки навигации по спискам.
    */
    void updateButtons();
    /*!
      Возвращает кол-во заделок в конструкции.
    */
    int pillarCount();
    /*!
      Меняет виджет для изменение свойств (значений) элемента.
     \param treeWidget - список с элементами.
     \param item - элемент.
    */
    void changePropertiesTable(QTreeWidget *treeWidget, QTreeWidgetItem *item);

    /*!
      Проверка заданых сосредоточенных сил на корректность.
    */
    void checkLongitudinalForceErrors();
    /*!
      Проверка заданых распределенных сил на корректность.
    */
    void checkSectionalForceErrors();

    /*!
      Добавляет стержень на сцену.
     \param rod - стержень.
    */
    void addRodToScene(Rod &rod);
    /*!
      Добавляет сосредоточенную силу на корректность.
     \param force - сосредоточенная сила.
    */
    void addLongitudinalForceToScene(LongitudinalForce &force);
    /*!
       Добавляет распределенную силу на корректность.
     \param force - распределенная сила.
    */
    void addSectionalForceToScene(SectionalForce &force);

    /*!
      Удалить все конструкцию.
    */
    void clearAll();

private:
    const static int TYPE_ELEMENT = Qt::UserRole + 1;    /*!< Роль "типа элемента". */

    const static int NOTHING_ELEMENT = 0;       /*!< Никакой элемент. */
    const static int ROD_ELEMENT = 1;           /*!< Стержень. */
    const static int LONGITUDINAL_FORCE = 2;    /*!< Сосредаточенная сила. */
    const static int SECTIONAL_FORCE = 3;       /*!< Распределенная сила. */

    size_t rodIdPool;           /*!< Пул с ID для стерженей. */
    size_t longitudinalIdPool;  /*!< Пул с ID для сосредаточенных сил. */
    size_t sectionalIdPool;     /*!< Пул с ID для распределенных сил. */

    bool isLongitudinalChecked; /*!< Сосредаточенная сила редактируется. */
    bool isSectionalChecked;    /*!< Распределенная сила редактируется. */
    bool hasChange;             /*!< Есть изменения. */

    QGraphicsViewBlocked *graphicView; /*!< Содержит сцену конструкции. */

    QTabWidget *tabWidget;                  /*!< Содержит виджеты с редактируемыми элементами конструкции.  */
    QTreeWidget *treeWidgetRods;            /*!< Виджет с стерженями. */
    QTreeWidget *treeWidgetLongitudinal;    /*!< Виджет с сосредаточенными силами. */
    QTreeWidget *treeWidgetSectional;       /*!< Виджет с распределенными силами. */

    QStackedWidget *propertiesWidget;                                       /*!< Содержит виджеты
                                                                                 для редактирования. */
    RodPropertiesWidget *rodPropertiesWidget;                               /*!< Виджет для редактирование
                                                                                 стержня. */
    LongitudinalForcePropertiesWidget *longitudinalForcePropertiesWidget;   /*!< Виджет для редактирование
                                                                                 сосредоточенной нагрузки. */
    SectionalForcePropertiesWidget *sectionalForcePropertiesWidget;         /*!< Виджет для редактирование
                                                                                 распределенной нагрузки. */

    ConstructionGraphicsScene *graphicScene; /*!< Сцена конструкции. */

    QPushButton *pushButtonCompute; /*!< Кнопка "рассчитать". */

    QComboBox *comboBoxFulcrum;     /*!< Список с вариантами размещения заделки. */
    QLabel *labelProperties;        /*!< Метка свойств. */

    QPushButton *pushButtonAdd;     /*!< Кнопка "добавить". */
    QPushButton *pushButtonRemove;  /*!< Кнопка "удалить". */
    QPushButton *pushButtonUp;      /*!< Кнопка "вверх". */
    QPushButton *pushButtonDown;    /*!< Кнопка "вниз". */

    QVector<Rod> rods;                              /*!< Стержни. */
    QVector<LongitudinalForce> longitudinalForces;  /*!< Сосредаточенные силы. */
    QVector<SectionalForce> sectionalForces;        /*!< Распределенные силы. */

    QFileInfo projectFileInfo; /*!< Информация о файле проекта. */

    QMap<int, QString> longitudinalErrors;  /*!< Ошибки связанные с сосредаточенными силами. */
    QMap<int, QString> sectionalErrors;     /*!< Ошибки связанные с распределенными силами. */
};

#endif // MYSAPRWINDOW_H
