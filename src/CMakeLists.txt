file(GLOB local_source
  *.h
  *.cc
)

add_subdirectory(action)
add_subdirectory(common)
add_subdirectory(parser)
add_subdirectory(rule)
add_subdirectory(variable)
add_library(srsecurity ${local_source})
add_dependencies(srsecurity antlr4_gen)