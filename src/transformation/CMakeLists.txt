file(GLOB local_source
  *.h
  *.cc
)

find_package(Boost REQUIRED COMPONENTS url)
add_library(transformation ${local_source})
target_link_libraries(transformation PRIVATE Boost::url)
