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

std::unordered_map<std::string,
                   std::function<std::unique_ptr<Action::ActionBase>(std::string&&, std::string&&)>>
    Parser::action_factory_ = {
        {"accuracy", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"allow", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"auditlog", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"block", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"capture", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"chain", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"ctl", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"deny", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"drop", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"exec", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"expirevar", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"id",
         [](std::string&& name, std::string&& value) {
           return std::make_unique<Action::Id>(std::move(name), std::move(value));
         }},
        {"initcol", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"log", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"logdata", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"maturity", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"msg",
         [](std::string&& name, std::string&& value) {
           return std::make_unique<Action::Msg>(std::move(name), std::move(value));
         }},
        {"multiMatch", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"noauditlog", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"nolog", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"pass", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"phase", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"redirect", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"rev", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"severity", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"setuid", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"setrsc", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"setsid", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"setenv", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"setvar", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"skip", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"skipAfter", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"status", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"t", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"tag",
         [](std::string&& name,
            std::string&&
                value) { return std::make_unique<Action::Tag>(std::move(name), std::move(value)); }},
        {"ver", [](std::string&& name, std::string&& value) { return nullptr; }},
        {"xmlns", [](std::string&& name, std::string&& value) { return nullptr; }}};

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
  try {
    error = std::any_cast<std::string>(vistor.visit(tree));
  } catch (const std::exception&) {
  }

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
  try {
    error = std::any_cast<std::string>(vistor.visit(tree));
  } catch (const std::exception&) {
  }

  return error;
}
} // namespace SrSecurity::Antlr4