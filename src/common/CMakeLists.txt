file(GLOB local_source
  *.h
  *.cc
  hyperscan/*.h
  hyperscan/*.cc
  pcre/*.h
  pcre/*.cc
  re2/*.h
  re2/*.cc
  literal_match/*.h
  literal_match/*.cc
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

find_package(re2 CONFIG REQUIRED)
target_link_libraries(common PRIVATE re2::re2)