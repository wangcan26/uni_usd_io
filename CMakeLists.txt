cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10)

project(uni_usd_io)

set(UNI_USD_IO ${CMAKE_CURRENT_SOURCE_DIR})

#module path
list(APPEND CMAKE_MODULE_PATH
    ${UNI_USD_IO}/cmake/defaults
    ${UNI_USD_IO}/cmake/modules)

include(CXXDefaults)
add_definitions(${_UNI_CXX_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${_UNI_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")

include(ProjectDefaults)

#platform
set(PLATFORM_WIN32             FALSE CACHE INTERNAL "")
set(PLATFORM_ANDROID           FALSE CACHE INTERNAL "")
set(PLATFORM_MACOS             FALSE CACHE INTERNAL "")

if(WIN32)
  add_definitions(-DUNI_PLATFORM_WINDOWS=1)
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
    message("USD Include dirs: ${USD_INCLUDE_DIRS}, libs: ${USD_LIBRARIES}, dynamic libs: ${USD_DYNAMIC_LIBRARIES}")
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
set(TBB_ROOT_DIR ${UNI_USD_IO}/third_party/tbb)
find_package(TBB)
if(TBB_FOUND)
    message("TBB Include dirs: ${TBB_INCLUDE_DIRS}, libs: ${TBB_LIBRARIES}, dynamics: ${TBB_DYNAMIC_LIBRARIES}")
    include_directories(${TBB_INCLUDE_DIRS})
endif()

#glm
set(GLM_ROOT_DIR ${UNI_USD_IO}/third_party/glm)
find_package(GLM)
if(GLM_FOUND)
  message("GLM Include dirs: ${GLM_INCLUDE_DIRS}")
  include_directories(${GLM_INCLUDE_DIRS})
endif()

#source
set(SOURCE_DIR ${UNI_USD_IO}/src)
set(SOURCES 
${SOURCE_DIR}/uni_settings.cc
  ${SOURCE_DIR}/uni_reader_stage.cc
  ${SOURCE_DIR}/uni_reader_prim.cc
  ${SOURCE_DIR}/uni_reader_xform.cc
  ${SOURCE_DIR}/uni_reader_geom.cc
  ${SOURCE_DIR}/uni_reader_mesh.cc
  ${SOURCE_DIR}/uni_scene_description.cc
  ${SOURCE_DIR}/utils/uni_math_matrix.cc
  )
include_directories(${SOURCE_DIR})

if(NOT PLATFORM_ANDROID)
    set(UNI_IO_TARGET uni_io)
    set(SOURCES main.cc ${SOURCES})
    add_executable(${UNI_IO_TARGET} ${SOURCES})
    target_link_libraries(${UNI_IO_TARGET}  ${UNI_LIBARIES} ${USD_LIBRARIES}  ${TBB_LIBRARIES} ${Python3_LIBRARIES}) #
    # ${BOOST_LIBRARIES}
endif()

if(MSVC)
    if(${UNI_SYSTEM_TYPE} STREQUAL "UNI_SYS_64")
        set(UNI_LIBARIES_FILES 
          "${UNI_USD_IO}/libs/windows/x64/api-ms-win-crt-locale-l1-1-0.dll"
          "${UNI_USD_IO}/libs/windows/x64/api-ms-win-crt-stdio-l1-1-0.dll"
          "${UNI_USD_IO}/libs/windows/x64/api-ms-win-crt-math-l1-1-0.dll"
          "${UNI_USD_IO}/libs/windows/x64/api-ms-win-crt-heap-l1-1-0.dll"
          "${UNI_USD_IO}/libs/windows/x64/api-ms-win-crt-runtime-l1-1-0.dll"
          )
    else()
        set(UNI_LIBARIES_FILES 
          "${UNI_USD_IO}/libs/windows/x86/api-ms-win-crt-locale-l1-1-0.dll"
          "${UNI_USD_IO}/libs/windows/x86/api-ms-win-crt-stdio-l1-1-0.dll"
          "${UNI_USD_IO}/libs/windows/x86/api-ms-win-crt-math-l1-1-0.dll"
          "${UNI_USD_IO}/libs/windows/x86/api-ms-win-crt-heap-l1-1-0.dll"
          "${UNI_USD_IO}/libs/windows/x86/api-ms-win-crt-runtime-l1-1-0.dll"
          )
    endif()
    set(UNI_LIBARIES_FILES ${UNI_LIBARIES_FILES} ${USD_DYNAMIC_LIBRARIES}  ${TBB_DYNAMIC_LIBRARIES})
    foreach(file ${UNI_LIBARIES_FILES})
      message("Copy file ${file}...")
      add_custom_command(TARGET ${UNI_IO_TARGET}
      POST_BUILD 
      COMMAND ${CMAKE_COMMAND} -E
      copy_if_different
      "$<$<CONFIG:Release>:${file}>"
      "${EXECUTABLE_OUTPUT_PATH}/$<$<CONFIG:Release>:Release>")
    endforeach()
    add_custom_command(TARGET ${UNI_IO_TARGET} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${UNI_USD_IO}/data/ "${EXECUTABLE_OUTPUT_PATH}/$<$<CONFIG:Release>:Release>/data")
    add_custom_command(TARGET ${UNI_IO_TARGET} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${USD_ROOT_DIR}/lib/windows/usd/ "${EXECUTABLE_OUTPUT_PATH}/$<$<CONFIG:Release>:Release>/usd")
endif()
