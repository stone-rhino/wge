#include "visitor.h"

#include <format>
#include <unordered_map>

#include <assert.h>

namespace SrSecurity::Antlr4 {

std::any Visitor::visitInclude(Antlr4Gen::SecLangParser::IncludeContext* ctx) {
  std::string file_path = ctx->STRING()->getText();
  return parser_->loadFromFile(file_path);
}

std::any Visitor::visitEngine_config(Antlr4Gen::SecLangParser::Engine_configContext* ctx) {
  std::string directive;
  auto engine_config_directive = ctx->engine_config_directiv();
  if (engine_config_directive) {
    directive = engine_config_directive->getText();
  } else {
    auto sec_rule_engine = ctx->SecRuleEngine();
    if (sec_rule_engine) {
      directive = sec_rule_engine->getText();
    }
  }

  antlr4::tree::TerminalNode* option = ctx->OPTION();
  if (!option) {
    option = ctx->DERECTION_ONLY();
  }

  if (option) {
    parser_->setEngineConfig(directive, option->getText());
  }

  return "";
}

std::any Visitor::visitRule_define(Antlr4Gen::SecLangParser::Rule_defineContext* ctx) {
  // Variables
  auto variables = ctx->variables()->variable();
  std::vector<Parser::VariableAttr> variable_attrs;
  for (auto var : variables) {
    Parser::VariableAttr attr;
    attr.full_name_ = var->getText();
    attr.main_name_ = var->var_main_name()->getText();
    attr.is_not_ = var->NOT() != nullptr;
    attr.is_counter_ = var->VAR_COUNT() != nullptr;
    variable_attrs.emplace_back(std::move(attr));
  }

  // Operator name is default to rx
  auto op = ctx->operator_();
  std::string operator_name = "rx";
  if (op->operator_name()) {
    operator_name = op->operator_name()->getText();
  }

  // Actions
  auto actions = ctx->action();
  std::unordered_map<std::string, std::string> action_map;
  for (auto action : actions) {
    action_map[action->action_name()->getText()] = action->action_value()->getText();
  }

  parser_->addRule(std::move(variable_attrs), std::move(operator_name),
                   op->operator_value()->getText(), std::move(action_map));

  return "";
}

std::any Visitor::visitRule_remove_by_id(Antlr4Gen::SecLangParser::Rule_remove_by_idContext* ctx) {
  auto ids = ctx->INT();
  for (auto id : ids) {
    std::string id_str = id->getText();
    uint64_t id_num = ::atoll(id_str.c_str());
    parser_->removeRuleById(id_num);
  }

  auto id_ranges = ctx->INT_RANGE();
  for (auto range : id_ranges) {
    std::string id_range_str = range->getText();
    auto pos = id_range_str.find('-');
    if (pos != std::string::npos) {
      uint64_t first = ::atoll(id_range_str.substr(0, pos).c_str());
      uint64_t last = ::atoll(id_range_str.substr(pos + 1).c_str());
      for (auto id = first; id <= last; ++id) {
        parser_->removeRuleById(id);
      }
    }
  }

  return "";
}

std::any visitRule_remove_by_msg(Antlr4Gen::SecLangParser::Rule_remove_by_msgContext* ctx) {
  return "";
}

std::any
Visitor::visitRule_remove_by_msg(Antlr4Gen::SecLangParser::Rule_remove_by_msgContext* ctx) {
  auto& rules = parser_->rules();

  return "";
}

std::any
Visitor::visitRule_remove_by_tag(Antlr4Gen::SecLangParser::Rule_remove_by_tagContext* ctx) {
  return "";
}

} // namespace SrSecurity::Antlr4