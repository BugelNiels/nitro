add_subdirectory(nodeeditor)


add_library(flip INTERFACE)
target_include_directories(flip INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/flip/cpp)
