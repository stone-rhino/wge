file(GLOB local_source
  *.h
  *.cc
)

add_library(transformation ${local_source})