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
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/antlr4
  COMMAND java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.Tool -Dlanguage=Cpp -visitor -no-listener -package SrSecurity::Antlr4::Antlr4Gen -o ${CMAKE_BINARY_DIR}/src/antlr4/antlr4_gen *.g4
  DEPENDS ${CMAKE_SOURCE_DIR}/src/antlr4/SecLangParser.g4 ${CMAKE_SOURCE_DIR}/src/antlr4/SecLangLexer.g4
)

find_package(antlr4-runtime CONFIG REQUIRED)

add_library(antlr4 ${local_source} ${generated_source})
target_include_directories(antlr4 PRIVATE ${CMAKE_BINARY_DIR}/src/antlr4)
target_link_libraries(antlr4 PRIVATE antlr4_static)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(antlr4 PRIVATE spdlog::spdlog_header_only)
