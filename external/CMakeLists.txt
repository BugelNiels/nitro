set(NITRO_INCLUDE_DIRS "")
set(EXTERNAL_LIBS "")

# QtNodes
add_subdirectory(nodeeditor)
list(APPEND NITRO_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/nodeeditor)
list(APPEND EXTERNAL_LIBS QtNodes)

# FLIP
option(FLIP_LIBRARY ON)
add_subdirectory(flip)
list(APPEND NITRO_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/flip/cpp/CPP)
list(APPEND NITRO_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/flip/cpp/common)

# QT Value Sliders
option(SLIDER_LIBRARY ON)
add_subdirectory(qt-value-slider)
list(APPEND NITRO_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/qt-value-slider/include)
list(APPEND EXTERNAL_LIBS valueSliders)

# Update parent variable.
set(NITRO_INCLUDE_DIRS ${NITRO_INCLUDE_DIRS} PARENT_SCOPE)
set(EXTERNAL_LIBS ${EXTERNAL_LIBS} PARENT_SCOPE)
