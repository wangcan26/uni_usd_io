# - Find JSON library
# Find the nlohmann JSON includes
# This module defines
# JSON_INCLUDE_DIRS, where to find JSON headers, Set when JSON_INCLUDE_DIR is found.
# JSON_ROOT_DIR, the base directory to search for JSON.
# JSON_FOUND, if false, do not try to use JSON.

#If 'JSON_ROOT_DIR' was defined in the environment, use it.
if(DEFINED JSON_ROOT_DIR)
    # Pass.
elseif(DEFINED ENV{JSON_ROOT_DIR})
    set(JSON_ROOT_DIR $ENV{JSON_ROOT_DIR})
else()
    set(JSON_ROOT_DIR "")
endif()

set(_json_SEARCH_DIRS
    ${JSON_ROOT_DIR}
)

find_path(JSON_INCLUDE_DIR
    NAMES
        nlohmann/json.hpp
    HINTS
        ${_json_SEARCH_DIRS}
    PATH_SUFFIXES
        include
)

# handle the QUIETLY and REQUIRED arguments and set USD_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)

find_Package_handle_standard_args(JSON DEFAULT_MSG JSON_INCLUDE_DIR)

if(JSON_FOUND)
    set(JSON_INCLUDE_DIRS ${JSON_INCLUDE_DIR})
endif()

mark_as_advanced(JSON_INCLUDE_DIR)