#include "parser.h"

#include <array>
#include <format>
#include <fstream>

#include "antlr4_gen/SecLangLexer.h"
#include "antlr4_gen/SecLangParser.h"
#include "visitor.h"

#include "../common/try.h"
#include "../operator/begins_with.h"
#include "../operator/contains.h"
#include "../operator/contains_word.h"
#include "../operator/rx.h"
#include "../variable/args.h"
#include "../variable/args_get.h"
#include "../variable/args_post.h"

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

std::expected<bool, std::string> Parser::loadFromFile(const std::string& file_path) {
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
    return std::unexpected(parser_error_listener.error_msg);
  }
  if (!lexer_error_listener.error_msg.empty()) {
    return std::unexpected(lexer_error_listener.error_msg);
  }

  curr_load_file_ = "";
  auto result = loaded_file_paths_.emplace(file_path);
  if (result.second) {
    curr_load_file_ = *result.first;
  } else {
    auto iter = loaded_file_paths_.find(file_path);
    if (iter != loaded_file_paths_.end()) {
      curr_load_file_ = *iter;
    }
  }

  // visit
  std::string error;
  Visitor vistor(this);
  TRY_NOCATCH(error = std::any_cast<std::string>(vistor.visit(tree)));

  if (!error.empty()) {
    return std::unexpected(error);
  }

  return true;
}

std::expected<bool, std::string> Parser::load(const std::string& directive) {
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
    return std::unexpected(parser_error_listener.error_msg);
  }
  if (!lexer_error_listener.error_msg.empty()) {
    return std::unexpected(lexer_error_listener.error_msg);
  }

  // visit
  std::string error;
  Visitor vistor(this);
  TRY_NOCATCH(error = std::any_cast<std::string>(vistor.visit(tree)));

  if (!error.empty()) {
    return std::unexpected(error);
  }

  return true;
}

void Parser::secRuleEngine(EngineConfig::Option option) { engine_config_.is_rule_engine_ = option; }

void Parser::secRequestBodyAccess(EngineConfig::Option option) {
  engine_config_.is_request_body_access_ = option;
}

void Parser::secResponseBodyMimeType(const std::vector<std::string>& mime_types) {
  // Multiple SecResponseBodyMimeType directives can be used to add MIME types. Use
  // SecResponseBodyMimeTypesClear to clear previously configured MIME types and start over.
  for (auto& mime_type : mime_types) {
    engine_config_.response_body_mime_types_.emplace_back(mime_type);
  }
}

void Parser::secResponseBodyMimeTypeClear() { engine_config_.response_body_mime_types_.clear(); }

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

void Parser::secRequestBodyLimit(uint64_t limit_bytes) {
  engine_config_.request_body_limit_ = limit_bytes;
}

void Parser::secRequestBodyNoFilesLimit(uint64_t limit_bytes) {
  engine_config_.request_body_no_files_limit_ = limit_bytes;
}

void Parser::secRequestBodyJsonDepthLimit(uint64_t limit) {
  engine_config_.request_body_json_depth_limit_ = limit;
}

void Parser::secRequsetBodyLimitAction(EngineConfig::BodyLimitAction action) {
  engine_config_.request_body_limit_action_ = action;
}

void Parser::secResponseBodyLimit(uint64_t limit_bytes) {
  engine_config_.response_body_limit_ = limit_bytes;
}

void Parser::secResponseBodyLimitAction(EngineConfig::BodyLimitAction action) {
  engine_config_.response_body_limit_action_ = action;
}

void Parser::secArgumentsLimit(uint32_t limit_bytes) {
  engine_config_.arguments_limit_ = limit_bytes;
}

void Parser::secArgumentSeparator(char separator) {
  engine_config_.argument_separator_ = separator;
}

void Parser::secUnicodeMapFile(std::string&& file_path, uint32_t code_point) {
  engine_config_.unicode_map_file_ = std::move(file_path);
  engine_config_.unicode_code_point_ = code_point;
}

