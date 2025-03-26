file(GLOB local_source
  *.h
  *.cc
)

add_library(operator ${local_source})

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(operator PRIVATE spdlog::spdlog_header_only)

find_package(PkgConfig REQUIRED)
pkg_check_modules(HYPERSCAN REQUIRED libhs)
message(STATUS "HYPERSCAN_INCLUDE_DIRS: ${HYPERSCAN_INCLUDE_DIRS}")
message(STATUS "HYPERSCAN_LIBRARY_DIRS: ${HYPERSCAN_LIBRARY_DIRS}")
message(STATUS "HYPERSCAN_LIBRARIES: ${HYPERSCAN_LIBRARIES}")
target_include_directories(operator PRIVATE ${HYPERSCAN_INCLUDE_DIRS})
target_link_directories(operator PRIVATE ${HYPERSCAN_LIBRARY_DIRS})
target_link_libraries(operator PRIVATE ${HYPERSCAN_LIBRARIES})

find_library(INJECTION_LIB injection)
target_link_libraries(operator PRIVATE ${INJECTION_LIB})