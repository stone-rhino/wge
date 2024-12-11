file(GLOB local_source
  *.h
  *.cc
)

find_package(spdlog CONFIG REQUIRED)
add_library(common ${local_source})
target_link_libraries(common PRIVATE spdlog::spdlog_header_only)