file(GLOB local_source
  *.h
  *.cc
)

add_library(operator ${local_source})