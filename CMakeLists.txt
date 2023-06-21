cmake_minimum_required(VERSION 3.9)

set(NITRO_VERSION 1.2.0)

project(nitro VERSION ${NITRO_VERSION} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
cmake_policy(SET CMP0079 NEW)

set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Widgets OpenGL OpenGLWidgets)
find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)

set(NITRO_INCLUDE_DIRS "")
set(EXTERNAL_LIBS "")
add_subdirectory(external)
list(APPEND NITRO_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)

set(NITRO_LIBS "")
list(APPEND NITRO_LIBS ${OpenCV_LIBS})
list(APPEND NITRO_LIBS Qt6::Core)
list(APPEND NITRO_LIBS Qt6::Gui)
list(APPEND NITRO_LIBS Qt6::Widgets)
if (OpenMP_FOUND)
    list(APPEND NITRO_LIBS OpenMP::OpenMP_CXX)
else ()
    message("OpenMP was not found.")
endif ()

include_directories(
        ${CMAKE_BINARY_DIR}
        ${OpenCV_INCLUDE_DIRS})

file(GLOB_RECURSE CPPS src/*.cpp)
file(GLOB_RECURSE HPPS src/*.hpp include/*.hpp)

qt_add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
        ${CPPS} ${HPPS}
        resources/resources.qrc src/core/nodes/datatypes/colimagedata.cpp)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${NITRO_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${EXTERNAL_LIBS}
        PRIVATE
        ${NITRO_LIBS}
        )

# Enable Modules - add more modules if required
set(ENABLED_MODULES
        ImCore
        ImExtra
        Im3D
        )

include(cmake/config.cmake)

install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
