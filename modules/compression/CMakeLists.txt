set(MODULE_NAME Compression)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

requires_module(ImCore)

file(GLOB_RECURSE MODULE_SOURCES src/*.cpp)
file(GLOB_RECURSE MODULE_HEADERS src/*.hpp)

add_library(${MODULE_NAME} STATIC ${MODULE_SOURCES} ${MODULE_HEADERS})

set(INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )
list(APPEND NITRO_INCLUDE_DIRS ${INCLUDE_DIRS})

target_include_directories(${MODULE_NAME}
        PUBLIC
        ${INCLUDE_DIRS}
        )

target_link_libraries(${MODULE_NAME}
        PRIVATE
        ${MODULE_LIBS}
        )

set(MODULE_NAME ${MODULE_NAME} PARENT_SCOPE)
set(MODULE_LIBS ${MODULE_LIBS} PARENT_SCOPE)
set(NITRO_INCLUDE_DIRS ${NITRO_INCLUDE_DIRS} PARENT_SCOPE)
set(MODULE_RESOURCES resources/resources.qrc PARENT_SCOPE)
