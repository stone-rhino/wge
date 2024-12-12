#pragma once

#include "antlr4_gen/SecLangParserBaseVisitor.h"

namespace SrSecurity::Parser {
class Visitor : public Antlr4Gen::SecLangParserBaseVisitor {
public:
  std::any visitConfiguration(Antlr4Gen::SecLangParser::ConfigurationContext* ctx) override {
    return visitChildren(ctx);
  }
};
} // namespace SrSecurity::Parser