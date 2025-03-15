file(GLOB local_source
  *.h
  *.cc
)

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(spdlog CONFIG REQUIRED)
find_package(pcre2 CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(TCMALLOC REQUIRED libtcmalloc)
find_package(PkgConfig REQUIRED)
pkg_check_modules(HYPERSCAN REQUIRED libhs)
find_package(boost_url CONFIG REQUIRED)

# whitout tcmalloc
add_executable(srsecurity_benchmark ${local_source})
target_include_directories(srsecurity_benchmark PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(srsecurity_benchmark PRIVATE ${CMAKE_BINARY_DIR}/src/libsrsecurity.a)
target_link_libraries(srsecurity_benchmark PRIVATE spdlog::spdlog_header_only)
target_link_libraries(srsecurity_benchmark PRIVATE PCRE2::8BIT)
target_include_directories(srsecurity_benchmark PRIVATE ${HYPERSCAN_INCLUDE_DIRS})
target_link_directories(srsecurity_benchmark PRIVATE ${HYPERSCAN_LIBRARY_DIRS})
target_link_libraries(srsecurity_benchmark PRIVATE ${HYPERSCAN_LIBRARIES})
target_link_libraries(srsecurity_benchmark PRIVATE Boost::url)


# with tcmalloc
add_executable(srsecurity_benchmark_tcmalloc ${local_source})
target_include_directories(srsecurity_benchmark_tcmalloc PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(srsecurity_benchmark_tcmalloc PRIVATE ${CMAKE_BINARY_DIR}/src/libsrsecurity.a)
target_link_libraries(srsecurity_benchmark_tcmalloc PRIVATE spdlog::spdlog_header_only)
target_link_libraries(srsecurity_benchmark_tcmalloc PRIVATE PCRE2::8BIT)
target_link_directories(srsecurity_benchmark_tcmalloc PRIVATE ${TCMALLOC_LIBRARY_DIRS})
target_link_libraries(srsecurity_benchmark_tcmalloc PRIVATE ${TCMALLOC_LIBRARIES})
target_include_directories(srsecurity_benchmark_tcmalloc PRIVATE ${HYPERSCAN_INCLUDE_DIRS})
target_link_directories(srsecurity_benchmark_tcmalloc PRIVATE ${HYPERSCAN_LIBRARY_DIRS})
target_link_libraries(srsecurity_benchmark_tcmalloc PRIVATE ${HYPERSCAN_LIBRARIES})
target_link_libraries(srsecurity_benchmark_tcmalloc PRIVATE Boost::url)
