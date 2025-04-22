file(GLOB local_source
  *.h
  *.cc
)

add_subdirectory(ragel)

add_library(transformation ${local_source})
target_link_libraries(transformation PRIVATE ragel)
target_include_directories(transformation PRIVATE ${CMAKE_BINARY_DIR}/src/transformation/ragel/ragel_gen)


find_package(spdlog CONFIG REQUIRED)
target_link_libraries(transformation PRIVATE spdlog::spdlog_header_only)
