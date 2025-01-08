file(GLOB local_source
  *.h
  *.cc
)

add_library(rule ${local_source})
target_link_libraries(rule PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${CMAKE_BINARY_DIR}/src/libsrsecurity.a)