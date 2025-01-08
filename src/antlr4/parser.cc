#include "parser.h"

#include <format>
#include <fstream>

#include "antlr4_gen/SecLangLexer.h"
#include "antlr4_gen/SecLangParser.h"
#include "visitor.h"

#include "../action/id.h"
#include "../action/msg.h"
#include "../action/tag.h"
#include "../common/likely.h"
#include "../common/try.h"
#include "../operator/begins_with.h"
#include "../operator/contains.h"
#include "../operator/contains_word.h"
#include "../operator/rx.h"
#include "../variable/args.h"
#include "../variable/args_get.h"
#include "../variable/args_post.h"

// I don't know why vscode intelli sense was too slowly when placed the code at here, so put them
// into the file.
#include "parser.inl"

namespace SrSecurity::Antlr4 {

class ParserErrorListener : public antlr4::BaseErrorListener {
public:
  void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
                   size_t charPositionInLine, const std::string& msg,
                   std::exception_ptr e) override {
    error_msg = std::format("parser error. line {}:{} {}", line, charPositionInLine, msg);
  }

public:
  std::string error_msg;
};

class LexerErrorListener : public antlr4::BaseErrorListener {
public:
  void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
                   size_t charPositionInLine, const std::string& msg,
                   std::exception_ptr e) override {
    error_msg = std::format("lexer error. line {}:{} {}", line, charPositionInLine, msg);
  }

public:
  std::string error_msg;
};

std::string Parser::loadFromFile(const std::string& file_path) {
  // init
  std::ifstream ifs(file_path);
  antlr4::ANTLRInputStream input(ifs);
  Antlr4Gen::SecLangLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  Antlr4Gen::SecLangParser parser(&tokens);

  // sets error listener
  ParserErrorListener parser_error_listener;
  LexerErrorListener lexer_error_listener;
  // parser.setBuildParseTree(true);
  parser.removeErrorListeners();
  parser.addErrorListener(&parser_error_listener);
  lexer.removeErrorListeners();
  lexer.addErrorListener(&lexer_error_listener);

  // parse
  auto tree = parser.configuration();
  if (!parser_error_listener.error_msg.empty()) {
    return parser_error_listener.error_msg;
  }
  if (!lexer_error_listener.error_msg.empty()) {
    return lexer_error_listener.error_msg;
  }

  // visit
  std::string error;
  Visitor vistor(this);
  TRY_NOCATCH(error = std::any_cast<std::string>(vistor.visit(tree)));

  return error;
}

std::string Parser::load(const std::string& directive) {
  // init
  antlr4::ANTLRInputStream input(directive);
  Antlr4Gen::SecLangLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  Antlr4Gen::SecLangParser parser(&tokens);

  // sets error listener
  ParserErrorListener parser_error_listener;
  LexerErrorListener lexer_error_listener;
  // parser.setBuildParseTree(true);
  parser.removeErrorListeners();
  parser.addErrorListener(&parser_error_listener);
  lexer.removeErrorListeners();
  lexer.addErrorListener(&lexer_error_listener);

  // parse
  auto tree = parser.configuration();
  if (!parser_error_listener.error_msg.empty()) {
    return parser_error_listener.error_msg;
  }
  if (!lexer_error_listener.error_msg.empty()) {
    return lexer_error_listener.error_msg;
  }

  // visit
  std::string error;
  Visitor vistor(this);
  TRY_NOCATCH(error = std::any_cast<std::string>(vistor.visit(tree)));

  return error;
}

void Parser::secRuleEngine(EngineConfig::Option option) { engine_config_.is_rule_engine_ = option; }

void Parser::secRequestBodyAccess(EngineConfig::Option option) {
  engine_config_.is_request_body_access_ = option;
}

void Parser::secResponseBodyAccess(EngineConfig::Option option) {
  engine_config_.is_response_body_access_ = option;
}

