cmake_minimum_required(VERSION 3.9)

#  -------------------- Versioning ----------------------------------
file(STRINGS "version.txt" NITRO_VERSION LIMIT_COUNT 1)
set(NITRO_NAME "NITRO")

project(nitro VERSION ${NITRO_VERSION} LANGUAGES CXX)

#  -------------------- Options -----------------------------

set(CMAKE_INCLUDE_CURRENT_DIR ON)
cmake_policy(SET CMP0079 NEW)

set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#  -------------------- Required components --------------------
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGL OpenGLWidgets)
find_package(OpenCV REQUIRED)
find_package(OpenMP)
find_package(ZLIB REQUIRED)

#  -------------------- 3rd Party dependencies
add_subdirectory(external)
list(APPEND NITRO_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)

#  -------------------- Requires libraries --------------------
set(NITRO_LIBS "")
list(APPEND NITRO_LIBS ${OpenCV_LIBS})
list(APPEND NITRO_LIBS Qt6::Core)
list(APPEND NITRO_LIBS Qt6::Gui)
list(APPEND NITRO_LIBS Qt6::Widgets)
list(APPEND NITRO_LIBS ZLIB::ZLIB)
if (OpenMP_FOUND)
    list(APPEND NITRO_LIBS OpenMP::OpenMP_CXX)
else ()
    message("OpenMP was not found. Compiling without OpenMP support.")
endif ()

#  -------------------- Create main project target --------------------
include_directories(
        ${CMAKE_BINARY_DIR}
        ${OpenCV_INCLUDE_DIRS})


file(GLOB_RECURSE CPPS src/*.cpp)
file(GLOB_RECURSE HPPS src/*.hpp include/*.hpp)

qt_add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
        ${CPPS} ${HPPS}
        resources/resources.qrc
        )

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

# -------------------- Modules --------------------

# Module functions
include(cmake/modules.cmake)

add_module(ImCore)
add_module(ImProc)
add_module(Compression)
#add_module(Thesis)

include(cmake/config.cmake)

install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
