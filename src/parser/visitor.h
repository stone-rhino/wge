#pragma once

#include "antlr4_gen/SecLangBaseVisitor.h"
#include "parser.h"

namespace SrSecurity::Parser {
class Visitor : public Antlr4Gen::SecLangBaseVisitor {
public:
  Visitor(Parser* parser) : parser_(parser) {}

public:
  std::any visitInclude(Antlr4Gen::SecLangParser::IncludeContext* ctx) override;

private:
  Parser* parser_;
};
} // namespace SrSecurity::Parser