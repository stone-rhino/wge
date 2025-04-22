file(GLOB local_source
  *.h
  *.cc
)

add_library(integration ${local_source})
target_link_libraries(integration PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${CMAKE_BINARY_DIR}/src/libwge.a)