#include "visitor.h"

#include <unordered_map>

#include <assert.h>

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
  // Variables consturctor
  static std::unordered_map<std::string,
                            std::function<std::unique_ptr<Variable::VariableBase>(
                                std::string && full_name, bool is_not, bool is_counter)>>
      variable_consturctor_ = {
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

  // Select a constructor to execute
  std::string main_name = ctx->var_main_name()->getText();
  auto iter = variable_consturctor_.find(main_name);
  if (iter != variable_consturctor_.end()) {
    std::string full_name = ctx->getText();
    bool is_not = ctx->NOT() != nullptr;
    bool is_counter = ctx->VAR_COUNT() != nullptr;
    auto var = iter->second(std::move(full_name), is_not, is_counter);
    parser_->rules().back()->appendVariable(std::move(var));
  }

  return "";
}
} // namespace SrSecurity::Antlr4