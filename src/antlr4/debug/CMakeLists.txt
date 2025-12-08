
find_package(antlr4-runtime CONFIG REQUIRED)

add_executable(antlr4_debug_lexer debug_lexer.cc ${CMAKE_BINARY_DIR}/src/antlr4/antlr4_gen/SecLangLexer.cpp)
add_executable(antlr4_debug_parser debug_parser.cc ${CMAKE_BINARY_DIR}/src/antlr4/antlr4_gen/SecLangLexer.cpp ${CMAKE_BINARY_DIR}/src/antlr4/antlr4_gen/SecLangParser.cpp)

target_include_directories(antlr4_debug_lexer PRIVATE ${CMAKE_BINARY_DIR}/src/antlr4)
target_include_directories(antlr4_debug_parser PRIVATE ${CMAKE_BINARY_DIR}/src/antlr4)
target_link_libraries(antlr4_debug_lexer PRIVATE antlr4_static)
target_link_libraries(antlr4_debug_parser PRIVATE antlr4_static)