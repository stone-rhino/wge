#include "visitor.h"

#include <unordered_map>

#include <assert.h>

#include "../operator/begins_with.h"
#include "../operator/contains.h"
#include "../operator/contains_word.h"
#include "../operator/rx.h"
#include "../variable/args.h"
#include "../variable/args_get.h"
#include "../variable/args_post.h"

namespace SrSecurity::Antlr4 {

std::any Visitor::visitInclude(Antlr4Gen::SecLangParser::IncludeContext* ctx) {
  std::string file_path = ctx->STRING()->getText();
  return parser_->loadFromFile(file_path);
}

std::any Visitor::visitEngine_config(Antlr4Gen::SecLangParser::Engine_configContext* ctx) {
  static const std::unordered_map<std::string, Parser::EngineConfig::Option&> enginge_config_map{
      {"SecRequestBodyAccess", parser_->engineConfig().is_request_body_access_},
      {"SecResponseBodyAccess", parser_->engineConfig().is_response_body_access_},
      {"SecRuleEngine", parser_->engineConfig().is_rule_engine_},
      {"SecTmpSaveUploadedFiles", parser_->engineConfig().is_tmp_save_uploaded_files_},
      {"SecUploadKeepFiles", parser_->engineConfig().is_upload_keep_files_},
      {"SecXmlExternalEntity", parser_->engineConfig().is_xml_external_entity_},
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
        iter->second = Parser::EngineConfig::Option::On;
      } else {
        iter->second = Parser::EngineConfig::Option::Off;
      }
    } else {
      iter->second = Parser::EngineConfig::Option::DetectionOnly;
    }
  }
  return "";
}

std::any Visitor::visitRule_directiv(Antlr4Gen::SecLangParser::Rule_directivContext* ctx) {
  parser_->rules().emplace_back(std::make_unique<Rule>());
  return visitChildren(ctx);
}

std::any Visitor::visitVariable(Antlr4Gen::SecLangParser::VariableContext* ctx) {
  // Variable factory
  static std::unordered_map<std::string, std::function<std::unique_ptr<Variable::VariableBase>(
                                             std::string&&, bool, bool)>>
      variable_factory = {
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
          {"DURATION",
           [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
          {"ENV", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
          {"FILES", [](std::string&& full_name, bool is_not, bool is_counter) { return nullptr; }},
      };

  // Construct a varialbe and append it to the rule
  std::string main_name = ctx->var_main_name()->getText();
  auto iter = variable_factory.find(main_name);
  if (iter != variable_factory.end()) {
    std::string full_name = ctx->getText();
    bool is_not = ctx->NOT() != nullptr;
    bool is_counter = ctx->VAR_COUNT() != nullptr;
    auto var = iter->second(std::move(full_name), is_not, is_counter);
    parser_->rules().back()->appendVariable(std::move(var));
  }

  return "";
}

std::any Visitor::visitOperator(Antlr4Gen::SecLangParser::OperatorContext* ctx) {
  // Operator consturctor
  static std::unordered_map<std::string, std::function<std::unique_ptr<Operator::OperatorBase>(
                                             std::string&&, std::string&&)>>
      operator_factory = {
          {"beginsWith",
           [](std::string&& operator_name, std::string&& operator_value) {
             return std::make_unique<Operator::BeginsWith>(std::move(operator_name),
                                                           std::move(operator_value));
           }},
          {"contains",
           [](std::string&& operator_name, std::string&& operator_value) {
             return std::make_unique<Operator::Contains>(std::move(operator_name),
                                                         std::move(operator_value));
           }},
          {"containsWord",
           [](std::string&& operator_name, std::string&& operator_value) {
             return std::make_unique<Operator::ContainsWord>(std::move(operator_name),
                                                             std::move(operator_value));
           }},
          {"detectSQLi",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"detectXSS",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"endsWith",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"fuzzyHash",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"eq", [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"ge", [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"geoLookup",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"gsbLookup",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"gt", [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"inspectFile",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"ipMatch",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"ipMatchF",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"ipMatchFromFile",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"le", [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"lt", [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"noMatch",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"pm", [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"pmf",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"pmFromFile",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"rbl",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"rsub",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"rx",
           [](std::string&& operator_name, std::string&& operator_value) {
             return std::make_unique<Operator::Rx>(std::move(operator_name),
                                                   std::move(operator_value));
           }},
          {"rxGlobal",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"streq",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"strmatch",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"unconditionalMatch",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"validateByteRange",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"validateDTD",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"validateSchema",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"validateUrlEncoding",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"validateUtf8Encoding",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"verifyCC",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"verifyCPF",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"verifySSN",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }},
          {"within",
           [](std::string&& operator_name, std::string&& operator_value) { return nullptr; }}};

  // Construct a operator and sets it into the rule
  std::string operator_name;
  if (ctx->operator_name()) {
    operator_name = ctx->operator_name()->getText();
  } else {
    operator_name = "rx";
  }
  auto iter = operator_factory.find(operator_name);
  if (iter != operator_factory.end()) {
    std::string operator_value = ctx->operator_value()->getText();
    auto op = iter->second(std::move(operator_name), std::move(operator_value));
    parser_->rules().back()->setOperator(std::move(op));
  }

  return "";
}
} // namespace SrSecurity::Antlr4