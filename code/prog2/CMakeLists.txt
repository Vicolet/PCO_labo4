cmake_minimum_required(VERSION 3.13)
project(PCO_LAB04_prog2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Core Gui Test Widgets PrintSupport)
if (NOT Qt5_FOUND)
    find_package(Qt6 COMPONENTS Core Gui Test Widgets PrintSupport Core5Compat REQUIRED)
endif()

find_library(QTRAINSIM_LIB qtrainsim PATHS ${CMAKE_CURRENT_BUILD_DIR}/QtrainSim/)

if (Qt5_FOUND)
    add_definitions(-DUSING_QT5)
else()
    add_definitions(-DUSING_QT6)
endif()

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Liste des fichiers sources avec chemins complets
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sharedstation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/locomotive.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cppmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/locomotivebehavior.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sharedstation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sharedsectioninterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/locomotive.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/launchable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/locomotivebehavior.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sharedsection.h
)

qt_add_resources(SOURCES ../../QtrainSim/qtrainsim.qrc)

add_executable(PCO_LAB04_prog2 ${SOURCES} ${HEADERS})


if (Qt5_FOUND)
    target_link_libraries(PCO_LAB04_prog2 PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport qtrainsim -lpcosynchro)
else()
    target_link_libraries(PCO_LAB04_prog2 PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::PrintSupport Qt6::Core5Compat qtrainsim -lpcosynchro)
endif()

file(COPY ../../QtrainSim/data DESTINATION ${CMAKE_BINARY_DIR}/code/prog2)
