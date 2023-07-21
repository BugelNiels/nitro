
# Generate file that automatically registers all enabled modules
foreach (module ${ENABLED_MODULES})
    string(TOLOWER ${module} lModule)
    set(MODULE_INCLUDES "${MODULE_INCLUDES}#include \"modules/${lModule}/include/${lModule}.hpp\"\n")
    set(MODULE_APPENDS "${MODULE_APPENDS}modules.push_back(std::make_unique<nitro::${module}::${module}>());\n\t")
    add_subdirectory(modules/${lModule})
endforeach ()

configure_file(include/config.hpp.in config.hpp)
