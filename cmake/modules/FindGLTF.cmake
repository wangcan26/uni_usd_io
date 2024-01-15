# - Find GLTF library
# Find the cgltf headers
# This module defines
# GLTF_INCLUDE_DIRS, where to find cgltf headers, Set when GLTF_INCLUDE_DIR is found.
# GLTF_ROOT_DIR, the base directory to search for GLTF.
# GLTF_FOUND, if false, do not try to use GLTF.

#If 'JSON_ROOT_DIR' was defined in the environment, use it.
if(DEFINED GLTF_ROOT_DIR)
    # Pass.
elseif(DEFINED ENV{GLTF_ROOT_DIR})
    set(GLTF_ROOT_DIR $ENV{GLTF_ROOT_DIR})
else()
    set(GLTF_ROOT_DIR "")
endif()

set(_gltf_SEARCH_DIRS
    ${GLTF_ROOT_DIR}
)

find_path(GLTF_INCLUDE_DIR
    NAMES
        cgltf.h
    HINTS
        ${_gltf_SEARCH_DIRS}
)

# handle the QUIETLY and REQUIRED arguments and set USD_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)

find_Package_handle_standard_args(GLTF DEFAULT_MSG GLTF_INCLUDE_DIR)

if(GLTF_FOUND)
    set(GLTF_INCLUDE_DIRS ${GLTF_INCLUDE_DIR})
endif()

mark_as_advanced(GLTF_INCLUDE_DIR)