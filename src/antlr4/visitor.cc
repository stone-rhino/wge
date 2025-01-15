#include "visitor.h"

#include <format>
#include <unordered_map>

#include <assert.h>

#include "../action/set_env.h"
#include "../action/set_var.h"
#include "../common/try.h"
#include "../macro/tx.h"

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

  // Add rule whitout actions first, then sets actions of the rule by visit actions
  action_map_.clear();
  current_rule_iter_ = parser_->secRule(std::move(variable_attrs), std::move(operator_name),
                                        op->operator_value()->getText(), std::move(action_map_));

  // Visit actions
  std::string error;
  TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
  if (!error.empty()) {
    parser_->removeBackRule();
    return error;
  }

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
  current_rule_iter_ = parser_->findRuleById(id);
  if (current_rule_iter_ != parser_->rules().end()) {
    // Clear all old tags first if the new actions has tag
    auto actions = ctx->action();
    for (auto action : actions) {
      if (action->action_meta_data() && action->action_meta_data()->action_meta_data_tag()) {
        (*current_rule_iter_)->tags().clear();
        parser_->clearRuleTagIndex(current_rule_iter_);
        break;
      }
    }

    // Visit actions
    std::string error;
    TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
    if (!error.empty()) {
      return error;
    }
  }

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

std::any
Visitor::visitAction_meta_data_id(Antlr4Gen::SecLangParser::Action_meta_data_idContext* ctx) {
  uint64_t id = ::atoll(ctx->INT()->getText().c_str());
  (*current_rule_iter_)->id(id);
  parser_->setRuleIdIndex(current_rule_iter_);
  return "";
};

std::any
Visitor::visitAction_meta_data_phase(Antlr4Gen::SecLangParser::Action_meta_data_phaseContext* ctx) {
  (*current_rule_iter_)->phase(::atoll(ctx->INT()->getText().c_str()));
  return "";
};

std::any Visitor::visitAction_meta_data_severity(
    Antlr4Gen::SecLangParser::Action_meta_data_severityContext* ctx) {
  std::string value = ctx->SeverityEnum()->getText();
  (*current_rule_iter_)->severity(parser_->transferServerity(value));
  return "";
};

std::any
Visitor::visitAction_meta_data_msg(Antlr4Gen::SecLangParser::Action_meta_data_msgContext* ctx) {
  (*current_rule_iter_)->msg(ctx->STRING()->getText());
  parser_->setRuleMsgIndex(current_rule_iter_);
  return "";
};

std::any
Visitor::visitAction_meta_data_tag(Antlr4Gen::SecLangParser::Action_meta_data_tagContext* ctx) {
  auto& tags = (*current_rule_iter_)->tags();
  auto result = tags.emplace(ctx->STRING()->getText());
  if (result.second) {
    parser_->setRuleTagIndex(current_rule_iter_, *result.first);
  }

  return "";
};

std::any
Visitor::visitAction_meta_data_ver(Antlr4Gen::SecLangParser::Action_meta_data_verContext* ctx) {
  (*current_rule_iter_)->ver(ctx->STRING()->getText());
  return "";
};

std::any
Visitor::visitAction_meta_data_rev(Antlr4Gen::SecLangParser::Action_meta_data_revContext* ctx) {
  (*current_rule_iter_)->rev(ctx->STRING()->getText());
  return "";
};

std::any Visitor::visitAction_meta_data_accuracy(
    Antlr4Gen::SecLangParser::Action_meta_data_accuracyContext* ctx) {
  (*current_rule_iter_)->accuracy(::atoll(ctx->LEVEL()->getText().c_str()));
  return "";
};

std::any Visitor::visitAction_meta_data_maturity(
    Antlr4Gen::SecLangParser::Action_meta_data_maturityContext* ctx) {
  (*current_rule_iter_)->maturity(::atoll(ctx->LEVEL()->getText().c_str()));
  return "";
};

std::any Visitor::visitAction_non_disruptive_setvar_create(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_createContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(std::make_unique<Action::SetVar>(ctx->VAR_NAME()->getText(), "",
                                                        Action::SetVar::EvaluateType::Create));
  return "";
};

