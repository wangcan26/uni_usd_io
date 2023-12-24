# - Find TBB library
# Find the native TBB includes and library
# This module defines
# TBB_INCLUDE_DIRS, where to find TBB headers, Set when TBB_INCLUDE_DIR is found.
# TBB_LIBRARIES, libraries to link against to use TBB.
# TBB_ROOT_DIR, the base directory to search for TBB.
# TBB_FOUND, if false, do not try to use TBB.

#If 'TBB_ROOT_DIR' was defined in the environment, use it.
if(DEFINED TBB_ROOT_DIR)
    # Pass.
elseif(DEFINED ENV{TBB_ROOT_DIR})
    set(TBB_ROOT_DIR $ENV{TBB_ROOT_DIR})
else()
    set(TBB_ROOT_DIR "")
endif()

set(TBB_TARGET_COMPONENT tbb)
if(WIN32)
    set(TBB_TARGET_COMPONENT tbb12)
endif()

set(_tbb_FIND_COMPONENTS
    ${TBB_TARGET_COMPONENT}
)

set(_tbb_SEARCH_DIRS
    ${TBB_ROOT_DIR}
)

find_path(TBB_INCLUDE_DIR
    NAMES
        oneapi/tbb/version.h
    HINTS
        ${_tbb_SEARCH_DIRS}
    PATH_SUFFIXES
        include
)

set(_tbb_LIBRARIES)
set(_tbb_DYNAMIC_LIBRARIES)
foreach(COMPONENT ${_tbb_FIND_COMPONENTS})
    string(TOUPPER ${COMPONENT} UPPERCOMPONENT)
    find_library(TBB_${UPPERCOMPONENT}_LIBRARY
        NAMES
            ${COMPONENT}
        HINTS
            ${_tbb_SEARCH_DIRS}
        PATH_SUFFIXES
            lib lib/${UNI_LIB_PATH_SUFFIX}
    )
    get_filename_component(_tbb_FILE_DIR ${TBB_${UPPERCOMPONENT}_LIBRARY} DIRECTORY)
    set(_tbb_DYNAMIC_LIBRARIES ${_tbb_DYNAMIC_LIBRARIES} ${_tbb_FILE_DIR}/${COMPONENT}.${UNI_DYNAMIC_LIB_SUFFIX})
    list(APPEND _tbb_LIBRARIES "${TBB_${UPPERCOMPONENT}_LIBRARY}")
endforeach(COMPONENT ${_tbb_FIND_COMPONENTS})

# handle the QUIETLY and REQUIRED arguments and set USD_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_Package_handle_standard_args(TBB DEFAULT_MSG _tbb_LIBRARIES TBB_INCLUDE_DIR)

if(TBB_FOUND)
    set(TBB_LIBRARIES ${_tbb_LIBRARIES})
    set(TBB_INCLUDE_DIRS ${TBB_INCLUDE_DIR})  
    set(TBB_DYNAMIC_LIBRARIES ${_tbb_DYNAMIC_LIBRARIES})
endif()
mark_as_advanced(TBB_INCLUDE_DIR)

foreach(COMPONENT ${_tbb_LIBRARIES})
    string(TOUPPER ${COMPONENT} UPPERCOMPONENT)
    mark_as_advanced(TBB_${UPPERCOMPONENT}_LIBRARY)
endforeach()




