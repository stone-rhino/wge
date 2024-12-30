#include "parser.h"

#include <format>
#include <fstream>

#include "antlr4_gen/SecLangLexer.h"
#include "antlr4_gen/SecLangParser.h"
#include "visitor.h"

#include "../common/likely.h"

namespace SrSecurity::Antlr4 {

class ErrorListener : public antlr4::BaseErrorListener {
public:
  void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
                   size_t charPositionInLine, const std::string& msg,
                   std::exception_ptr e) override {
    error_msg = std::format("line {}:{} {}", line, charPositionInLine, msg);
  }

public:
  std::string error_msg;
};

std::string Parser::loadFromFile(const std::string& file_path) {
  // init
  std::ifstream ifs(file_path);
  antlr4::ANTLRInputStream input(ifs);
  Antlr4Gen::SecLangLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  Antlr4Gen::SecLangParser parser(&tokens);

  // sets error listener
  ErrorListener error_listener;
  // parser.setBuildParseTree(true);
  parser.removeErrorListeners();
  parser.addErrorListener(&error_listener);

  // parse
  auto tree = parser.configuration();
  if (!error_listener.error_msg.empty()) {
    return error_listener.error_msg;
  }

  // visit
  std::string error;
  Visitor vistor(this);
  try {
    error = std::any_cast<std::string>(vistor.visit(tree));
  } catch (const std::exception&) {
  }

  return error;
}

std::string Parser::load(const std::string& directive) {
  // init
  antlr4::ANTLRInputStream input(directive);
  Antlr4Gen::SecLangLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  Antlr4Gen::SecLangParser parser(&tokens);

  // sets error listener
  ErrorListener error_listener;
  // parser.setBuildParseTree(true);
  parser.removeErrorListeners();
  parser.addErrorListener(&error_listener);

  // parse
  auto tree = parser.configuration();
  if (!error_listener.error_msg.empty()) {
    return error_listener.error_msg;
  }

  // visit
  std::string error;
  Visitor vistor(this);
  try {
    error = std::any_cast<std::string>(vistor.visit(tree));
  } catch (const std::exception&) {
  }

  return error;
}
} // namespace SrSecurity::Antlr4