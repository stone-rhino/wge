file(GLOB local_source
  *.h
  *.cc
)

add_library(operator ${local_source})

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(operator PRIVATE spdlog::spdlog_header_only)