enable_testing()

find_package(GTest CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(crs)
add_subdirectory(parser)
add_subdirectory(rule)

add_executable(test $<TARGET_OBJECTS:crs> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:rule>)
# add_executable(test $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:rule>)
target_link_libraries(test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${CMAKE_BINARY_DIR}/src/libsrsecurity.a)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(test PRIVATE spdlog::spdlog_header_only)

find_package(pcre2 CONFIG REQUIRED)
target_link_libraries(test PRIVATE PCRE2::8BIT)