void Parser::secPcreMatchLimit(uint32_t limit) { engine_config_.pcre_match_limit_ = limit; }

std::list<std::unique_ptr<Rule>>::iterator Parser::secAction(int line) {
  rules_.emplace_back(std::make_unique<Rule>(curr_load_file_, line));
  return std::prev(rules_.end());
}

std::list<std::unique_ptr<Rule>>::iterator Parser::secRule(int line) {
  rules_.emplace_back(std::make_unique<Rule>(curr_load_file_, line));
  return std::prev(rules_.end());
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

void Parser::secMarker(std::string&& name) {
  std::array<const Rule*, Marker::phase_total_> prev_rules{nullptr};

  // Get the previous rule in each phase
  int geted = 0;
  for (auto iter = rules_.rbegin(); iter != rules_.rend(); ++iter) {
    auto& rule = *iter;
    int phase = rule->phase();
    if (phase != -1) {
      if (prev_rules[phase - 1] == nullptr) {
        prev_rules[phase - 1] = rule.get();
        ++geted;
      }
    }

    // each phase has a previous rule then break
    if (geted == Marker::phase_total_) {
      break;
    }
  }

  makers_.emplace_back(std::move(name), std::move(prev_rules));
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

void Parser::removeBackRule() {
  // remove index
  auto iter = std::prev(rules_.end());
  clearRuleIdIndex(iter);
  clearRuleMsgIndex(iter);
  clearRuleTagIndex(iter);

  // remove rule
  rules_.erase(std::prev(rules_.end()));
}

void Parser::setRuleIdIndex(std::list<std::unique_ptr<Rule>>::iterator iter) {
  rules_index_id_[(*iter)->id()] = iter;
}

void Parser::clearRuleIdIndex(std::list<std::unique_ptr<Rule>>::iterator iter) {
  // remove id index
  std::erase_if(rules_index_id_,
                [&](const std::pair<uint64_t, std::list<std::unique_ptr<Rule>>::iterator>& pair) {
                  if (pair.second == iter) {
                    return true;
                  }
                  return false;
                });
}

void Parser::setRuleMsgIndex(std::list<std::unique_ptr<Rule>>::iterator iter) {
  rules_index_msg_.insert({(*iter)->msg(), iter});
}

void Parser::clearRuleMsgIndex(std::list<std::unique_ptr<Rule>>::iterator iter) {
  // remove msg index
  std::erase_if(
      rules_index_msg_,
      [&](const std::pair<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>& pair) {
        if (pair.second == iter) {
          return true;
        }
        return false;
      });
}

void Parser::setRuleTagIndex(std::list<std::unique_ptr<Rule>>::iterator iter,
                             const std::string_view& tag) {
  rules_index_tag_.insert({tag, iter});
}

void Parser::clearRuleTagIndex(std::list<std::unique_ptr<Rule>>::iterator iter) {
  // remove tag index
  std::erase_if(
      rules_index_tag_,
      [&](const std::pair<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>& pair) {
        if (pair.second == iter) {
          return true;
        }
        return false;
      });
}

std::list<std::unique_ptr<Rule>>::iterator Parser::findRuleById(uint64_t id) {
  auto iter = rules_index_id_.find(id);
  if (iter != rules_index_id_.end()) {
    return iter->second;
  }

  return rules_.end();
}

std::pair<
    std::unordered_multimap<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>::iterator,
    std::unordered_multimap<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>::iterator>
Parser::findRuleByMsg(const std::string& msg) {
  return rules_index_msg_.equal_range(msg);
}

std::pair<
    std::unordered_multimap<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>::iterator,
    std::unordered_multimap<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>::iterator>
Parser::findRuleByTag(const std::string& tag) {
  return rules_index_tag_.equal_range(tag);
}
} // namespace SrSecurity::Antlr4