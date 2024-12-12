#include "parser.h"

#include <fstream>

#include "antlr4_gen/SecLangLexer.h"
#include "antlr4_gen/SecLangParser.h"
#include "visitor.h"

#include "../common/likely.h"

namespace SrSecurity::Parser {

void Parser::loadFromFile(const std::string& file_path) {
  std::ifstream ifs(file_path);
  antlr4::ANTLRInputStream input(ifs);
  Antlr4Gen::SecLangLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  Antlr4Gen::SecLangParser parser(&tokens);

  parser.setBuildParseTree(true);
  parser.removeErrorListeners();
  auto tree = parser.configuration();
  Visitor vistor;
  vistor.visit(tree);

  std::unordered_set<uint64_t> remove_ids;
  std::unordered_set<std::string> remove_tags;
  fillValideRules(remove_ids, remove_tags);
}

const std::list<Rule::RuleSharedPtr>& Parser::getValidRules(size_t phase) const {
  assert(phase < phase_total_);
  if (likely(phase < phase_total_)) {
    return valid_rules_.at(phase);
  }

  static std::list<Rule::RuleSharedPtr> empty_rules;
  return empty_rules;
}

void Parser::fillValideRules(const std::unordered_set<uint64_t>& remove_ids,
                             const std::unordered_set<std::string>& remove_tags) {
  for (size_t phase = 0; phase < phase_total_; ++phase) {
    for (auto rule : all_rules_[phase]) {
      if (likely(remove_ids.find(rule->id()) == remove_ids.end() && !rule->hasTag(remove_tags))) {
        valid_rules_[phase].emplace_back(std::move(rule));
      }
    }
  }
}
} // namespace SrSecurity::Parser