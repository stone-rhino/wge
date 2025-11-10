/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "parser.h"

#include <array>
#include <format>
#include <fstream>
#include <string_view>

#include "antlr4_gen/SecLangLexer.h"
#include "antlr4_gen/SecLangParser.h"
#include "visitor.h"

#include "../common/assert.h"
#include "../common/try.h"
#include "../operator/begins_with.h"
#include "../operator/contains.h"
#include "../operator/contains_word.h"
#include "../operator/rx.h"
#include "../variable/args.h"
#include "../variable/args_get.h"
#include "../variable/args_post.h"

namespace Wge::Antlr4 {
class ParserErrorListener : public antlr4::BaseErrorListener {
public:
  ParserErrorListener(std::string_view file_path) : file_path_(file_path) {}

public:
  void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
                   size_t charPositionInLine, const std::string& msg,
                   std::exception_ptr e) override {
    if (file_path_.empty()) {
      error_msg = std::format("parser error. [{}:{}] {}", line, charPositionInLine, msg);
    } else {
      error_msg =
          std::format("parser error. [{}:{}:{}] {}", file_path_, line, charPositionInLine, msg);
    }
  }

public:
  std::string error_msg;

private:
  std::string_view file_path_;
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

Parser::Parser() {
  constexpr size_t tx_variable_index_size = 1000;
  tx_variable_index_.reserve(tx_variable_index_size);
  tx_variable_index_reverse_.reserve(tx_variable_index_size);
}

std::expected<bool, std::string> Parser::loadFromFile(const std::string& file_path) {
  // Init
  std::ifstream ifs(file_path);
  if (!ifs.is_open()) {
    return std::unexpected(std::format("open file {} failed", file_path));
  }
  antlr4::ANTLRInputStream input(ifs);
  Antlr4Gen::SecLangLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  Antlr4Gen::SecLangParser parser(&tokens);

  // Sets error listener
  ParserErrorListener parser_error_listener(file_path);
  LexerErrorListener lexer_error_listener;
  // parser.setBuildParseTree(true);
  parser.removeErrorListeners();
  parser.addErrorListener(&parser_error_listener);
  lexer.removeErrorListeners();
  lexer.addErrorListener(&lexer_error_listener);

  // Parse
  auto tree = parser.configuration();
  if (!parser_error_listener.error_msg.empty()) {
    return std::unexpected(parser_error_listener.error_msg);
  }
  if (!lexer_error_listener.error_msg.empty()) {
    return std::unexpected(lexer_error_listener.error_msg);
  }

  // Push the file path to the stack
  const auto& [inserted_file_iter, success] = loaded_file_paths_.emplace(file_path);
  if (success) {
    curr_load_file_.push(*inserted_file_iter);
  } else {
    auto iter = loaded_file_paths_.find(file_path);
    if (iter != loaded_file_paths_.end()) {
      curr_load_file_.push(*iter);
    }
  }

  // Visit
  std::string error;
  Visitor vistor(this);
  TRY_NOCATCH(error = std::any_cast<std::string>(vistor.visit(tree)));

  curr_load_file_.pop();

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
  ParserErrorListener parser_error_listener("");
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

void Parser::secRuleEngine(EngineConfig::Option option) {
  engine_config_.rule_engine_option_ = option;
}

void Parser::secRequestBodyAccess(bool value) { engine_config_.is_request_body_access_ = value; }

void Parser::secResponseBodyMimeType(const std::vector<std::string>& mime_types) {
  // Multiple SecResponseBodyMimeType directives can be used to add MIME types. Use
  // SecResponseBodyMimeTypesClear to clear previously configured MIME types and start over.
  for (auto& mime_type : mime_types) {
    engine_config_.response_body_mime_types_.emplace_back(mime_type);
  }
}

void Parser::secResponseBodyMimeTypeClear() { engine_config_.response_body_mime_types_.clear(); }

void Parser::secResponseBodyAccess(bool value) { engine_config_.is_response_body_access_ = value; }

void Parser::secTmpSaveUploadedFiles(bool value) {
  engine_config_.is_tmp_save_uploaded_files_ = value;
}

void Parser::secUploadFileLimit(uint32_t limit_count) {
  engine_config_.upload_file_limit_ = limit_count;
}

void Parser::secUploadKeepFiles(bool value) { engine_config_.is_upload_keep_files_ = value; }

void Parser::secXmlExternalEntity(bool value) { engine_config_.is_xml_external_entity_ = value; }

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

void Parser::secParseXmlIntoArgs(ParseXmlIntoArgsOption option) {
  parse_xml_into_args_option_ = option;
}

void Parser::secPcreMatchLimit(uint32_t limit) { engine_config_.pcre_match_limit_ = limit; }

void Parser::secPmfSerializeDir(std::string&& file_path) {
  engine_config_.pmf_serialize_dir_ = std::move(file_path);
}

void Parser::secAction(std::unique_ptr<Rule>&& rule) {
  if (rule->phase() < 1 || rule->phase() > PHASE_TOTAL) {
    assert(false && "The rule must has valid phase");
    return;
  }

  // Check the rule count limit, ensure the index won't overflow
  size_t phase_rules_size = rules_[rule->phase() - 1].size();
  if (static_cast<size_t>(std::numeric_limits<RuleIndexType>::max()) < phase_rules_size) {
    assert(false && "Too many rules in phase");
    return;
  }

  rule->index(phase_rules_size);
  rules_[rule->phase() - 1].emplace_back(std::move(*rule));
  rule.reset();
}

Rule* Parser::secRule(std::unique_ptr<Rule>&& rule) {
  if (rule->phase() < 1 || rule->phase() > PHASE_TOTAL) {
    assert(false && "The rule must has valid phase");
    return nullptr;
  }

  // Check the rule count limit, ensure the index won't overflow
  size_t phase_rules_size = rules_[rule->phase() - 1].size();
  if (static_cast<size_t>(std::numeric_limits<RuleIndexType>::max()) < phase_rules_size) {
    assert(false && "Too many rules in phase");
    return nullptr;
  }

  rule->index(phase_rules_size);
  auto& appended_rule = rules_[rule->phase() - 1].emplace_back(std::move(*rule));
  rule.reset();

  // Set indexes
  setRuleIdIndex({appended_rule.phase(), appended_rule.index()});
  setRuleMsgIndex({appended_rule.phase(), appended_rule.index()});
  for (auto& tag : appended_rule.tags()) {
    setRuleTagIndex({appended_rule.phase(), appended_rule.index()}, tag);
  }

  return &appended_rule;
}

void Parser::secRuleRemoveById(uint64_t id) {
  auto iter = rules_index_id_.find(id);
  if (iter != rules_index_id_.end()) {
    clearRuleIdIndex(iter->second);
    clearRuleTagIndex(iter->second);
    clearRuleMsgIndex(iter->second);
    updateMarker(iter->second);
    updateRuleIndex(iter->second);
    auto& rules = rules_[iter->second.phase_ - 1];
    rules.erase(rules.begin() + iter->second.index_);
  }
}

void Parser::secRuleRemoveByMsg(const std::string& msg) {
  auto rules = findRuleByMsg(msg);
  std::vector<uint64_t> ids;
  for (auto rule : rules) {
    ids.emplace_back(rule->id());
  }

  for (auto id : ids) {
    secRuleRemoveById(id);
  }

  assert(findRuleByMsg(msg).empty());
}

void Parser::secRuleRemoveByTag(const std::string& tag) {
  auto rules = findRuleByTag(tag);
  std::vector<uint64_t> ids;
  for (auto rule : rules) {
    ids.emplace_back(rule->id());
  }

  for (auto id : ids) {
    secRuleRemoveById(id);
  }

  assert(findRuleByTag(tag).empty());
}

void Parser::secMarker(std::string&& name) {
  std::array<RuleIndexType, PHASE_TOTAL> prev_rule_indexes;

  // Get the previous rule index in each phase
  for (size_t i = 0; i < PHASE_TOTAL; ++i) {
    auto& rules = rules_[i];
    prev_rule_indexes[i] = rules.size() - 1;
  }

  markers_.emplace(Rule::intern(std::move(name)), std::move(prev_rule_indexes));
}

void Parser::secDefaultAction(std::unique_ptr<Rule>&& rule) {
  if (rule->phase() < 1 || rule->phase() > PHASE_TOTAL) {
    assert(false && "The rule must has valid phase");
    return;
  }

  default_actions_rules_[rule->phase() - 1] = std::move(*rule);
  rule.reset();
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

void Parser::setRuleIdIndex(RuleIndex rule_index) {
  if (rule_index.index_ == -1) {
    return;
  }

  auto& rule = rules_[rule_index.phase_ - 1][rule_index.index_];
  rules_index_id_[rule.id()] = rule_index;
}

void Parser::setRuleMsgIndex(RuleIndex rule_index) {
  if (rule_index.index_ == -1) {
    return;
  }

  auto& rule = rules_[rule_index.phase_ - 1][rule_index.index_];
  rules_index_msg_.insert({rule.msg(), rule_index});
}

void Parser::setRuleTagIndex(RuleIndex rule_index, std::string_view tag) {
  if (rule_index.index_ == -1) {
    return;
  }

  auto& rule = rules_[rule_index.phase_ - 1][rule_index.index_];
  rules_index_tag_.insert({tag, rule_index});
}

void Parser::clearRuleIdIndex(RuleIndex rule_index) {
  if (rule_index.index_ == -1) {
    return;
  }

  // Remove id index
  auto& rule = rules_[rule_index.phase_ - 1][rule_index.index_];
  rules_index_id_.erase(rule.id());

  // Update other id idexes
  for (auto& id_index : rules_index_id_) {
    if (id_index.second.index_ > rule_index.index_ && id_index.second.phase_ == rule_index.phase_) {
      id_index.second.index_--;
    }
  }
}

void Parser::clearRuleMsgIndex(RuleIndex rule_index) {
  if (rule_index.index_ == -1) {
    return;
  }

  // Remove msg index
  std::erase_if(rules_index_msg_, [&](const std::pair<std::string_view, RuleIndex>& pair) {
    if (pair.second == rule_index) {
      return true;
    }
    return false;
  });

  // Update other msg indexes
  for (auto& msg_index : rules_index_msg_) {
    if (msg_index.second.index_ > rule_index.index_ &&
        msg_index.second.phase_ == rule_index.phase_) {
      msg_index.second.index_--;
    }
  }
}

void Parser::clearRuleTagIndex(RuleIndex rule_index) {
  if (rule_index.index_ == -1) {
    return;
  }

  // Remove tag index
  std::erase_if(rules_index_tag_, [&](const std::pair<std::string_view, RuleIndex>& pair) {
    if (pair.second == rule_index) {
      return true;
    }
    return false;
  });

  // Update other tag indexes
  for (auto& tag_index : rules_index_tag_) {
    if (tag_index.second.index_ > rule_index.index_ &&
        tag_index.second.phase_ == rule_index.phase_) {
      tag_index.second.index_--;
    }
  }
}

void Parser::updateMarker(RuleIndex rule_index) {
  for (auto& [_, prev_rule_indexes] : markers_) {
    if (prev_rule_indexes[rule_index.phase_ - 1] >= rule_index.index_) {
      prev_rule_indexes[rule_index.phase_ - 1]--;
    }
  }
}

void Parser::updateRuleIndex(RuleIndex rule_index) {
  auto& rules = rules_[rule_index.phase_ - 1];
  for (auto i = rule_index.index_ + 1; i < rules.size(); ++i) {
    rules[i].index(i - 1);
  }
}

Rule* Parser::findRuleById(uint64_t id) {
  auto iter = rules_index_id_.find(id);
  if (iter != rules_index_id_.end()) {
    return &(rules_[iter->second.phase_ - 1][iter->second.index_]);
  }

  return nullptr;
}

std::unordered_set<Rule*> Parser::findRuleByMsg(const std::string& msg) {
  std::unordered_set<Rule*> result;
  auto [begin, end] = rules_index_msg_.equal_range(msg);
  for (auto iter = begin; iter != end; ++iter) {
    auto& rule = rules_[iter->second.phase_ - 1][iter->second.index_];
    result.emplace(&rule);
  }
  return result;
}

std::unordered_set<Rule*> Parser::findRuleByTag(const std::string& tag) {
  std::unordered_set<Rule*> result;
  auto [begin, end] = rules_index_tag_.equal_range(tag);
  for (auto iter = begin; iter != end; ++iter) {
    auto& rule = rules_[iter->second.phase_ - 1][iter->second.index_];
    result.emplace(&rule);
  }
  return result;
}

std::optional<size_t> Parser::getTxVariableIndex(const std::string& name, bool force) {
  // The name is case insensitive
  std::string less_case_name;
  less_case_name.reserve(name.size());
  std::transform(name.begin(), name.end(), std::back_inserter(less_case_name), ::tolower);

  auto iter = tx_variable_index_.find(less_case_name);
  if (iter != tx_variable_index_.end()) {
    return iter->second;
  } else {
    if (force) {
      ASSERT_IS_MAIN_THREAD();
      auto [insert_iter, _] =
          tx_variable_index_.insert({less_case_name, tx_variable_index_.size()});
      tx_variable_index_reverse_.emplace_back(insert_iter->first);
      return tx_variable_index_.size() - 1;
    }
  }

  return std::nullopt;
}

std::string_view Parser::getTxVariableIndexReverse(size_t index) const {
  assert(index < tx_variable_index_reverse_.size());
  if (index < tx_variable_index_reverse_.size()) {
    return tx_variable_index_reverse_[index];
  }

  return EMPTY_STRING_VIEW;
}
} // namespace Wge::Antlr4