#include "visitor.h"

#include <unordered_map>

#include <assert.h>

#include "antlr4_gen/SecLangLexer.h"

namespace SrSecurity::Parser {

Antlr4Gen::SecLangLexer seclang_lexer(nullptr);

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
  std::string var_name = ctx->var_name()->getText();
  // auto aa = seclang_lexer.getVocabulary().getSymbolicName(Antlr4Gen::SecLangLexer::VAR_ARGS);
  return "";
}
} // namespace SrSecurity::Parser