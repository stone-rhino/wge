#pragma once

#include "antlr4_gen/SecLangParserBaseVisitor.h"
#include "parser.h"

namespace SrSecurity::Antlr4 {
class Visitor : public Antlr4Gen::SecLangParserBaseVisitor {
public:
  Visitor(Parser* parser) : parser_(parser) {}

public:
  std::any visitInclude(Antlr4Gen::SecLangParser::IncludeContext* ctx) override;
  std::any visitEngine_config(Antlr4Gen::SecLangParser::Engine_configContext* ctx) override;
  std::any visitRule_directiv(Antlr4Gen::SecLangParser::Rule_directivContext* ctx) override;
  std::any visitVariable(Antlr4Gen::SecLangParser::VariableContext* ctx) override;

private:
  Parser* parser_;
};
} // namespace SrSecurity::Antlr4