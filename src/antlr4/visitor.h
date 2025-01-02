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

  std::any visitRule_define(Antlr4Gen::SecLangParser::Rule_defineContext* ctx) override;
  std::any visitVariable(Antlr4Gen::SecLangParser::VariableContext* ctx) override;
  std::any visitOperator(Antlr4Gen::SecLangParser::OperatorContext* ctx) override;
  std::any visitAction(Antlr4Gen::SecLangParser::ActionContext* ctx) override;
  std::any visitRule_remove_by_id(Antlr4Gen::SecLangParser::Rule_remove_by_idContext* ctx) override;
  std::any
  visitRule_remove_by_msg(Antlr4Gen::SecLangParser::Rule_remove_by_msgContext* ctx) override;
  std::any
  visitRule_remove_by_tag(Antlr4Gen::SecLangParser::Rule_remove_by_tagContext* ctx) override;

public:
  bool shouldVisitNextChild(antlr4::tree::ParseTree* /*node*/,
                            const std::any& /*currentResult*/) override {
    return should_visit_next_child_;
  }

private:
  bool should_visit_next_child_{true};
  Parser* parser_;
};
} // namespace SrSecurity::Antlr4