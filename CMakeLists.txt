cmake_minimum_required(VERSION 3.28)
project(wge VERSION 0.0.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

if(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
  add_compile_options(-fno-omit-frame-pointer)
endif()

# Enable logging
set(WGE_LOG_ACTIVE_LEVEL 7 CACHE STRING "Set the active log level (1: trace, 2: debug, 3: info, 4: warn, 5: error, 6: critical, 7: off)")
if(WGE_LOG_ACTIVE_LEVEL LESS 7)
  add_definitions(-DWGE_LOG_ACTIVE_LEVEL=${WGE_LOG_ACTIVE_LEVEL})
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug"  AND WGE_LOG_ACTIVE_LEVEL EQUAL 7)
  add_definitions(-DWGE_LOG_ACTIVE_LEVEL=1)
endif()


add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmarks/wge)
add_subdirectory(benchmarks/modsecurity)
add_dependencies(test wge)
add_dependencies(wge_install_target wge)

install(
  TARGETS wge
)

install(
  DIRECTORY src/
  DESTINATION include/wge
  FILES_MATCHING PATTERN "*.h"
)