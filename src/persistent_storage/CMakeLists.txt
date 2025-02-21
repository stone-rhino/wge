file(GLOB local_source
  *.h
  *.cc
)

add_library(persistent_storage ${local_source})

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(persistent_storage PRIVATE spdlog::spdlog_header_only)