file(GLOB local_source
  *.h
  *.cc
)

add_library(action ${local_source})

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(action PRIVATE spdlog::spdlog_header_only)