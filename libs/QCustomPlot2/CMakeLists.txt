set(LIBRARY_NAME QCustomPlot2)

set(HEADER_FILES qcustomplot.h)
set(SOURCE_FILES qcustomplot.cpp)

find_package(Qt5PrintSupport REQUIRED)

add_library(${LIBRARY_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${LIBRARY_NAME} INTERFACE ./)
target_link_libraries(${LIBRARY_NAME} Qt5::Widgets Qt5::PrintSupport)

