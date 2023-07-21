
list(REMOVE_DUPLICATES ENABLED_MODULES)

# Generate file that automatically registers all enabled modules
foreach (module ${ENABLED_MODULES})
    # Set up code to generate in config.hpp.in
    string(TOLOWER ${module} lModule)
    set(MODULE_INCLUDES "${MODULE_INCLUDES}#include \"modules/${lModule}/include/${lModule}.hpp\"\n")
    set(MODULE_APPENDS "${MODULE_APPENDS}modules.push_back(std::make_unique<nitro::${module}::${module}>());\n\t")

    add_subdirectory(modules/${lModule})

    # Link the module library to the project target
    target_link_libraries(${PROJECT_NAME}
            PUBLIC
            ${MODULE_NAME}
            PRIVATE
            ${MODULE_LIBS}
            )

    # Add the include directories of the main NITRO project to the module
    target_include_directories(${MODULE_NAME}
            PUBLIC
            ${NITRO_INCLUDE_DIRS}
            )

    # Link libraries by both nitro and from the external/directory
    target_link_libraries(${MODULE_NAME}
            PUBLIC
            ${EXTERNAL_LIBS}

            PRIVATE
            ${NITRO_LIBS}
            )

    # Convert the relative paths of the resource files to absolute paths
    foreach (resource_file ${MODULE_RESOURCES})
        set(ABSOLUTE_RESOURCE_FILE "${PROJECT_SOURCE_DIR}/modules/${lModule}/${resource_file}")
        list(APPEND ABSOLUTE_RESOURCE_FILES ${ABSOLUTE_RESOURCE_FILE})
    endforeach ()
    target_sources(${PROJECT_NAME} PRIVATE ${ABSOLUTE_RESOURCE_FILES})

endforeach ()

configure_file(include/config.hpp.in config.hpp)
