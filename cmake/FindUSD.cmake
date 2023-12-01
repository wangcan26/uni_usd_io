# - Find USD library
# Find the native USD includes and library
# This module defines
# USD_INCLUDE_DIRS, where to find USD headers, Set when USD_INCLUDE_DIR is found.
# USD_LIBRARIES, libraries to link against to use USD.
# USD_ROOT_DIR, the base directory to search for USD.
# USD_FOUND, if false, do not try to use USD.

#If 'USD_ROOT_DIR' was defined in the environment, use it.
if(DEFINED USD_ROOT_DIR)
    # Pass.
elseif(DEFINED ENV{USD_ROOT_DIR})
    set(USD_ROOT_DIR $ENV{USD_ROOT_DIR})
else()
    set(USD_ROOT_DIR "")
endif()

set(_usd_FIND_COMPONENTS
    usd_ms
)

set(_usd_SEARCH_DIRS
    ${USD_ROOT_DIR}
)

find_path(USD_INCLUDE_DIR
    NAMES
        pxr/pxr.h
    HINTS
        ${_usd_SEARCH_DIRS}
    PATH_SUFFIXES
        include
)

set(_usd_LIBRARIES)
foreach(COMPONENT ${_usd_FIND_COMPONENTS})
    string(TOUPPER ${COMPONENT} UPPERCOMPONENT)

    find_library(USD_${UPPERCOMPONENT}_LIBRARY
        NAMES
            ${COMPONENT}
        HINTS
            ${_usd_SEARCH_DIRS}
        PATH_SUFFIXES
            lib lib/mac
    )
    list(APPEND _usd_LIBRARIES "${USD_${UPPERCOMPONENT}_LIBRARY}")
endforeach(COMPONENT ${_usd_FIND_COMPONENTS})

# handle the QUIETLY and REQUIRED arguments and set USD_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_Package_handle_standard_args(USD DEFAULT_MSG _usd_LIBRARIES USD_INCLUDE_DIR)

if(USD_FOUND)
    set(USD_LIBRARIES ${_usd_LIBRARIES})
    set(USD_INCLUDE_DIRS ${USD_INCLUDE_DIR})
endif()

mark_as_advanced(USD_INCLUDE_DIR)

foreach(COMPONENT ${_usd_LIBRARIES})
    string(TOUPPER ${COMPONENT} UPPERCOMPONENT)
    mark_as_advanced(USD_${UPPERCOMPONENT}_LIBRARY)
endforeach()




