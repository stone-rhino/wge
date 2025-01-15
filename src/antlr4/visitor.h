#pragma once

#include "antlr4_gen/SecLangParserBaseVisitor.h"
#include "parser.h"

namespace SrSecurity::Antlr4 {
class Visitor : public Antlr4Gen::SecLangParserBaseVisitor {
public:
  Visitor(Parser* parser) : parser_(parser) {}

public:
  std::any visitInclude(Antlr4Gen::SecLangParser::IncludeContext* ctx) override;

  // Engine configurations
public:
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

  // Rule directives
public:
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

  // Action Group: Meta-data
  std::any
  visitAction_meta_data_id(Antlr4Gen::SecLangParser::Action_meta_data_idContext* ctx) override;
  std::any visitAction_meta_data_phase(
      Antlr4Gen::SecLangParser::Action_meta_data_phaseContext* ctx) override;
  std::any visitAction_meta_data_severity(
      Antlr4Gen::SecLangParser::Action_meta_data_severityContext* ctx) override;
  std::any
  visitAction_meta_data_msg(Antlr4Gen::SecLangParser::Action_meta_data_msgContext* ctx) override;
  std::any
  visitAction_meta_data_tag(Antlr4Gen::SecLangParser::Action_meta_data_tagContext* ctx) override;
  std::any
  visitAction_meta_data_ver(Antlr4Gen::SecLangParser::Action_meta_data_verContext* ctx) override;
  std::any
  visitAction_meta_data_rev(Antlr4Gen::SecLangParser::Action_meta_data_revContext* ctx) override;
  std::any visitAction_meta_data_accuracy(
      Antlr4Gen::SecLangParser::Action_meta_data_accuracyContext* ctx) override;
  std::any visitAction_meta_data_maturity(
      Antlr4Gen::SecLangParser::Action_meta_data_maturityContext* ctx) override;

  // Action Group: Non-disruptive
  // setvar
  std::any visitAction_non_disruptive_setvar_create(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_createContext* ctx) override;
  std::any visitAction_non_disruptive_setvar_create_init(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_create_initContext* ctx) override;
  std::any visitAction_non_disruptive_setvar_remove(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_removeContext* ctx) override;
  std::any visitAction_non_disruptive_setvar_increase(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_increaseContext* ctx) override;
  std::any visitAction_non_disruptive_setvar_decrease(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_decreaseContext* ctx) override;

  // setvar macro expansion
  std::any visitAction_non_disruptive_setvar_macro_tx(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_txContext* ctx) override;
  std::any visitAction_non_disruptive_setvar_macro_remote_addr(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_remote_addrContext* ctx)
      override;
  std::any visitAction_non_disruptive_setvar_macro_user_id(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_user_idContext* ctx) override;
  std::any visitAction_non_disruptive_setvar_macro_highest_severity(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_highest_severityContext* ctx)
      override;
  std::any visitAction_non_disruptive_setvar_macro_matched_var(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_matched_varContext* ctx)
      override;
  std::any visitAction_non_disruptive_setvar_macro_matched_var_name(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_matched_var_nameContext* ctx)
      override;
  std::any visitAction_non_disruptive_setvar_macro_multipart_strict_error(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_multipart_strict_errorContext*
          ctx) override;
  std::any visitAction_non_disruptive_setvar_macro_rule(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_ruleContext* ctx) override;
  std::any visitAction_non_disruptive_setvar_macro_session(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_sessionContext* ctx) override;

  // setenv
  std::any visitAction_non_disruptive_setenv(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setenvContext* ctx) override;

  // Audit log configurations
public:
  std::any visitSec_audit_engine(Antlr4Gen::SecLangParser::Sec_audit_engineContext* ctx) override;
  std::any visitSec_audit_log(Antlr4Gen::SecLangParser::Sec_audit_logContext* ctx) override;
  std::any visitSec_audit_log2(Antlr4Gen::SecLangParser::Sec_audit_log2Context* ctx) override;
  std::any visitSec_audit_log_dir_mode(
      Antlr4Gen::SecLangParser::Sec_audit_log_dir_modeContext* ctx) override;
  std::any
  visitSec_audit_log_format(Antlr4Gen::SecLangParser::Sec_audit_log_formatContext* ctx) override;
  std::any visitSec_audit_log_file_mode(
      Antlr4Gen::SecLangParser::Sec_audit_log_file_modeContext* ctx) override;
  std::any
  visitSec_audit_log_parts(Antlr4Gen::SecLangParser::Sec_audit_log_partsContext* ctx) override;
  std::any visitSec_audit_log_relevant_status(
      Antlr4Gen::SecLangParser::Sec_audit_log_relevant_statusContext* ctx) override;
  std::any visitSec_audit_log_storage_dir(
      Antlr4Gen::SecLangParser::Sec_audit_log_storage_dirContext* ctx) override;
  std::any
  visitSec_audit_log_type(Antlr4Gen::SecLangParser::Sec_audit_log_typeContext* ctx) override;
  std::any visitSec_component_signature(
      Antlr4Gen::SecLangParser::Sec_component_signatureContext* ctx) override;

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
  std::list<std::unique_ptr<Rule>>::iterator current_rule_iter_;
  std::unordered_multimap<std::string, std::string> action_map_;
};
} // namespace SrSecurity::Antlr4