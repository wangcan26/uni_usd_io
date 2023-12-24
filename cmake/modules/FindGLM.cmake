# - Find GLM library
# Find the native GLM includes and library
# This module defines
# GLM_INCLUDE_DIRS, where to find GLM headers, Set when GLM_INCLUDE_DIR is found.
# GLM_LIBRARIES, libraries to link against to use GLM.
# GLM_ROOT_DIR, the base directory to search for GLM.
# GLM_FOUND, if false, do not try to use GLM.

#If 'GLM_ROOT_DIR' was defined in the environment, use it.
if(DEFINED GLM_ROOT_DIR)
    # Pass.
elseif(DEFINED ENV{GLM_ROOT_DIR})
    set(GLM_ROOT_DIR $ENV{GLM_ROOT_DIR})
else()
    set(GLM_ROOT_DIR "")
endif()

set(_tbb_SEARCH_DIRS
    ${TBB_ROOT_DIR}
)

find_path(GLM_INCLUDE_DIR
    NAMES
        glm/glm.hpp
    HINTS
        ${_tbb_SEARCH_DIRS}
    PATH_SUFFIXES
        include
)

# handle the QUIETLY and REQUIRED arguments and set USD_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)

find_Package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)

if(GLM_FOUND)
    set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})
endif()

mark_as_advanced(GLM_INCLUDE_DIR)