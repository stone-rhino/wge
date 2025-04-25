file(GLOB local_source
  *.h
  *.cc
)

find_package(spdlog CONFIG REQUIRED)
find_package(pcre2 CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(TCMALLOC REQUIRED libtcmalloc)
find_package(PkgConfig REQUIRED)
pkg_check_modules(HYPERSCAN REQUIRED libhs)
find_library(INJECTION_LIB injection)

if (NOT TARGET test_data)
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../test_data test_data)
endif()

# install the wge
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(BUILD_PATH build/debug)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
  set(BUILD_PATH build/release-with-debug-info)
else()
  set(BUILD_PATH build/release)
endif()
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/wge_install/lib/libwge.a
  COMMAND cmake --install ${BUILD_PATH} --prefix ${CMAKE_CURRENT_BINARY_DIR}/wge_install
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_custom_target(wge_install_target
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/wge_install/lib/libwge.a
)

# whitout tcmalloc
add_executable(wge_benchmark ${local_source})
add_dependencies(wge_benchmark wge_install_target)
target_include_directories(wge_benchmark PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/wge_install/include)
target_link_libraries(wge_benchmark PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/wge_install/lib/libwge.a)
target_link_libraries(wge_benchmark PRIVATE spdlog::spdlog_header_only)
target_link_libraries(wge_benchmark PRIVATE PCRE2::8BIT)
target_link_directories(wge_benchmark PRIVATE ${HYPERSCAN_LIBRARY_DIRS})
target_link_libraries(wge_benchmark PRIVATE ${HYPERSCAN_LIBRARIES})
target_link_libraries(wge_benchmark PRIVATE ${INJECTION_LIB})
target_link_libraries(wge_benchmark PRIVATE test_data)



# with tcmalloc
add_executable(wge_benchmark_tcmalloc ${local_source})
add_dependencies(wge_benchmark_tcmalloc wge_install_target)
target_include_directories(wge_benchmark_tcmalloc PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/wge_install/include)
target_link_libraries(wge_benchmark_tcmalloc PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/wge_install/lib/libwge.a)
target_link_libraries(wge_benchmark_tcmalloc PRIVATE spdlog::spdlog_header_only)
target_link_libraries(wge_benchmark_tcmalloc PRIVATE PCRE2::8BIT)
target_link_directories(wge_benchmark_tcmalloc PRIVATE ${TCMALLOC_LIBRARY_DIRS})
target_link_libraries(wge_benchmark_tcmalloc PRIVATE ${TCMALLOC_LIBRARIES})
target_link_directories(wge_benchmark_tcmalloc PRIVATE ${HYPERSCAN_LIBRARY_DIRS})
target_link_libraries(wge_benchmark_tcmalloc PRIVATE ${HYPERSCAN_LIBRARIES})
target_link_libraries(wge_benchmark_tcmalloc PRIVATE ${INJECTION_LIB})
target_link_libraries(wge_benchmark_tcmalloc PRIVATE test_data)

