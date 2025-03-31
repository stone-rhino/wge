file(GLOB local_source
  *.h
  *.cc
  hyperscan/*.h
  hyperscan/*.cc
  pcre/*.h
  pcre/*.cc
)


add_subdirectory(ragel)
add_library(common ${local_source} $<TARGET_OBJECTS:common_ragel>)

find_package(spdlog CONFIG REQUIRED)
find_package(pcre2 CONFIG REQUIRED)
target_link_libraries(common PRIVATE spdlog::spdlog_header_only PCRE2::8BIT)

find_package(PkgConfig REQUIRED)
pkg_check_modules(HYPERSCAN REQUIRED libhs)
target_include_directories(common PRIVATE ${HYPERSCAN_INCLUDE_DIRS})
target_link_directories(common PRIVATE ${HYPERSCAN_LIBRARY_DIRS})
target_link_libraries(common PRIVATE ${HYPERSCAN_LIBRARIES})