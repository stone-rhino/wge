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

namespace SrSecurity::Antlr4 {

std::unordered_map<
    std::string, std::function<std::unique_ptr<Variable::VariableBase>(std::string&&, bool, bool)>>
    Parser::variable_factory_ = {
        {"ARGS",
         [](std::string&& full_name, bool is_not, bool is_counter) {
           return std::make_unique<Variable::Args>(std::move(full_name), is_not, is_counter);
         }},
        {"ARGS_COMBINED_SIZE",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"ARGS_GET",
         [](std::string&& full_name, bool is_not, bool is_counter) {
           return std::make_unique<Variable::ArgsGet>(std::move(full_name), is_not, is_counter);
         }},
        {"ARGS_GET_NAMES",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"ARGS_NAMES",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"ARGS_POST",
         [](std::string&& full_name, bool is_not, bool is_counter) {
           return std::make_unique<Variable::ArgsPost>(std::move(full_name), is_not, is_counter);
         }},
        {"ARGS_POST_NAMES",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"AUTH_TYPE",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},

        {"DURATION", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},

        {"ENV", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},

        {"FILES", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},

        {"FILES_COMBINED_SIZE",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"FILES_NAMES",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"FULL_REQUEST",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"FULL_REQUEST_LENGTH",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"FILES_SIZES",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"FILES_TMPNAMES",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"FILES_TMP_CONTENT",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"GEO", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"HIGHEST_SEVERITY",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"INBOUND_DATA_ERROR",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"MATCHED_VAR",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"MATCHED_VARS",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"MATCHED_VAR_NAME",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"MATCHED_VARS_NAMES",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"MODSEC_BUILD",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"MSC_PCRE_LIMITS_EXCEEDED",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"MULTIPART_CRLF_LF_LINES",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"MULTIPART_FILENAME",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"MULTIPART_NAME",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"MULTIPART_PART_HEADERS",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"MULTIPART_STRICT_ERROR",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"MULTIPART_UNMATCHED_BOUNDARY",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"OUTBOUND_DATA_ERROR",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"PATH_INFO",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"QUERY_STRING",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REMOTE_ADDR",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REMOTE_HOST",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REMOTE_PORT",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REMOTE_USER",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQBODY_ERROR",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQBODY_ERROR_MSG",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQBODY_PROCESSOR",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_BASENAME",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_BODY",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_BODY_LENGTH",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_COOKIES",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_COOKIES_NAMES",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_FILENAME",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_HEADERS",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_HEADERS_NAMES",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_LINE",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_METHOD",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_PROTOCOL",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_URI",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"REQUEST_URI_RAW",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"RESPONSE_BODY",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"RESPONSE_CONTENT_LENGTH",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"RESPONSE_CONTENT_TYPE",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"RESPONSE_HEADERS",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"RESPONSE_HEADERS_NAMES",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"RESPONSE_PROTOCOL",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"RESPONSE_STATUS",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"RULE", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"SERVER_ADDR",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"SERVER_NAME",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"SERVER_PORT",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"SESSION", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"SESSIONID",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"STATUS_LINE",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"TIME", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"TIME_DAY", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"TIME_EPOCH",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"TIME_HOUR",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"TIME_MIN", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"TIME_MON", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"TIME_SEC", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"TIME_WDAY",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"TIME_YEAR",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"TX", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"UNIQUE_ID",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"URLENCODED_ERROR",
         [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"USERID", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"WEBAPPID", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
        {"XML", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }}};

std::unordered_map<std::string, std::function<std::unique_ptr<Operator::OperatorBase>(
                                    std::string&&, std::string&&)>>
    Parser::operator_factory_ = {{"beginsWith",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return std::make_unique<Operator::BeginsWith>(
                                        std::move(operator_name), std::move(operator_value));
                                  }},
                                 {"contains",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return std::make_unique<Operator::Contains>(
                                        std::move(operator_name), std::move(operator_value));
                                  }},
                                 {"containsWord",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return std::make_unique<Operator::ContainsWord>(
                                        std::move(operator_name), std::move(operator_value));
                                  }},
                                 {"detectSQLi",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"detectXSS",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"endsWith", [](std::string&& operator_name,
                                                 std::string&& operator_value) { return nullptr; }},
                                 {"fuzzyHash",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"eq", [](std::string&& operator_name,
                                           std::string&& operator_value) { return nullptr; }},
                                 {"ge", [](std::string&& operator_name,
                                           std::string&& operator_value) { return nullptr; }},
                                 {"geoLookup",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"gsbLookup",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"gt", [](std::string&& operator_name,
                                           std::string&& operator_value) { return nullptr; }},
                                 {"inspectFile",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"ipMatch", [](std::string&& operator_name,
                                                std::string&& operator_value) { return nullptr; }},
                                 {"ipMatchF", [](std::string&& operator_name,
                                                 std::string&& operator_value) { return nullptr; }},
                                 {"ipMatchFromFile",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"le",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"lt",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"noMatch",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"pm",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"pmf",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"pmFromFile",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"rbl",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"rsub",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"rx",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return std::make_unique<Operator::Rx>(
                                        std::move(operator_name), std::move(operator_value));
                                  }},
                                 {"rxGlobal", [](std::string&& operator_name,
                                                 std::string&& operator_value) { return nullptr; }},
                                 {"streq", [](std::string&& operator_name,
                                              std::string&& operator_value) { return nullptr; }},
                                 {"strmatch", [](std::string&& operator_name,
                                                 std::string&& operator_value) { return nullptr; }},
                                 {"unconditionalMatch",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"validateByteRange",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"validateDTD",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"validateSchema",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"validateUrlEncoding",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"validateUtf8Encoding",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"verifyCC", [](std::string&& operator_name,
                                                 std::string&& operator_value) { return nullptr; }},
                                 {"verifyCPF",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"verifySSN",
                                  [](std::string&& operator_name, std::string&& operator_value) {
                                    return nullptr;
                                  }},
                                 {"within", [](std::string&& operator_name,
                                               std::string&& operator_value) { return nullptr; }}};

