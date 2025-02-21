cmake_minimum_required(VERSION 3.28)
project(srsecurity VERSION 0.0.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

if(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
  add_compile_options(-fno-omit-frame-pointer)
endif()

# Enable logging
add_definitions(-DSRSECURITY_LOG_ACTIVE_LEVEL=1)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmarks/srsecurity)
add_subdirectory(benchmarks/modsecurity)
add_dependencies(test srsecurity)
add_dependencies(srsecurity_benchmark srsecurity)