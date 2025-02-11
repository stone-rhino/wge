file(GLOB local_source
  *.h
  *.cc
)

add_library(persistent_storage ${local_source})