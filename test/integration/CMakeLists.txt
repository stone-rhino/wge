file(GLOB local_source
  variable.cc
  rule_operator_test.cc
  rule_action_test.cc
  rule_evaluate_logic_test.cc
  crs_test.cc
)

add_library(integration ${local_source})
target_link_libraries(integration PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${CMAKE_BINARY_DIR}/src/libsrsecurity.a)