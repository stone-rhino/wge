file(GLOB local_source
  *.h
  *.cc
)

find_package(Boost REQUIRED COMPONENTS url)
add_library(transformation ${local_source})
target_link_libraries(transformation PRIVATE Boost::url)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(transformation PRIVATE spdlog::spdlog_header_only)