std::unordered_map<std::string, std::function<void(Rule&, std::string&&)>> Parser::action_factory_ =
    {{"accuracy", [](Rule& rule, std::string&& value) { rule.accuracy_ = std::move(value); }},
     {"allow", [](Rule& rule, std::string&& value) { rule.disruptive_ = Rule::Disruptive::ALLOW; }},
     {"auditlog", [](Rule& rule, std::string&& value) { rule.audit_log_ = true; }},
     {"block", [](Rule& rule, std::string&& value) { rule.disruptive_ = Rule::Disruptive::BLOCK; }},
     {"capture", [](Rule& rule, std::string&& value) { rule.capture_ = true; }},
     {"chain", [](Rule& rule, std::string&& value) { rule.chain_ = true; }},
     {"ctl", [](Rule& rule, std::string&& value) { rule.ctl_ = std::move(value); }},
     {"deny", [](Rule& rule, std::string&& value) { rule.disruptive_ = Rule::Disruptive::DENY; }},
     {"drop", [](Rule& rule, std::string&& value) { rule.disruptive_ = Rule::Disruptive::DENY; }},
     {"exec", [](Rule& rule, std::string&& value) { rule.exec_ = std::move(value); }},
     {"expirevar", [](Rule& rule, std::string&& value) { rule.expire_var_ = std::move(value); }},
     {"id", [](Rule& rule, std::string&& value) { rule.id_ = ::atoll(value.c_str()); }},
     {"initcol", [](Rule& rule, std::string&& value) { rule.init_col_ = std::move(value); }},
     {"log", [](Rule& rule, std::string&& value) { rule.log_ = true; }},
     {"logdata", [](Rule& rule, std::string&& value) { rule.log_data_ = std::move(value); }},
     {"maturity", [](Rule& rule, std::string&& value) { rule.maturity_ = std::move(value); }},
     {"msg", [](Rule& rule, std::string&& value) { rule.msg_ = std::move(value); }},
     {"multiMatch", [](Rule& rule, std::string&& value) { rule.multi_match_ = true; }},
     {"noauditlog", [](Rule& rule, std::string&& value) { rule.no_audit_log_ = true; }},
     {"nolog", [](Rule& rule, std::string&& value) { rule.no_log_ = true; }},
     {"pass", [](Rule& rule, std::string&& value) { rule.disruptive_ = Rule::Disruptive::PASS; }},
     {"phase", [](Rule& rule, std::string&& value) { rule.phase_ = std::move(value); }},
     {"redirect",
      [](Rule& rule, std::string&& value) {
        rule.disruptive_ = Rule::Disruptive::REDIRECT;
        rule.redirect_ = std::move(value);
      }},
     {"rev", [](Rule& rule, std::string&& value) { rule.rev_ = std::move(value); }},
     {"severity", [](Rule& rule, std::string&& value) { return; }},
     {"setuid", [](Rule& rule, std::string&& value) { return; }},
     {"setrsc", [](Rule& rule, std::string&& value) { return; }},
     {"setsid", [](Rule& rule, std::string&& value) { return; }},
     {"setenv", [](Rule& rule, std::string&& value) { return; }},
     {"setvar", [](Rule& rule, std::string&& value) { return; }},
     {"skip", [](Rule& rule, std::string&& value) { return; }},
     {"skipAfter", [](Rule& rule, std::string&& value) { return; }},
     {"status", [](Rule& rule, std::string&& value) { return; }},
     {"t", [](Rule& rule, std::string&& value) { return; }},
     {"tag", [](Rule& rule, std::string&& value) { rule.tag_.emplace(std::move(value)); }},
     {"ver", [](Rule& rule, std::string&& value) { return; }},
     {"xmlns", [](Rule& rule, std::string&& value) { return; }}};

class ErrorListener : public antlr4::BaseErrorListener {
public:
  void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
                   size_t charPositionInLine, const std::string& msg,
                   std::exception_ptr e) override {
    error_msg = std::format("line {}:{} {}", line, charPositionInLine, msg);
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
  ErrorListener error_listener;
  // parser.setBuildParseTree(true);
  parser.removeErrorListeners();
  parser.addErrorListener(&error_listener);

  // parse
  auto tree = parser.configuration();
  if (!error_listener.error_msg.empty()) {
    return error_listener.error_msg;
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
  ErrorListener error_listener;
  // parser.setBuildParseTree(true);
  parser.removeErrorListeners();
  parser.addErrorListener(&error_listener);

  // parse
  auto tree = parser.configuration();
  if (!error_listener.error_msg.empty()) {
    return error_listener.error_msg;
  }

  // visit
  std::string error;
  Visitor vistor(this);
  TRY_NOCATCH(error = std::any_cast<std::string>(vistor.visit(tree)));

  return error;
}
} // namespace SrSecurity::Antlr4