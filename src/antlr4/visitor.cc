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
  static const std::unordered_map<
      std::string, std::function<Parser::EngineConfig::Option&(SrSecurity::Antlr4::Parser*)>>
      enginge_config_map{
          {"SecRequestBodyAccess",
           [](SrSecurity::Antlr4::Parser* parser) -> Parser::EngineConfig::Option& {
             return parser->engineConfig().is_request_body_access_;
           }},
          {"SecResponseBodyAccess",
           [](SrSecurity::Antlr4::Parser* parser) -> Parser::EngineConfig::Option& {
             return parser->engineConfig().is_response_body_access_;
           }},
          {"SecRuleEngine",
           [](SrSecurity::Antlr4::Parser* parser) -> Parser::EngineConfig::Option& {
             return parser->engineConfig().is_rule_engine_;
           }},
          {"SecTmpSaveUploadedFiles",
           [](SrSecurity::Antlr4::Parser* parser) -> Parser::EngineConfig::Option& {
             return parser->engineConfig().is_tmp_save_uploaded_files_;
           }},
          {"SecUploadKeepFiles",
           [](SrSecurity::Antlr4::Parser* parser) -> Parser::EngineConfig::Option& {
             return parser->engineConfig().is_upload_keep_files_;
           }},
          {"SecXmlExternalEntity",
           [](SrSecurity::Antlr4::Parser* parser) -> Parser::EngineConfig::Option& {
             return parser->engineConfig().is_xml_external_entity_;
           }},
      };

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

  auto iter = enginge_config_map.find(directive);
  assert(iter != enginge_config_map.end());
  if (iter != enginge_config_map.end()) {
    auto option = ctx->OPTION();
    if (option) {
      if (option->getText() == "On") {
        iter->second(parser_) = Parser::EngineConfig::Option::On;
      } else {
        iter->second(parser_) = Parser::EngineConfig::Option::Off;
      }
    } else {
      iter->second(parser_) = Parser::EngineConfig::Option::DetectionOnly;
    }
  }
  return "";
}

std::any Visitor::visitRule_define(Antlr4Gen::SecLangParser::Rule_defineContext* ctx) {
  parser_->rules().emplace_back(std::make_unique<Rule>());
  return visitChildren(ctx);
}

std::any Visitor::visitVariable(Antlr4Gen::SecLangParser::VariableContext* ctx) {
  // Construct a varialbe and append it to the rule
  auto& factory = Parser::getVariableFactory();
  std::string main_name = ctx->var_main_name()->getText();
  auto iter = factory.find(main_name);
  if (iter != factory.end()) {
    std::string full_name = ctx->getText();
    bool is_not = ctx->NOT() != nullptr;
    bool is_counter = ctx->VAR_COUNT() != nullptr;
    auto var = iter->second(std::move(full_name), is_not, is_counter);
    if (var) {
      parser_->rules().back()->appendVariable(std::move(var));
    } else {
      should_visit_next_child_ = false;
      return std::format("The variable is not support now: {} line: {} offset: {}", full_name,
                         ctx->getStart()->getLine(), ctx->getStart()->getStartIndex());
    }
  }

  return "";
}

std::any Visitor::visitOperator(Antlr4Gen::SecLangParser::OperatorContext* ctx) {
  // Construct a operator and sets it into the rule
  auto& factory = Parser::getOperatorFactory();
  std::string operator_name;
  if (ctx->operator_name()) {
    operator_name = ctx->operator_name()->getText();
  } else {
    operator_name = "rx";
  }
  auto iter = factory.find(operator_name);
  if (iter != factory.end()) {
    std::string operator_value = ctx->operator_value()->getText();
    auto op = iter->second(std::move(operator_name), std::move(operator_value));
    if (op) {
      parser_->rules().back()->setOperator(std::move(op));
    } else {
      should_visit_next_child_ = false;
      return std::format("The operator is not support now: {} line: {} offset: {}", operator_name,
                         ctx->getStart()->getLine(), ctx->getStart()->getStartIndex());
    }
  }

  return "";
}

std::any Visitor::visitAction(Antlr4Gen::SecLangParser::ActionContext* ctx) {
  // Construct a action and append it to the rule
  auto& factory = Parser::getActionFactory();
  std::string action_name = ctx->action_name()->getText();
  auto iter = factory.find(action_name);
  if (iter != factory.end()) {
    std::string action_value;
    if (ctx->action_value()) {
      action_value = ctx->action_value()->getText();
    }
    iter->second(*parser_->rules().back(), std::move(action_value));
  }

  return "";
}

std::any Visitor::visitRule_remove_by_id(Antlr4Gen::SecLangParser::Rule_remove_by_idContext* ctx) {
  auto& rules = parser_->rules();

  auto ids = ctx->INT();
  for (auto id : ids) {
    std::string id_str = id->getText();
    uint64_t id_num = ::atoll(id_str.c_str());
    std::erase_if(rules, [&](const std::unique_ptr<SrSecurity::Rule>& rule) {
      if (rule->id() == id_num) {
        return true;
      }
      return false;
    });
  }

  auto id_ranges = ctx->INT_RANGE();
  for (auto range : id_ranges) {
    std::string id_range_str = range->getText();
    auto pos = id_range_str.find('-');
    if (pos != std::string::npos) {
      uint64_t first = ::atoll(id_range_str.substr(0, pos).c_str());
      uint64_t last = ::atoll(id_range_str.substr(pos + 1).c_str());
      std::erase_if(rules, [&](const std::unique_ptr<SrSecurity::Rule>& rule) {
        const uint64_t id = rule->id();
        if (id >= first && id <= last) {
          return true;
        }
        return false;
      });
    }
  }

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