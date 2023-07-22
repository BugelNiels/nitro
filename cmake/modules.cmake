# Specifies that this module relies on another module. It checks whether this other module is then enabled or not.
# Note that it does not automatically enable the required module.
function(requires_module REQUIRED_MODULE)
    if (NOT ${REQUIRED_MODULE} IN_LIST ENABLED_MODULES)
        message(FATAL_ERROR "Module dependency ${REQUIRED_MODULE} was not explicitly enabled. However, ${MODULE_NAME} requires it.\nAdd the ${REQUIRED_MODULE} module to ENABLED_MODULES to ensure ${MODULE_NAME} can be compiled successfully.")
    endif ()
endfunction()

# Enables the given module. The same module cannot be enabled more than once.
function(add_module NEW_MODULE)
    if (NOT ${NEW_MODULE} IN_LIST ENABLED_MODULES)
        list(APPEND ENABLED_MODULES ${NEW_MODULE})
        set(ENABLED_MODULES ${ENABLED_MODULES} PARENT_SCOPE)
    else ()
        message(WARNING "${NEW_MODULE} is already enabled.")
    endif ()
endfunction()


