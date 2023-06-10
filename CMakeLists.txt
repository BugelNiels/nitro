cmake_minimum_required(VERSION 3.9)
project(nitro VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS OpenGL OpenGLWidgets Widgets)
find_package(OpenMP REQUIRED)

# TODO: check this
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/flip/cpp/CPP)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/flip/cpp/common)

include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(external)

file(GLOB_RECURSE CPPS src/*.cpp modules/*.cpp)
file(GLOB_RECURSE HPPS src/*.hpp modules/*.hpp include/*.hpp)


qt_add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
        ${CPPS} ${HPPS}
        resources/resources.qrc
        modules/imCore/gui/im3dviewer/renderdockwidget.cpp modules/imCore/gui/imviewer/imviewdockwidget.cpp include/nitromodule.hpp modules/imCore/imcoremodule.cpp modules/imCore/imcoremodule.hpp)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/src/core
        ${CMAKE_CURRENT_LIST_DIR}/src/gui
        ${CMAKE_CURRENT_LIST_DIR}/src/improc
        )

target_link_libraries(${PROJECT_NAME} PUBLIC QtNodes)
target_link_libraries(${PROJECT_NAME} PUBLIC valueSliders)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        )


target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)

if ((QT_VERSION_MAJOR GREATER 5))
    target_link_libraries(${PROJECT_NAME} PRIVATE
            Qt::OpenGL
            Qt::OpenGLWidgets
            Qt::Widgets
            )
endif ()

install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
