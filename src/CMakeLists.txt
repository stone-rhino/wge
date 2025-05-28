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