std::any Visitor::visitAction_non_disruptive_setvar_create_init(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_create_initContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();

  if (ctx->action_non_disruptive_setvar_macro()) {
    try {
      std::shared_ptr<Macro::MacroBase> macro = std::any_cast<std::shared_ptr<Macro::MacroBase>>(
          visitChildren(ctx->action_non_disruptive_setvar_macro()));
      actions.emplace_back(std::make_unique<Action::SetVar>(
          ctx->VAR_NAME()->getText(), macro, Action::SetVar::EvaluateType::CreateAndInit));
    } catch (const std::bad_any_cast& ex) {
      return ex.what();
    }
  } else {
    actions.emplace_back(
        std::make_unique<Action::SetVar>(ctx->VAR_NAME()->getText(), ctx->VAR_VALUE()->getText(),
                                         Action::SetVar::EvaluateType::CreateAndInit));
  }

  return "";
};

std::any Visitor::visitAction_non_disruptive_setvar_remove(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_removeContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(std::make_unique<Action::SetVar>(ctx->VAR_NAME()->getText(), "",
                                                        Action::SetVar::EvaluateType::Remove));
  return "";
};

std::any Visitor::visitAction_non_disruptive_setvar_increase(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_increaseContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();

  if (ctx->action_non_disruptive_setvar_macro()) {
    try {
      std::shared_ptr<Macro::MacroBase> macro = std::any_cast<std::shared_ptr<Macro::MacroBase>>(
          visitChildren(ctx->action_non_disruptive_setvar_macro()));
      actions.emplace_back(std::make_unique<Action::SetVar>(
          ctx->VAR_NAME()->getText(), macro, Action::SetVar::EvaluateType::Increase));
    } catch (const std::bad_any_cast& ex) {
      return ex.what();
    }
  } else {
    actions.emplace_back(std::make_unique<Action::SetVar>(ctx->VAR_NAME()->getText(),
                                                          ctx->VAR_VALUE()->getText(),
                                                          Action::SetVar::EvaluateType::Increase));
  }
  return "";
};

std::any Visitor::visitAction_non_disruptive_setvar_decrease(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_decreaseContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();

  if (ctx->action_non_disruptive_setvar_macro()) {
    try {
      std::shared_ptr<Macro::MacroBase> macro = std::any_cast<std::shared_ptr<Macro::MacroBase>>(
          visitChildren(ctx->action_non_disruptive_setvar_macro()));
      actions.emplace_back(std::make_unique<Action::SetVar>(
          ctx->VAR_NAME()->getText(), macro, Action::SetVar::EvaluateType::Decrease));
    } catch (const std::bad_any_cast& ex) {
      return ex.what();
    }
  } else {
    actions.emplace_back(std::make_unique<Action::SetVar>(ctx->VAR_NAME()->getText(),
                                                          ctx->VAR_VALUE()->getText(),
                                                          Action::SetVar::EvaluateType::Decrease));
  }
  return "";
};

