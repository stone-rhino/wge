# Set visibility to hidden by default for all targets
# set(CMAKE_CXX_VISIBILITY_PRESET hidden)
# set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

# Avoid GNU unique symbols to ensure that the shared library can be unloaded properly
add_compile_options(-fno-gnu-unique)

file(GLOB local_source
  *.h
  *.cc
)

add_subdirectory(action)
add_subdirectory(antlr4)
add_subdirectory(common)
add_subdirectory(macro)
add_subdirectory(operator)
add_subdirectory(variable)
add_subdirectory(persistent_storage)
add_subdirectory(transformation)

add_dependencies(common_ragel ragel)

add_library(wge
  $<TARGET_OBJECTS:action>
  $<TARGET_OBJECTS:antlr4>
  $<TARGET_OBJECTS:common>
  $<TARGET_OBJECTS:common_ragel>
  $<TARGET_OBJECTS:macro>
  $<TARGET_OBJECTS:operator>
  $<TARGET_OBJECTS:variable>
  $<TARGET_OBJECTS:persistent_storage>
  $<TARGET_OBJECTS:transformation>
  ${local_source}
)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(wge PRIVATE spdlog::spdlog_header_only)

# Shared library
add_library(wge_shared SHARED
  $<TARGET_OBJECTS:action>
  $<TARGET_OBJECTS:antlr4>
  $<TARGET_OBJECTS:common>
  $<TARGET_OBJECTS:common_ragel>
  $<TARGET_OBJECTS:macro>
  $<TARGET_OBJECTS:operator>
  $<TARGET_OBJECTS:variable>
  $<TARGET_OBJECTS:persistent_storage>
  $<TARGET_OBJECTS:transformation>
  ${local_source}
)
target_link_options(wge_shared PRIVATE "LINKER:--exclude-libs,ALL")

target_link_libraries(wge_shared PUBLIC spdlog::spdlog_header_only)

find_package(antlr4-runtime CONFIG REQUIRED)
target_link_libraries(wge_shared PRIVATE antlr4_static)

find_library(INJECTION_LIB injection)
target_link_libraries(wge_shared PRIVATE ${INJECTION_LIB})

find_package(re2 CONFIG REQUIRED)
target_link_libraries(wge_shared PRIVATE re2::re2)

find_package(pcre2 CONFIG REQUIRED)
target_link_libraries(wge_shared PRIVATE PCRE2::8BIT)

find_package(PkgConfig REQUIRED)
pkg_check_modules(HYPERSCAN REQUIRED libhs)
target_link_directories(wge_shared PRIVATE ${HYPERSCAN_LIBRARY_DIRS})
target_link_libraries(wge_shared PRIVATE ${HYPERSCAN_LIBRARIES})

# Set consistent output names (only filename differs by suffix)
set_target_properties(wge PROPERTIES OUTPUT_NAME "wge")
set_target_properties(wge_shared PROPERTIES OUTPUT_NAME "wge")