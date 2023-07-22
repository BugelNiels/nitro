# Remove any modules added more than once
list(REMOVE_DUPLICATES ENABLED_MODULES)

# Generate file that automatically registers all enabled modules
foreach (module ${ENABLED_MODULES})
    # Reset the libraries required by the current module
    set(MODULE_LIBS "")
    # Set up code to generate in config.hpp.in
    # The name of the module's header file is assumed to be identical to the name of the module (lowercase).
    # e.g. the module ImCore expects a header file called imcore.hpp.
    # The modules are expected to reside in the modules/ directory and the module directory itself should also be the lowercase version of the module name.
    string(TOLOWER ${module} lModule)
    set(MODULE_INCLUDES "${MODULE_INCLUDES}#include \"modules/${lModule}/include/${lModule}.hpp\"\n")
    # Add the module at runtime
    set(MODULE_APPENDS "${MODULE_APPENDS}modules.push_back(std::make_unique<nitro::${module}::${module}>());\n\t")

    # Ensure we call the CMakeLists.txt of the corresponding module
    add_subdirectory(modules/${lModule})

    # Link the module library to the project target (nitro)
    target_link_libraries(${PROJECT_NAME}
            PUBLIC
            ${MODULE_NAME}
            )

    # Add the include directories of the main NITRO project to the module
    target_include_directories(${MODULE_NAME}
            PUBLIC
            ${NITRO_INCLUDE_DIRS}
            )

    # Link libraries used by nitro and any libraries from the external directory to the module
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

# Create the config.hpp file containing the runtime code to register the modules
configure_file(include/config.hpp.in config.hpp)
