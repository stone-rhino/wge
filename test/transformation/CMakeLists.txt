file(GLOB local_source
  *.h
  *.cc
)

add_library(transformation_test ${local_source})
target_link_libraries(transformation_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${CMAKE_BINARY_DIR}/src/libwge.a)
