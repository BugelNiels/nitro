set(MODULE_NAME imExtra)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE MODULE_SOURCES src/*.cpp)
file(GLOB_RECURSE MODULE_HEADERS src/*.hpp)

add_library(${MODULE_NAME} STATIC ${MODULE_SOURCES} ${MODULE_HEADERS})

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
