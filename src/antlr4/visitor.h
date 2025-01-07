#pragma once

#include "antlr4_gen/SecLangParserBaseVisitor.h"
#include "parser.h"

namespace SrSecurity::Antlr4 {
class Visitor : public Antlr4Gen::SecLangParserBaseVisitor {
public:
  Visitor(Parser* parser) : parser_(parser) {}

public:
  std::any visitInclude(Antlr4Gen::SecLangParser::IncludeContext* ctx) override;

  std::any visitSec_reqeust_body_access(
      Antlr4Gen::SecLangParser::Sec_reqeust_body_accessContext* ctx) override;

  std::any visitSec_response_body_access(
      Antlr4Gen::SecLangParser::Sec_response_body_accessContext* ctx) override;

  std::any visitSec_rule_engine(Antlr4Gen::SecLangParser::Sec_rule_engineContext* ctx) override;

  std::any visitSec_tmp_save_uploaded_files(
      Antlr4Gen::SecLangParser::Sec_tmp_save_uploaded_filesContext* ctx) override;

  std::any
  visitSec_upload_keep_files(Antlr4Gen::SecLangParser::Sec_upload_keep_filesContext* ctx) override;

  std::any visitSec_xml_external_entity(
      Antlr4Gen::SecLangParser::Sec_xml_external_entityContext* ctx) override;

  std::any visitSec_rule(Antlr4Gen::SecLangParser::Sec_ruleContext* ctx) override;

  std::any
  visitSec_rule_remove_by_id(Antlr4Gen::SecLangParser::Sec_rule_remove_by_idContext* ctx) override;

  std::any visitSec_rule_remove_by_msg(
      Antlr4Gen::SecLangParser::Sec_rule_remove_by_msgContext* ctx) override;

  std::any visitSec_rule_remove_by_tag(
      Antlr4Gen::SecLangParser::Sec_rule_remove_by_tagContext* ctx) override;

  std::any visitSec_rule_update_action_by_id(
      Antlr4Gen::SecLangParser::Sec_rule_update_action_by_idContext* ctx) override;

  std::any visitSec_rule_update_target_by_id(
      Antlr4Gen::SecLangParser::Sec_rule_update_target_by_idContext* ctx) override;

  std::any visitSec_rule_update_target_by_msg(
      Antlr4Gen::SecLangParser::Sec_rule_update_target_by_msgContext* ctx) override;

  std::any visitSec_rule_update_target_by_tag(
      Antlr4Gen::SecLangParser::Sec_rule_update_target_by_tagContext* ctx) override;

private:
  static Parser::EngineConfig::Option optionStr2EnumValue(const std::string& option_str);
  template <class T> std::vector<Parser::VariableAttr> getVariableAttr(T* ctx) {
    auto variables = ctx->variables()->variable();
    std::vector<Parser::VariableAttr> variable_attrs;
    for (auto var : variables) {
      Parser::VariableAttr attr;
      attr.full_name_ = var->VAR_MAIN_NAME()->getText();
      if (var->STRING()) {
        attr.full_name_ += ":" + var->STRING()->getText();
      }
      attr.main_name_ = var->VAR_MAIN_NAME()->getText();
      attr.is_not_ = var->NOT() != nullptr;
      attr.is_counter_ = var->VAR_COUNT() != nullptr;
      variable_attrs.emplace_back(std::move(attr));
    }
    return variable_attrs;
  }

private:
  Parser* parser_;
};
} // namespace SrSecurity::Antlr4