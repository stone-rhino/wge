file(GLOB local_source
  *.h
  *.cc
)

add_library(action ${local_source})