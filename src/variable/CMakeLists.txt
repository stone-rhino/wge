file(GLOB local_source
  *.h
  *.cc
)

add_library(variable ${local_source})