void Parser::secTmpSaveUploadedFiles(EngineConfig::Option option) {
  engine_config_.is_tmp_save_uploaded_files_ = option;
}

void Parser::secUploadKeepFiles(EngineConfig::Option option) {
  engine_config_.is_upload_keep_files_ = option;
}

void Parser::secXmlExternalEntity(EngineConfig::Option option) {
  engine_config_.is_xml_external_entity_ = option;
}

void Parser::secRule(std::vector<VariableAttr>&& variable_attrs, std::string&& operator_name,
                     std::string&& operator_value,
                     std::unordered_multimap<std::string, std::string>&& actions) {
  auto& rule = rules_.emplace_back(std::make_unique<Rule>());

  // Append variable
  for (auto& attr : variable_attrs) {
    auto iter = variable_factory_.find(attr.main_name_);
    if (iter != variable_factory_.end()) {
      rule->appendVariable(
          iter->second(std::move(attr.full_name_), attr.is_not_, attr.is_counter_));
    }
  }

  // Sets operator
  {
    auto iter = operator_factory_.find(operator_name);
    if (iter != operator_factory_.end()) {
      rule->setOperator(iter->second(std::move(operator_name), std::move(operator_value)));
    }
  }

  // Sets action
  for (auto& [name, value] : actions) {
    auto iter = action_factory_.find(name);
    if (iter != action_factory_.end()) {
      iter->second(*this, *rule, std::move(value));
    }
  }
}

void Parser::secRuleRemoveById(uint64_t id) {
  auto iter = rules_index_id_.find(id);
  if (iter != rules_index_id_.end()) {
    rules_.erase(iter->second);
    rules_index_id_.erase(iter);
  }
}

void Parser::secRuleRemoveByMsg(const std::string& msg) {
  auto range = rules_index_msg_.equal_range(msg);
  for (auto iter = range.first; iter != range.second; ++iter) {
    rules_.erase(iter->second);
  }
  rules_index_msg_.erase(msg);
}

void Parser::secRuleRemoveByTag(const std::string& tag) {
  auto range = rules_index_tag_.equal_range(tag);
  for (auto iter = range.first; iter != range.second; ++iter) {
    rules_.erase(iter->second);
  }
  rules_index_tag_.erase(tag);
}

void Parser::secRuleUpdateActionById(uint64_t id,
                                     std::unordered_multimap<std::string, std::string>&& actions) {
  auto iter = rules_index_id_.find(id);
  if (iter != rules_index_id_.end()) {
    auto& rule = *iter->second;
    if (actions.find("tag") != actions.end()) {
      rule->tags_.clear();
    }

    for (auto& [name, value] : actions) {
      auto iter = action_factory_.find(name);
      iter->second(*this, *rule, std::move(value));
    }
  }
}

void Parser::secRuleUpdateTargetById(uint64_t id, std::vector<VariableAttr>&& variable_attrs) {
  auto iter = rules_index_id_.find(id);
  if (iter != rules_index_id_.end()) {
    auto& rule = *iter->second;

    // Append variable
    for (auto& attr : variable_attrs) {
      auto iter = variable_factory_.find(attr.main_name_);
      if (iter != variable_factory_.end()) {
        rule->removeVariable(attr.full_name_);
        rule->appendVariable(
            iter->second(std::move(attr.full_name_), attr.is_not_, attr.is_counter_));
      }
    }
  }
}

void Parser::secRuleUpdateTargetByMsg(const std::string& msg,
                                      std::vector<VariableAttr>&& variable_attrs) {
  auto range = rules_index_msg_.equal_range(msg);
  for (auto iter = range.first; iter != range.second; ++iter) {
    auto& rule = *iter->second;

    // Append variable
    for (auto& attr : variable_attrs) {
      auto iter = variable_factory_.find(attr.main_name_);
      if (iter != variable_factory_.end()) {
        rule->removeVariable(attr.full_name_);
        rule->appendVariable(
            iter->second(std::move(attr.full_name_), attr.is_not_, attr.is_counter_));
      }
    }
  }
}

