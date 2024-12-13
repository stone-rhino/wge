file(GLOB local_source
  *.h
  *.cc
)

add_executable(parser_test ${local_source})
target_link_directories(parser_test PRIVATE ${CMAKE_BINARY_DIR}/src)
target_link_libraries(parser_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
target_link_libraries(parser_test PRIVATE libsrsecurity.a)