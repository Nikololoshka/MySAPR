cmake_minimum_required(VERSION 3.0)

project(MySAPR VERSION 1.0 DESCRIPTION "${PROJECT_NAME} with Qt")

set(COMPANY "PEPEGA INC.")
set(COPYRIGHT "All rights reserved.")
set(IDENTIFIER "github.com/nikololoshka")

set(CMAKE_CXX_STANDARD 14)
add_definitions("-Wall" "-O2")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)

list(APPEND SOURCE_FILES
    src/main.cpp
    src/mySAPRWindow.h
    src/mySAPRWindow.cpp
)

# виджеты свойств
list(APPEND SOURCE_FILES
    src/view/rodPropertiesWidget.h
    src/view/rodPropertiesWidget.cpp
    src/view/longitudinalForcePropertiesWidget.h
    src/view/longitudinalForcePropertiesWidget.cpp
    src/view/sectionalForcePropertiesWidget.h
    src/view/sectionalForcePropertiesWidget.cpp
)

# графика
list(APPEND SOURCE_FILES
    src/graphics/constructionGraphicsScene.h
    src/graphics/constructionGraphicsScene.cpp
    src/graphics/constructionPlotGraphicsScene.h
    src/graphics/constructionPlotGraphicsScene.cpp

    src/graphics/plotValuesGraphicsItem.h
    src/graphics/plotValuesGraphicsItem.cpp

    src/graphics/pillarGraphicsItem.h
    src/graphics/pillarGraphicsItem.cpp

    src/graphics/rodGraphicsItem.h
    src/graphics/rodGraphicsItem.cpp
    src/graphics/longitudinalForceGraphicsItem.h
    src/graphics/longitudinalForceGraphicsItem.cpp
    src/graphics/sectionalForceGraphicsItem.h
    src/graphics/sectionalForceGraphicsItem.cpp

    src/graphics/labels/rodNumberGraphicsItem.h
    src/graphics/labels/rodNumberGraphicsItem.cpp
    src/graphics/labels/nodeNumberGraphicsItem.h
    src/graphics/labels/nodeNumberGraphicsItem.cpp
    src/graphics/labels/rodLengthGraphicsItem.h
    src/graphics/labels/rodLengthGraphicsItem.cpp
)

# элементы конструкции
list(APPEND SOURCE_FILES
    src/construction/constructionElement.h
    src/construction/constructionElement.cpp
    src/construction/rod.h
    src/construction/rod.cpp
    src/construction/longitudinalForce.h
    src/construction/longitudinalForce.cpp
    src/construction/sectionalForce.h
    src/construction/sectionalForce.cpp
)

# пост-процессор
list(APPEND SOURCE_FILES
    src/postprocessor/postProcessorWindow.h
    src/postprocessor/postProcessorWindow.cpp
)

# процессор
list(APPEND SOURCE_FILES
    src/processor/processor.h
    src/processor/processor.cpp
    src/processor/linearMethods.h
    src/processor/linearMethods.hpp
)

# свои виджеты
list(APPEND SOURCE_FILES
    src/widgets/qDoubleSpinBoxNotZero.h
    src/widgets/qDoubleSpinBoxNotZero.cpp
    src/widgets/qDoubleSpinBoxFine.h
    src/widgets/qDoubleSpinBoxFine.cpp
    src/widgets/qGraphicsViewBlocked.h
    src/widgets/qGraphicsViewBlocked.cpp
)

# ресурсы
list(APPEND RESOURCE_FILES
    res.qrc
)

add_subdirectory(libs)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${RESOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets QCustomPlot2)
