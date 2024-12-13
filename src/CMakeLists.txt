file(GLOB local_source
  *.h
  *.cc
)

file(GLOB antlr4_obj CONFIGURE_DEPENDS
 ${CMAKE_BINARY_DIR}/antlr4obj/*.o
)

add_subdirectory(action)
add_subdirectory(common)
add_subdirectory(parser)
add_subdirectory(rule)
add_subdirectory(variable)
add_library(srsecurity 
  $<TARGET_OBJECTS:parser>
  ${antlr4_obj}
  ${local_source}
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  SET(ANTLR4_OBJ ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/debug/lib/libantlr4-runtime.a)
else()
  SET(ANTLR4_OBJ ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/lib/libantlr4-runtime.a)
endif()

add_custom_target(antlr4obj
  COMMAND rm -rf antlr4obj && mkdir antlr4obj && cd antlr4obj && ar -x ${ANTLR4_OBJ}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  DEPENDS parser
)

add_dependencies(srsecurity antlr4obj)

# add_custom_target(genexdebug COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_OBJECTS:parser>")
# cmake --build . --target genexdebug
