file(GLOB local_source
  *.h
  *.cc
)

# update submodules
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/bindings/python/modsecurity
  ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/others/libinjection
  ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/others/mbedtls
  COMMAND git submodule update --init
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
)

# generate configure
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/configure
  COMMAND ./build.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/bindings/python/modsecurity
  ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/others/libinjection
  ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/others/mbedtls
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(INSTALL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/debug)
  # Clean release-with-debug-info and release
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/build-debug
    COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/release-with-debug-info && rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/release && rm -rf release-with-debug-info && rm -rf build-release && touch build-debug && make distclean || true
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/configure
  )

  # Configure
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/config.log
    COMMAND bash -c "CFLAGS=\"-g -O0\" CXXFLAGS=\"-g -O0 -std=c++17\" ./configure --disable-debug-logs --prefix=${INSTALL_PATH}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/build-debug
  )
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
  set(INSTALL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/release-with-debug-info)
    # Clean debug and release
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/build-release-with-debug-info
      COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/debug && rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/release && rm -rf build-debug && rm -rf build-release && touch build-release-with-debug-info && make distclean || true
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/configure
    )
  
    # Configure
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/config.log
      COMMAND bash -c "CFLAGS=\"-g -O2 -fno-omit-frame-pointer\" CXXFLAGS=\"-g -O2 -std=c++17 -fno-omit-frame-pointer\" ./configure --disable-debug-logs --prefix=${INSTALL_PATH}"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/build-release-with-debug-info
    )
else()
  set(INSTALL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/release)
  # Clean debug and release-with-debug-info
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/build-release
    COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/debug  && rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/install/release-with-debug-info && rm -rf build-debug && rm -rf  build-release-with-debug-info && touch build-release && make distclean || true
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/configure
  )

  # Configure
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/config.log
    COMMAND bash -c "CFLAGS=\"-g -O2\" CXXFLAGS=\"-g -O2 -std=c++17\" ./configure --disable-debug-logs --prefix=${INSTALL_PATH}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/build-release
  )
endif()

# make && make install
add_custom_command(
  OUTPUT ${INSTALL_PATH}/lib/libmodsecurity.a
  COMMAND make -j8 && make install
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity/config.log
)

add_custom_target(build
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modsecurity
  DEPENDS ${INSTALL_PATH}/lib/libmodsecurity.a
)

# without tcmalloc
add_executable(modsecurity_benchmark ${local_source})
add_dependencies(modsecurity_benchmark build)
target_include_directories(modsecurity_benchmark PRIVATE ${INSTALL_PATH}/include ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(modsecurity_benchmark PRIVATE ${INSTALL_PATH}/lib/libmodsecurity.a)
target_link_libraries(modsecurity_benchmark PRIVATE dl curl pthread libpcre.a libyajl_s.a libxml2.a libz.a liblzma.a libicuuc.a libicudata.a)

# with tcmalloc
find_package(PkgConfig REQUIRED)
pkg_check_modules(TCMALLOC REQUIRED libtcmalloc)
add_executable(modsecurity_benchmark_tcmalloc ${local_source})
add_dependencies(modsecurity_benchmark_tcmalloc build)
target_include_directories(modsecurity_benchmark_tcmalloc PRIVATE ${INSTALL_PATH}/include ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(modsecurity_benchmark_tcmalloc PRIVATE ${INSTALL_PATH}/lib/libmodsecurity.a)
target_link_libraries(modsecurity_benchmark_tcmalloc PRIVATE dl curl pthread libpcre.a libyajl_s.a libxml2.a libz.a liblzma.a libicuuc.a libicudata.a)
target_link_directories(modsecurity_benchmark_tcmalloc PRIVATE ${TCMALLOC_LIBRARY_DIRS})
target_link_libraries(modsecurity_benchmark_tcmalloc PRIVATE ${TCMALLOC_LIBRARIES})

