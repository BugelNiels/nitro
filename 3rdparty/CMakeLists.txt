add_subdirectory(nodeeditor)

add_library(flip STATIC)
target_include_directories(flip PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/flip/cpp/common ${CMAKE_CURRENT_SOURCE_DIR}/flip/cpp/CPP)

