set(MODULE_NAME im3D)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE MODULE_SOURCES src/*.cpp)
file(GLOB_RECURSE MODULE_HEADERS src/*.hpp)

# Any additional required packages defined here
list(APPEND MODULE_LIBS Qt::OpenGL)
list(APPEND MODULE_LIBS Qt::OpenGLWidgets)

add_library(${MODULE_NAME} STATIC ${MODULE_SOURCES} ${MODULE_HEADERS} src/nodes/restoration/resampler/smoothstepsampler.cpp src/nodes/restoration/resampler/easeinsampler.cpp)

target_include_directories(${MODULE_NAME}
        PUBLIC
        ${NITRO_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

# Always required
target_link_libraries(${MODULE_NAME}
        PUBLIC
        ${EXTERNAL_LIBS}

        PRIVATE
        ${NITRO_LIBS}
        ${MODULE_LIBS}
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${MODULE_NAME}
        PRIVATE
        ${MODULE_LIBS}
        )
