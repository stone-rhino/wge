file(GLOB local_source
  *.h
  *.cc
)

add_library(variable ${local_source})

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(variable PRIVATE spdlog::spdlog_header_only)