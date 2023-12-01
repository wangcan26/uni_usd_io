cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(uni_usd_io)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(UNI_USD_IO ${CMAKE_CURRENT_SOURCE_DIR})

#module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${UNI_USD_IO}/cmake)

#build type
if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

#platform
set(PLATFORM_WIN32             FALSE CACHE INTERNAL "")
set(PLATFORM_ANDROID           FALSE CACHE INTERNAL "")
set(PLATFORM_MACOS             FALSE CACHE INTERNAL "")

if(WIN32)
else()
  if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    add_definitions(-DUNI_PLATFORM_ANDROID=1)
    set(PLATFORM_ANDROID TRUE)
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    add_definitions(-DUNI_PLATFORM_MACOS=1)
    set(PLATFORM_MACOS TRUE)
  else()
    message(FATAL_ERROR "Unsupported platform")
  endif()
endif(WIN32)

#Find Dependencies
#python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
message("Python_FOUND: ${Python3_FOUND}")
message("Python_VERSION: ${Python3_VERSION}")
if(Python3_FOUND)
    include_directories(${Python3_INCLUDE_DIRS})
    message("Python_LIBRARIES: ${Python3_LIBRARIES}")
endif()

#usd
set(USD_ROOT_DIR ${UNI_USD_IO}/third_party/usd)
find_package(USD)
if(USD_FOUND)
    message("USD Include dirs: ${USD_INCLUDE_DIRS}, libs: ${USD_LIBRARIES}")
    include_directories(${USD_INCLUDE_DIRS})
    add_definitions(-DPXR_ONETBB_SUPPORT_ENABLED)
endif()

#boost
set(BOOST_ROOT_DIR ${UNI_USD_IO}/third_party/boost)
find_package(Boost)
if(BOOST_FOUND)
    message("Boost Include dirs: ${BOOST_INCLUDE_DIRS}, libs: ${BOOST_LIBRARIES}")
    include_directories(${BOOST_INCLUDE_DIRS})
endif()

#tbb
#boost
set(TBB_ROOT_DIR ${UNI_USD_IO}/third_party/tbb)
find_package(TBB)
if(TBB_FOUND)
    message("TBB Include dirs: ${TBB_INCLUDE_DIRS}, libs: ${TBB_LIBRARIES}")
    include_directories(${TBB_INCLUDE_DIRS})
endif()

#source
set(SOURCE_DIR ${UNI_USD_IO}/src)
set(SOURCES 
  ${SOURCE_DIR}/uni_reader_stage.cc
  ${SOURCE_DIR}/uni_reader_prim.cc
  ${SOURCE_DIR}/uni_reader_xform.cc
  ${SOURCE_DIR}/uni_reader_geom.cc
  ${SOURCE_DIR}/uni_reader_mesh.cc
  ${SOURCE_DIR}/uni_scene_description.cc
  )
include_directories(${SOURCE_DIR})

if(NOT PLATFORM_ANDROID)
    set(UNI_IO_TARGET uni_io)
    set(SOURCES main.cc ${SOURCES})
    add_executable(${UNI_IO_TARGET} ${SOURCES})
    target_link_libraries(${UNI_IO_TARGET} ${USD_LIBRARIES} ${BOOST_LIBRARIES} ${TBB_LIBRARIES} ${Python3_LIBRARIES})
endif()
