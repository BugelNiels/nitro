cmake_minimum_required(VERSION 3.16)
project(CbdImage VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS OpenGL OpenGLWidgets Widgets)

# Make sure to add any new files you add over here manually
qt_add_executable(BasicOpenGL WIN32 MACOSX_BUNDLE
    main.cpp
    imageview.cpp imageview.h
    meshview.cpp meshview.h
    mainwindow.cpp mainwindow.h mainwindow.ui
    renderers/renderer.cpp renderers/renderer.h
    renderers/surfacerenderer.cpp renderers/surfacerenderer.h
    settings/settings.h settings/viewsettings.h settings/colorsettings.h
    settings/shadertypes.h
    resources.qrc
)

target_link_libraries(BasicOpenGL PRIVATE
    Qt::Core
    Qt::Gui
)

if((QT_VERSION_MAJOR GREATER 5))
    target_link_libraries(BasicOpenGL PRIVATE
        Qt::OpenGL
        Qt::OpenGLWidgets
        Qt::Widgets
    )
endif()

install(TARGETS BasicOpenGL
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
