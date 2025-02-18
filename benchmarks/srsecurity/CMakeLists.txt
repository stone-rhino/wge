file(GLOB local_source
  *.h
  *.cc
)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(srsecurity_benchmark ${local_source})
target_include_directories(srsecurity_benchmark PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(srsecurity_benchmark PRIVATE ${CMAKE_BINARY_DIR}/src/libsrsecurity.a)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(srsecurity_benchmark PRIVATE spdlog::spdlog_header_only)

find_package(pcre2 CONFIG REQUIRED)
target_link_libraries(srsecurity_benchmark PRIVATE PCRE2::8BIT)