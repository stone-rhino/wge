#include <fstream>
#include <iostream>

#include "antlr4-runtime.h"
#include "antlr4_gen/SecLangLexer.h"
#include "antlr4_gen/SecLangParser.h"

int main(int argc, char* argv[]) {
  using namespace antlr4;
  using namespace Wge::Antlr4::Antlr4Gen;

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return 1;
  }

  std::ifstream stream;
  stream.open(argv[1]);
  if (!stream.is_open()) {
    std::cerr << "Error: Could not open file " << argv[1] << std::endl;
    return 1;
  }

  ANTLRInputStream input(stream);
  SecLangLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SecLangParser parser(&tokens);

  // Parse the configuration
  auto tree = parser.configuration();

  // Print the parse tree
  std::cout << tree->toStringTree(&parser) << std::endl;

  return 0;
}