void Parser::secRuleUpdateTargetByTag(const std::string& tag,
                                      std::vector<VariableAttr>&& variable_attrs) {
  auto range = rules_index_tag_.equal_range(tag);
  for (auto iter = range.first; iter != range.second; ++iter) {
    auto& rule = *iter->second;

    // Append variable
    for (auto& attr : variable_attrs) {
      auto iter = variable_factory_.find(attr.main_name_);
      if (iter != variable_factory_.end()) {
        rule->removeVariable(attr.full_name_);
        rule->appendVariable(
            iter->second(std::move(attr.full_name_), attr.is_not_, attr.is_counter_));
      }
    }
  }
}

void Parser::secAuditEngine(AuditLogConfig::AuditEngine option) {
  audit_log_config_.audit_engine_ = option;
}

void Parser::secAuditLog(std::string&& path) { audit_log_config_.log_path_ = std::move(path); }

void Parser::secAuditLog2(std::string&& path) { audit_log_config_.log_path2_ = std::move(path); }

void Parser::secAuditLogDirMode(int mode) { audit_log_config_.dir_mode_ = mode; }

void Parser::secAuditLogFormat(AuditLogConfig::AuditFormat format) {
  audit_log_config_.format_ = format;
}

void Parser::secAuditLogFileMode(int mode) { audit_log_config_.file_mode_ = mode; }

void Parser::secAuditLogParts(const std::string& parts) {
  for (auto ch : parts) {
    switch (ch) {
    case 'A':
      audit_log_config_.log_parts_[static_cast<int>(AuditLogConfig::AuditLogPart::A)] = true;
      break;
    case 'B':
      audit_log_config_.log_parts_[static_cast<int>(AuditLogConfig::AuditLogPart::B)] = true;
      break;
    case 'C':
      audit_log_config_.log_parts_[static_cast<int>(AuditLogConfig::AuditLogPart::C)] = true;
      break;
    case 'D':
      audit_log_config_.log_parts_[static_cast<int>(AuditLogConfig::AuditLogPart::D)] = true;
      break;
    case 'E':
      audit_log_config_.log_parts_[static_cast<int>(AuditLogConfig::AuditLogPart::E)] = true;
      break;
    case 'F':
      audit_log_config_.log_parts_[static_cast<int>(AuditLogConfig::AuditLogPart::F)] = true;
      break;
    case 'G':
      audit_log_config_.log_parts_[static_cast<int>(AuditLogConfig::AuditLogPart::G)] = true;
      break;
    case 'H':
      audit_log_config_.log_parts_[static_cast<int>(AuditLogConfig::AuditLogPart::H)] = true;
      break;
    case 'I':
      audit_log_config_.log_parts_[static_cast<int>(AuditLogConfig::AuditLogPart::I)] = true;
      break;
    case 'J':
      audit_log_config_.log_parts_[static_cast<int>(AuditLogConfig::AuditLogPart::J)] = true;
      break;
    case 'K':
      audit_log_config_.log_parts_[static_cast<int>(AuditLogConfig::AuditLogPart::K)] = true;
      break;
    case 'Z':
      audit_log_config_.log_parts_[static_cast<int>(AuditLogConfig::AuditLogPart::Z)] = true;
      break;
    default:
      break;
    }
  }
}

void Parser::secAuditLogRelevantStatus(std::string&& pattern) {
  audit_log_config_.relevant_status_regex_ = std::move(pattern);
}

void Parser::secAuditLogStorageDir(std::string&& dir) {
  audit_log_config_.storage_dir_ = std::move(dir);
}

void Parser::secAuditLogType(AuditLogConfig::AuditLogType type) {
  audit_log_config_.audit_log_type_ = type;
}

void Parser::secComponentSignature(std::string&& signature) {
  audit_log_config_.component_signature_ = std::move(signature);
}

} // namespace SrSecurity::Antlr4