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
  action
  antlr4
  common
  macro
  operator
  variable
  persistent_storage
  transformation
  ${local_source}
)
target_link_libraries(wge_shared PUBLIC spdlog::spdlog_header_only)

# Set consistent output names (only filename differs by suffix)
set_target_properties(wge PROPERTIES OUTPUT_NAME "wge")
set_target_properties(wge_shared PROPERTIES OUTPUT_NAME "wge")