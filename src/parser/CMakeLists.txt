file(GLOB local_source
  *.h
  *.cc
  antlr4_gen/*.h
  antlr4_gen/*.cpp
)

set(generated_source 
  SecLang*.h
  SecLang*.cpp
)

add_custom_command(
  OUTPUT ${generated_source}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/parser/seclang_parser/parser
  COMMAND java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.Tool -Dlanguage=Cpp -visitor -no-listener -package SrSecurity::Parser::Antlr4Gen -o ${CMAKE_SOURCE_DIR}/src/parser/antlr4_gen *.g4
)

add_custom_target(antlr4_gen
  DEPENDS ${generated_source}
)

find_package(antlr4-runtime CONFIG REQUIRED)

add_library(parser ${local_source})
add_dependencies(parser antlr4_gen)
target_link_libraries(parser PRIVATE antlr4_static)
