#include "visitor.h"

#include <format>
#include <unordered_map>

#include <assert.h>

namespace SrSecurity::Antlr4 {

std::any Visitor::visitInclude(Antlr4Gen::SecLangParser::IncludeContext* ctx) {
  std::string file_path = ctx->STRING()->getText();
  return parser_->loadFromFile(file_path);
}

std::any Visitor::visitSec_reqeust_body_access(
    Antlr4Gen::SecLangParser::Sec_reqeust_body_accessContext* ctx) {
  parser_->secRequestBodyAccess(optionStr2EnumValue(ctx->OPTION()->getText()));
  return "";
}

std::any Visitor::visitSec_response_body_access(
    Antlr4Gen::SecLangParser::Sec_response_body_accessContext* ctx) {
  parser_->secResponseBodyAccess(optionStr2EnumValue(ctx->OPTION()->getText()));
  return "";
}

std::any Visitor::visitSec_rule_engine(Antlr4Gen::SecLangParser::Sec_rule_engineContext* ctx) {
  parser_->secRuleEngine(optionStr2EnumValue(ctx->OPTION()->getText()));
  return "";
}

std::any Visitor::visitSec_tmp_save_uploaded_files(
    Antlr4Gen::SecLangParser::Sec_tmp_save_uploaded_filesContext* ctx) {
  parser_->secTmpSaveUploadedFiles(optionStr2EnumValue(ctx->OPTION()->getText()));
  return "";
}

std::any
Visitor::visitSec_upload_keep_files(Antlr4Gen::SecLangParser::Sec_upload_keep_filesContext* ctx) {
  parser_->secUploadKeepFiles(optionStr2EnumValue(ctx->OPTION()->getText()));
  return "";
}

std::any Visitor::visitSec_xml_external_entity(
    Antlr4Gen::SecLangParser::Sec_xml_external_entityContext* ctx) {
  parser_->secXmlExternalEntity(optionStr2EnumValue(ctx->OPTION()->getText()));
  return "";
}

std::any Visitor::visitSec_rule(Antlr4Gen::SecLangParser::Sec_ruleContext* ctx) {
  // Variables
  std::vector<Parser::VariableAttr> variable_attrs = getVariableAttr(ctx);

  // Operator name is default to rx
  auto op = ctx->operator_();
  std::string operator_name = "rx";
  if (op->OPERATOR_NAME()) {
    operator_name = op->OPERATOR_NAME()->getText();
  }

  // Actions
  auto actions = ctx->action();
  std::unordered_multimap<std::string, std::string> action_map;
  for (auto action : actions) {
    action_map.insert({action->ACTION_NAME()->getText(), action->action_value()->getText()});
  }

  parser_->secRule(std::move(variable_attrs), std::move(operator_name),
                   op->operator_value()->getText(), std::move(action_map));

  return "";
}

std::any
Visitor::visitSec_rule_remove_by_id(Antlr4Gen::SecLangParser::Sec_rule_remove_by_idContext* ctx) {
  auto ids = ctx->INT();
  for (auto id : ids) {
    std::string id_str = id->getText();
    uint64_t id_num = ::atoll(id_str.c_str());
    parser_->secRuleRemoveById(id_num);
  }

  auto id_ranges = ctx->INT_RANGE();
  for (auto range : id_ranges) {
    std::string id_range_str = range->getText();
    auto pos = id_range_str.find('-');
    if (pos != std::string::npos) {
      uint64_t first = ::atoll(id_range_str.substr(0, pos).c_str());
      uint64_t last = ::atoll(id_range_str.substr(pos + 1).c_str());
      for (auto id = first; id <= last; ++id) {
        parser_->secRuleRemoveById(id);
      }
    }
  }

  return "";
}

std::any
Visitor::visitSec_rule_remove_by_msg(Antlr4Gen::SecLangParser::Sec_rule_remove_by_msgContext* ctx) {
  parser_->secRuleRemoveByMsg(ctx->STRING()->getText());
  return "";
}

std::any
Visitor::visitSec_rule_remove_by_tag(Antlr4Gen::SecLangParser::Sec_rule_remove_by_tagContext* ctx) {
  parser_->secRuleRemoveByTag(ctx->STRING()->getText());
  return "";
}

std::any Visitor::visitSec_rule_update_action_by_id(
    Antlr4Gen::SecLangParser::Sec_rule_update_action_by_idContext* ctx) {
  uint64_t id = ::atoll(ctx->INT()->getText().c_str());

  // Actions
  auto actions = ctx->action();
  std::unordered_multimap<std::string, std::string> action_map;
  for (auto action : actions) {
    action_map.insert({action->ACTION_NAME()->getText(), action->action_value()->getText()});
  }

  parser_->secRuleUpdateActionById(id, std::move(action_map));
  return "";
}

std::any Visitor::visitSec_rule_update_target_by_id(
    Antlr4Gen::SecLangParser::Sec_rule_update_target_by_idContext* ctx) {
  uint64_t id = ::atoll(ctx->INT()->getText().c_str());
  std::vector<Parser::VariableAttr> variable_attrs = getVariableAttr(ctx);
  parser_->secRuleUpdateTargetById(id, std::move(variable_attrs));
  return "";
}

std::any Visitor::visitSec_rule_update_target_by_msg(
    Antlr4Gen::SecLangParser::Sec_rule_update_target_by_msgContext* ctx) {
  std::string msg = ctx->STRING()->getText();
  std::vector<Parser::VariableAttr> variable_attrs = getVariableAttr(ctx);
  parser_->secRuleUpdateTargetByMsg(msg, std::move(variable_attrs));
  return "";
}

std::any Visitor::visitSec_rule_update_target_by_tag(
    Antlr4Gen::SecLangParser::Sec_rule_update_target_by_tagContext* ctx) {
  std::string tag = ctx->STRING()->getText();
  std::vector<Parser::VariableAttr> variable_attrs = getVariableAttr(ctx);
  parser_->secRuleUpdateTargetByTag(tag, std::move(variable_attrs));
  return "";
}

Parser::EngineConfig::Option Visitor::optionStr2EnumValue(const std::string& option_str) {
  Parser::EngineConfig::Option option = Parser::EngineConfig::Option::Off;
  if (option_str == "On") {
    option = Parser::EngineConfig::Option::On;
  } else if (option_str == "DetectionOnly") {
    option = Parser::EngineConfig::Option::DetectionOnly;
  }
  return option;
}

} // namespace SrSecurity::Antlr4