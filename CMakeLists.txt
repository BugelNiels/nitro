cmake_minimum_required(VERSION 3.16)
project(nitro VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS OpenGL OpenGLWidgets Widgets)
find_package(OpenMP REQUIRED)

# TODO: check this
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/flip/cpp/CPP)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/flip/cpp/common)

include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(3rdparty)

file(GLOB_RECURSE CPPS src/*.cpp)
file(GLOB_RECURSE HPPS src/*.hpp)


qt_add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
        ${CPPS} ${HPPS}
        resources/resources.qrc
        )

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/src/core
        ${CMAKE_CURRENT_LIST_DIR}/src/gui
        ${CMAKE_CURRENT_LIST_DIR}/src/nodes
        )

target_link_libraries(${PROJECT_NAME} PUBLIC QtNodes)

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
