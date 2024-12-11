file(GLOB local_source
  *.h
  *.cc
)

add_subdirectory(action)
add_subdirectory(common)
add_subdirectory(rule)
add_subdirectory(variable)
add_library(srsecurity ${local_source})