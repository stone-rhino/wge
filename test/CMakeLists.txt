enable_testing()

find_package(GTest CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(common)
add_subdirectory(parser)
add_subdirectory(transformation)
add_subdirectory(integration)

add_executable(test $<TARGET_OBJECTS:common_test> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:transformation_test> $<TARGET_OBJECTS:integration>)
# add_executable(test $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:rule>)
target_link_libraries(test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${CMAKE_BINARY_DIR}/src/libwge.a)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(test PRIVATE spdlog::spdlog_header_only)

find_package(pcre2 CONFIG REQUIRED)
target_link_libraries(test PRIVATE PCRE2::8BIT)

find_package(PkgConfig REQUIRED)
pkg_check_modules(HYPERSCAN REQUIRED libhs)
target_include_directories(test PRIVATE ${HYPERSCAN_INCLUDE_DIRS})
target_link_directories(test PRIVATE ${HYPERSCAN_LIBRARY_DIRS})
target_link_libraries(test PRIVATE ${HYPERSCAN_LIBRARIES})

find_package(boost_url CONFIG REQUIRED)
target_link_libraries(test PRIVATE Boost::url)

find_library(INJECTION_LIB injection)
target_link_libraries(test PRIVATE ${INJECTION_LIB})

find_package(LibXml2 REQUIRED)
target_link_libraries(test PRIVATE LibXml2::LibXml2)