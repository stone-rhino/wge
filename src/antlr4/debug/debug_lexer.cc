#include <fstream>
#include <iostream>

#include "antlr4-runtime.h"
#include "antlr4_gen/SecLangLexer.h"

class DebugLexer : public Wge::Antlr4::Antlr4Gen::SecLangLexer {
public:
  explicit DebugLexer(antlr4::CharStream* input) : SecLangLexer(input) {}

  std::unique_ptr<antlr4::Token> nextToken() override {
    // Get current mode name
    std::string current_mode_name = "DEFAULT";
    if (mode < getModeNames().size()) {
      current_mode_name = getModeNames()[mode];
    }

    auto token = SecLangLexer::nextToken();

    // Get token type name from vocabulary
    std::string token_name = "UNKNOWN";
    if (token->getType() != antlr4::Token::EOF &&
        token->getType() < getVocabulary().getMaxTokenType()) {
      auto symbolicName = getVocabulary().getSymbolicName(token->getType());
      if (!symbolicName.empty()) {
        token_name = symbolicName;
      }
    } else if (token->getType() == antlr4::Token::EOF) {
      token_name = "EOF";
    }

    // Print token information with mode and token name
    std::cout << token->toString() << " [mode: " << current_mode_name << ", token: " << token_name
              << "]" << std::endl;

    return token;
  }
};

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
  DebugLexer lexer(&input);

  std::unique_ptr<Token> token;
  do {
    token = lexer.nextToken();
  } while (token->getType() != Token::EOF);

  return 0;
}
