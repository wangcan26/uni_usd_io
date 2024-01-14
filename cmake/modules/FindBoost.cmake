# - Find Boost library
# Find the native Boost includes and library
# This module defines
# BOOST_INCLUDE_DIRS, where to find Boost headers, Set when BOOST_INCLUDE_DIR is found.
# BOOST_LIBRARIES, libraries to link against to use Boost.
# BOOST_ROOT_DIR, the base directory to search for Boost.
# BOOST_FOUND, if false, do not try to use Boost.

#If 'BOOST_ROOT_DIR' was defined in the environment, use it.
if(DEFINED BOOST_ROOT_DIR)
    # Pass.
elseif(DEFINED ENV{BOOST_ROOT_DIR})
    set(BOOST_ROOT_DIR $ENV{BOOST_ROOT_DIR})
else()
    set(BOOST_ROOT_DIR "")
endif()

set(_boost_FIND_COMPONENTS
#    boost_atomic
#    boost_regex
     ${BOOST_TARGET_COMPONENT}
)


set(_boost_SEARCH_DIRS
    ${BOOST_ROOT_DIR}
)

find_path(BOOST_INCLUDE_DIR
    NAMES
        boost/atomic/atomic.hpp
    HINTS
        ${_boost_SEARCH_DIRS}
    PATH_SUFFIXES
        include
)
message("To Find Boost lib: ${_boost_FIND_COMPONENTS}")
set(_boost_LIBRARIES)
foreach(COMPONENT ${_boost_FIND_COMPONENTS})
    string(TOUPPER ${COMPONENT} UPPERCOMPONENT)

    find_library(BOOST_${UPPERCOMPONENT}_LIBRARY
        NAMES
            ${COMPONENT}
        HINTS
            ${_boost_SEARCH_DIRS}
        PATH_SUFFIXES
            lib lib/${UNI_LIB_PATH_SUFFIX}
    )
    message("Find Boost lib: ${BOOST_${UPPERCOMPONENT}_LIBRARY}")
    list(APPEND _boost_LIBRARIES "${BOOST_${UPPERCOMPONENT}_LIBRARY}")
endforeach(COMPONENT ${_boost_FIND_COMPONENTS})

# handle the QUIETLY and REQUIRED arguments and set USD_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_Package_handle_standard_args(Boost DEFAULT_MSG BOOST_INCLUDE_DIR)

if(BOOST_FOUND)
    set(BOOST_LIBRARIES ${_boost_LIBRARIES})
    set(BOOST_INCLUDE_DIRS ${BOOST_INCLUDE_DIR})
endif()

mark_as_advanced(BOOST_INCLUDE_DIR)

foreach(COMPONENT ${_boost_LIBRARIES})
    string(TOUPPER ${COMPONENT} UPPERCOMPONENT)
    mark_as_advanced(BOOST_${UPPERCOMPONENT}_LIBRARY)
endforeach()




