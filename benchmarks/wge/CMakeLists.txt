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
find_library(INJECTION_LIB injection)

if (NOT TARGET test_data)
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../test_data test_data)
endif()

# whitout tcmalloc
add_executable(wge_benchmark ${local_source})
target_include_directories(wge_benchmark PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(wge_benchmark PRIVATE ${CMAKE_BINARY_DIR}/src/libwge.a)
target_link_libraries(wge_benchmark PRIVATE spdlog::spdlog_header_only)
target_link_libraries(wge_benchmark PRIVATE PCRE2::8BIT)
target_include_directories(wge_benchmark PRIVATE ${HYPERSCAN_INCLUDE_DIRS})
target_link_directories(wge_benchmark PRIVATE ${HYPERSCAN_LIBRARY_DIRS})
target_link_libraries(wge_benchmark PRIVATE ${HYPERSCAN_LIBRARIES})
target_link_libraries(wge_benchmark PRIVATE Boost::url)
target_link_libraries(wge_benchmark PRIVATE ${INJECTION_LIB})
target_link_libraries(wge_benchmark PRIVATE test_data)



# with tcmalloc
add_executable(wge_benchmark_tcmalloc ${local_source})
target_include_directories(wge_benchmark_tcmalloc PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(wge_benchmark_tcmalloc PRIVATE ${CMAKE_BINARY_DIR}/src/libwge.a)
target_link_libraries(wge_benchmark_tcmalloc PRIVATE spdlog::spdlog_header_only)
target_link_libraries(wge_benchmark_tcmalloc PRIVATE PCRE2::8BIT)
target_link_directories(wge_benchmark_tcmalloc PRIVATE ${TCMALLOC_LIBRARY_DIRS})
target_link_libraries(wge_benchmark_tcmalloc PRIVATE ${TCMALLOC_LIBRARIES})
target_include_directories(wge_benchmark_tcmalloc PRIVATE ${HYPERSCAN_INCLUDE_DIRS})
target_link_directories(wge_benchmark_tcmalloc PRIVATE ${HYPERSCAN_LIBRARY_DIRS})
target_link_libraries(wge_benchmark_tcmalloc PRIVATE ${HYPERSCAN_LIBRARIES})
target_link_libraries(wge_benchmark_tcmalloc PRIVATE Boost::url)
target_link_libraries(wge_benchmark_tcmalloc PRIVATE ${INJECTION_LIB})
target_link_libraries(wge_benchmark_tcmalloc PRIVATE test_data)

