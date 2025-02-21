file(GLOB local_source
  *.h
  *.cc
)

add_library(macro ${local_source})

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(macro PRIVATE spdlog::spdlog_header_only)