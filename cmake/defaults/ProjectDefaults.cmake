if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(UNI_SYSTEM_TYPE "UNI_SYS_64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(UNI_SYSTEM_TYPE "UNI_SYS_32")
endif()

if(MSVC)
    set(UNI_LIB_PATH_SUFFIX "windows")
    set(UNI_DYNAMIC_LIB_SUFFIX "dll")
elseif(APPLE)
    set(UNI_LIB_PATH_SUFFIX "mac")
    set(UNI_DYNAMIC_LIB_SUFFIX "dylib")
endif()

#if (MSVC)
#    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll")
#elseif (APPLE)
#    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
#endif()

#build type
if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}) 
message("Uni Ouput Path: ${EXECUTABLE_OUTPUT_PATH}")