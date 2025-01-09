file(GLOB local_source
  *.h
  *.cc
)

add_library(macro ${local_source})