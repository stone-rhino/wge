file(GLOB local_source
  *.h
  *.cc
)

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(spdlog CONFIG REQUIRED)
find_package(pcre2 CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(TCMALLOC REQUIRED libtcmalloc)

# whitout tcmalloc
add_executable(srsecurity_benchmark ${local_source})
target_include_directories(srsecurity_benchmark PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(srsecurity_benchmark PRIVATE ${CMAKE_BINARY_DIR}/src/libsrsecurity.a)
target_link_libraries(srsecurity_benchmark PRIVATE spdlog::spdlog_header_only)
target_link_libraries(srsecurity_benchmark PRIVATE PCRE2::8BIT)

# with tcmalloc
add_executable(srsecurity_benchmark_tcmalloc ${local_source})
target_include_directories(srsecurity_benchmark_tcmalloc PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(srsecurity_benchmark_tcmalloc PRIVATE ${CMAKE_BINARY_DIR}/src/libsrsecurity.a)
target_link_libraries(srsecurity_benchmark_tcmalloc PRIVATE spdlog::spdlog_header_only)
target_link_libraries(srsecurity_benchmark_tcmalloc PRIVATE PCRE2::8BIT)
target_link_directories(srsecurity_benchmark_tcmalloc PRIVATE ${TCMALLOC_LIBRARY_DIRS})
target_link_libraries(srsecurity_benchmark_tcmalloc PRIVATE ${TCMALLOC_LIBRARIES})