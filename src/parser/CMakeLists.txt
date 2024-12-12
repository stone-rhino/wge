set(generated_source 
  SecLang*.h
  SecLang*.cpp
)

add_custom_command(
  OUTPUT ${generated_source}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/parser/seclang_parser/parser
  COMMAND java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.Tool -Dlanguage=Cpp -package SrSecurity::Parser::Antlr4Gen -o ${CMAKE_SOURCE_DIR}/src/parser/antlr4_gen *.g4
)

add_custom_target(antlr4_gen
  DEPENDS ${generated_source}
)