std::any Visitor::visitAction_non_disruptive_setvar_macro_tx(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_txContext* ctx) {
  std::shared_ptr<Macro::MacroBase> macro = std::make_shared<Macro::Tx>(ctx->VAR_NAME()->getText());
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_remote_addr(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_remote_addrContext* ctx) {
  return "Not implemented!";
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_user_id(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_user_idContext* ctx) {
  return "Not implemented!";
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_highest_severity(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_highest_severityContext* ctx) {
  return "Not implemented!";
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_matched_var(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_matched_varContext* ctx) {
  return "Not implemented!";
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_matched_var_name(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_matched_var_nameContext* ctx) {
  return "Not implemented!";
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_strict_error(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_multipart_strict_errorContext*
        ctx) {
  return "Not implemented!";
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_rule(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_ruleContext* ctx) {
  return "Not implemented!";
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_session(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_sessionContext* ctx) {
  return "Not implemented!";
}

std::any Visitor::visitAction_non_disruptive_setenv(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setenvContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();

  if (ctx->action_non_disruptive_setvar_macro()) {
    try {
      std::shared_ptr<Macro::MacroBase> macro = std::any_cast<std::shared_ptr<Macro::MacroBase>>(
          visitChildren(ctx->action_non_disruptive_setvar_macro()));
      actions.emplace_back(std::make_unique<Action::SetEnv>(ctx->VAR_NAME()->getText(), macro));
    } catch (const std::bad_any_cast& ex) {
      return ex.what();
    }
  } else {
    actions.emplace_back(
        std::make_unique<Action::SetEnv>(ctx->VAR_NAME()->getText(), ctx->VAR_VALUE()->getText()));
  }

  return "";
}

std::any Visitor::visitSec_audit_engine(Antlr4Gen::SecLangParser::Sec_audit_engineContext* ctx) {
  using Option = SrSecurity::Antlr4::Parser::AuditLogConfig::AuditEngine;
  Option option = Option::Off;

  std::string option_str = ctx->AUDIT_ENGINE()->getText();
  if (option_str == "On") {
    option = Option::On;
  } else if (option_str == "RelevantOnly") {
    option = Option::RelevantOnly;
  }
  parser_->secAuditEngine(option);
  return "";
}

std::any Visitor::visitSec_audit_log(Antlr4Gen::SecLangParser::Sec_audit_logContext* ctx) {
  std::string path = ctx->STRING()->getText();
  parser_->secAuditLog(std::move(path));
  return "";
}

std::any Visitor::visitSec_audit_log2(Antlr4Gen::SecLangParser::Sec_audit_log2Context* ctx) {
  std::string path = ctx->STRING()->getText();
  parser_->secAuditLog2(std::move(path));
  return "";
}

std::any
Visitor::visitSec_audit_log_dir_mode(Antlr4Gen::SecLangParser::Sec_audit_log_dir_modeContext* ctx) {
  int mode = ::strtol(ctx->OCTAL()->getText().c_str(), nullptr, 8);
  parser_->secAuditLogDirMode(mode);
  return "";
}

std::any
Visitor::visitSec_audit_log_format(Antlr4Gen::SecLangParser::Sec_audit_log_formatContext* ctx) {
  using Format = SrSecurity::Antlr4::Parser::AuditLogConfig::AuditFormat;
  Format format = Format::Native;

  std::string format_str = ctx->AUDIT_FORMAT()->getText().c_str();
  if (format_str == "JSON") {
    format = Format::Json;
  }
  parser_->secAuditLogFormat(format);
  return "";
}

std::any Visitor::visitSec_audit_log_file_mode(
    Antlr4Gen::SecLangParser::Sec_audit_log_file_modeContext* ctx) {
  int mode = ::strtol(ctx->OCTAL()->getText().c_str(), nullptr, 8);
  parser_->secAuditLogFileMode(mode);
  return "";
}

std::any
Visitor::visitSec_audit_log_parts(Antlr4Gen::SecLangParser::Sec_audit_log_partsContext* ctx) {
  std::string parts = ctx->AUDIT_PARTS()->getText();
  parser_->secAuditLogParts(parts);
  return "";
}

std::any Visitor::visitSec_audit_log_relevant_status(
    Antlr4Gen::SecLangParser::Sec_audit_log_relevant_statusContext* ctx) {
  std::string pattern = ctx->STRING()->getText();
  parser_->secAuditLogRelevantStatus(std::move(pattern));
  return "";
}

std::any Visitor::visitSec_audit_log_storage_dir(
    Antlr4Gen::SecLangParser::Sec_audit_log_storage_dirContext* ctx) {
  std::string dir = ctx->STRING()->getText();
  parser_->secAuditLogStorageDir(std::move(dir));
  return "";
}

std::any
Visitor::visitSec_audit_log_type(Antlr4Gen::SecLangParser::Sec_audit_log_typeContext* ctx) {
  using Type = SrSecurity::Antlr4::Parser::AuditLogConfig::AuditLogType;
  Type type = Type::Serial;

  std::string type_str = ctx->AUDIT_TYPE()->getText();
  if (type_str == "Concurrent") {
    type = Type::Concurrent;
  } else if (type_str == "HTTPS") {
    type = Type::Https;
  }
  parser_->secAuditLogType(type);
  return "";
}

std::any Visitor::visitSec_component_signature(
    Antlr4Gen::SecLangParser::Sec_component_signatureContext* ctx) {
  std::string signature = ctx->STRING()->getText();
  parser_->secComponentSignature(std::move(signature));
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