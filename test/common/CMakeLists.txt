file(GLOB local_source
  *.h
  *.cc
)

add_library(common_test ${local_source})
target_link_libraries(common_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${CMAKE_BINARY_DIR}/src/libwge.a)

find_package(LibXml2 REQUIRED)
target_link_libraries(common_test PRIVATE LibXml2::LibXml2)