file(GLOB local_source
  *.h
  *.cc
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/bindings/python/modsecurity
  ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/others/libinjection
  ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/others/mbedtls
  COMMAND git submodule update --init
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/configure
  COMMAND ./build.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/bindings/python/modsecurity
  ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/others/libinjection
  ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/others/mbedtls
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/config.log
  COMMAND ./configure --disable-debug-logs --prefix=${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/configure
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/lib/libmodsecurity.a
  COMMAND make -j8 && make install
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/config.log
)

add_custom_target(build
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/lib/libmodsecurity.a
)

add_executable(modsecurity_benchmark ${local_source})
add_dependencies(modsecurity_benchmark build)
target_include_directories(modsecurity_benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/include ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(modsecurity_benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/lib/libmodsecurity.a)
target_link_libraries(modsecurity_benchmark PRIVATE dl curl pthread libpcre.a libyajl_s.a libxml2.a libz.a liblzma.a libicuuc.a libicudata.a)

