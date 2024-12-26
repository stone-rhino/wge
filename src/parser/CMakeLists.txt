file(GLOB local_source
  *.h
  *.cc
)

set(generated_source 
  antlr4_gen/SecLangParserBaseVisitor.h
  antlr4_gen/SecLangLexer.h
  antlr4_gen/SecLangParser.h
  antlr4_gen/SecLangParserVisitor.h
  antlr4_gen/SecLangParserBaseVisitor.cpp
  antlr4_gen/SecLangLexer.cpp
  antlr4_gen/SecLangParser.cpp
  antlr4_gen/SecLangParserVisitor.cpp
)

add_custom_command(
  OUTPUT ${generated_source}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/parser
  COMMAND java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.Tool -Dlanguage=Cpp -visitor -no-listener -package SrSecurity::Parser::Antlr4Gen -o ${CMAKE_BINARY_DIR}/src/parser/antlr4_gen *.g4
  DEPENDS ${CMAKE_SOURCE_DIR}/src/parser/SecLangParser.g4 ${CMAKE_SOURCE_DIR}/src/parser/SecLangLexer.g4
)

find_package(antlr4-runtime CONFIG REQUIRED)

add_library(parser ${local_source} ${generated_source})
target_include_directories(parser PRIVATE ${CMAKE_BINARY_DIR}/src/parser)
target_link_libraries(parser PRIVATE antlr4_static)
