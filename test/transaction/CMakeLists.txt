file(GLOB local_source
  *.h
  *.cc
)

add_library(transaction_test ${local_source})
target_link_libraries(transaction_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${CMAKE_BINARY_DIR}/src/libwge.a)
