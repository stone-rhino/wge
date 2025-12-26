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
#include "visitor.h"

#include <format>
#include <unordered_map>

#include "../action/actions_include.h"
#include "../common/log.h"
#include "../common/try.h"
#include "../common/variant.h"
#include "../macro/macro_include.h"
#include "../operator/operator_include.h"
#include "../persistent_storage/storage.h"
#include "../transformation/transform_include.h"
#include "../variable/variables_include.h"

namespace Wge::Antlr4 {

std::any Visitor::visitInclude(Antlr4Gen::SecLangParser::IncludeContext* ctx) {
  std::string_view curr_load_file = parser_->currLoadFile();
  std::string file_path;
  if (!curr_load_file.empty() && !curr_load_file.starts_with('/')) {
    auto pos = curr_load_file.find_last_of('/');
    if (pos != std::string_view::npos) {
      file_path = curr_load_file.substr(0, pos + 1);
    }
  }
  file_path += ctx->STRING()->getText();

  return parser_->loadFromFile(file_path);
}

std::any Visitor::visitSec_reqeust_body_access(
    Antlr4Gen::SecLangParser::Sec_reqeust_body_accessContext* ctx) {
  parser_->secRequestBodyAccess(optionStr2Bool(ctx->OPTION()->getText()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_response_body_mime_type(
    Antlr4Gen::SecLangParser::Sec_response_body_mime_typeContext* ctx) {
  std::string mime_types_str = ctx->MIME_TYPES()->getText();
  std::vector<std::string> mime_types_vec;

  // Split the string by space
  size_t pos = 0;
  while (pos < mime_types_str.size()) {
    size_t next_pos = mime_types_str.find(' ', pos);
    if (next_pos == std::string::npos) {
      mime_types_vec.emplace_back(mime_types_str.substr(pos));
      break;
    } else {
      mime_types_vec.emplace_back(mime_types_str.substr(pos, next_pos - pos));
      pos = next_pos + 1;
    }
  }

  parser_->secResponseBodyMimeType(mime_types_vec);
  return EMPTY_STRING;
}

std::any Visitor::visitSec_response_body_mime_type_clear(
    Antlr4Gen::SecLangParser::Sec_response_body_mime_type_clearContext* ctx) {
  parser_->secResponseBodyMimeTypeClear();
  return EMPTY_STRING;
}

std::any Visitor::visitSec_response_body_access(
    Antlr4Gen::SecLangParser::Sec_response_body_accessContext* ctx) {
  parser_->secResponseBodyAccess(optionStr2Bool(ctx->OPTION()->getText()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_rule_engine(Antlr4Gen::SecLangParser::Sec_rule_engineContext* ctx) {
  parser_->secRuleEngine(optionStr2EnumValue(ctx->OPTION()->getText()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_tmp_save_uploaded_files(
    Antlr4Gen::SecLangParser::Sec_tmp_save_uploaded_filesContext* ctx) {
  parser_->secTmpSaveUploadedFiles(optionStr2Bool(ctx->OPTION()->getText()));
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_upload_file_limit(Antlr4Gen::SecLangParser::Sec_upload_file_limitContext* ctx) {
  parser_->secUploadFileLimit(::atol(ctx->INT()->getText().c_str()));
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_upload_keep_files(Antlr4Gen::SecLangParser::Sec_upload_keep_filesContext* ctx) {
  parser_->secUploadKeepFiles(optionStr2Bool(ctx->OPTION()->getText()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_xml_external_entity(
    Antlr4Gen::SecLangParser::Sec_xml_external_entityContext* ctx) {
  parser_->secXmlExternalEntity(optionStr2Bool(ctx->OPTION()->getText()));
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_request_body_limit(Antlr4Gen::SecLangParser::Sec_request_body_limitContext* ctx) {
  parser_->secRequestBodyLimit(::atoll(ctx->INT()->getText().c_str()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_request_body_no_files_limit(
    Antlr4Gen::SecLangParser::Sec_request_body_no_files_limitContext* ctx) {
  parser_->secRequestBodyNoFilesLimit(::atoll(ctx->INT()->getText().c_str()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_request_body_json_depth_limit(
    Antlr4Gen::SecLangParser::Sec_request_body_json_depth_limitContext* ctx) {
  parser_->secRequestBodyJsonDepthLimit(::atoll(ctx->INT()->getText().c_str()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_request_body_action(
    Antlr4Gen::SecLangParser::Sec_request_body_actionContext* ctx) {
  parser_->secRequsetBodyLimitAction(
      bodyLimitActionStr2EnumValue(ctx->BODY_LIMIT_ACTION()->getText()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_response_body_limit(
    Antlr4Gen::SecLangParser::Sec_response_body_limitContext* ctx) {
  parser_->secResponseBodyLimit(::atoll(ctx->INT()->getText().c_str()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_response_body_action(
    Antlr4Gen::SecLangParser::Sec_response_body_actionContext* ctx) {
  parser_->secResponseBodyLimitAction(
      bodyLimitActionStr2EnumValue(ctx->BODY_LIMIT_ACTION()->getText()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_status_engine(Antlr4Gen::SecLangParser::Sec_status_engineContext* ctx) {
  // Not supported in v3
  WGE_LOG_WARN("SecStatusEngine is not supported yet.");
  return EMPTY_STRING;
}

std::any Visitor::visitSec_tmp_dir(Antlr4Gen::SecLangParser::Sec_tmp_dirContext* ctx) {
  // Not supported in v3
  WGE_LOG_WARN("SecTmpDir is not supported yet.");
  return EMPTY_STRING;
}

std::any Visitor::visitSec_data_dir(Antlr4Gen::SecLangParser::Sec_data_dirContext* ctx) {
  // Not supported in v3
  WGE_LOG_WARN("SecDataDir is not supported yet.");
  return EMPTY_STRING;
}

std::any Visitor::visitSec_cookie_format(Antlr4Gen::SecLangParser::Sec_cookie_formatContext* ctx) {
  // Not supported in v3
  WGE_LOG_WARN("SecCookieFormat is not supported yet.");
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_arguments_limit(Antlr4Gen::SecLangParser::Sec_arguments_limitContext* ctx) {
  parser_->secArgumentsLimit(::atol(ctx->INT()->getText().c_str()));
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_argument_separator(Antlr4Gen::SecLangParser::Sec_argument_separatorContext* ctx) {
  parser_->secArgumentSeparator(ctx->STRING()->getText()[0]);
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_unicode_map_file(Antlr4Gen::SecLangParser::Sec_unicode_map_fileContext* ctx) {
  parser_->secUnicodeMapFile(ctx->STRING()->getText(), ::atoi(ctx->INT()->getText().c_str()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_action(Antlr4Gen::SecLangParser::Sec_actionContext* ctx) {
  // Create an empty rule, and sets actions by visitChildren
  current_rule_ = std::make_unique<CurrentRule>(parser_, ctx->getStart()->getLine(), nullptr);

  // Visit actions
  current_rule_->visitActionMode(CurrentRule::VisitActionMode::SecAction);
  std::string error;
  TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
  if (!error.empty()) {
    // Drop the failed created rule
    current_rule_->finalize(false);
    return error;
  }

  return EMPTY_STRING;
}

std::any
Visitor::visitSec_default_action(Antlr4Gen::SecLangParser::Sec_default_actionContext* ctx) {
  // Create an empty rule, and sets variable and operators and actions by visitChildren
  current_rule_ = std::make_unique<CurrentRule>(parser_, ctx->getStart()->getLine(), nullptr);

  // Visit actions
  std::string error;
  current_rule_->visitActionMode(CurrentRule::VisitActionMode::SecDefaultAction);
  TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
  if (!error.empty()) {
    // Drop the failed created rule
    current_rule_->finalize(false);
    return error;
  }

  return EMPTY_STRING;
}

std::any Visitor::visitSec_parse_xml_into_args(
    Antlr4Gen::SecLangParser::Sec_parse_xml_into_argsContext* ctx) {
  using Option = Wge::ParseXmlIntoArgsOption;
  Option option = Option::Off;

  std::string option_str = ctx->OPTION()->getText();
  if (option_str == "On") {
    option = Option::On;
  } else if (option_str == "OnlyArgs") {
    option = Option::OnlyArgs;
  }

  parser_->secParseXmlIntoArgs(option);
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_pcre_match_limit(Antlr4Gen::SecLangParser::Sec_pcre_match_limitContext* ctx) {
  parser_->secPcreMatchLimit(::atol(ctx->INT()->getText().c_str()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_pcre_match_limit_recursion(
    Antlr4Gen::SecLangParser::Sec_pcre_match_limit_recursionContext* ctx) {
  // Not supported in v3
  WGE_LOG_WARN("SecPcreMatchLimitRecursion is not supported yet.");
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_collection_timeout(Antlr4Gen::SecLangParser::Sec_collection_timeoutContext* ctx) {
  // Not supported in v3
  WGE_LOG_WARN("SecCollectionTimeout is not supported yet.");
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_pmf_serialize_dir(Antlr4Gen::SecLangParser::Sec_pmf_serialize_dirContext* ctx) {
  parser_->secPmfSerializeDir(ctx->STRING()->getText());
  return EMPTY_STRING;
}

std::any Visitor::visitSec_rule(Antlr4Gen::SecLangParser::Sec_ruleContext* ctx) {
  // Create an empty rule, and sets variable and operators and actions by visitChildren
  if (chain_) {
    assert(current_rule_);
    RulePhaseType parent_rule_phase = current_rule_->get()->phase();
    Rule* appended_rule = current_rule_->finalize(true);
    assert(appended_rule);
    current_rule_ =
        std::make_unique<CurrentRule>(parser_, ctx->getStart()->getLine(), appended_rule);
  } else {
    current_rule_ = std::make_unique<CurrentRule>(parser_, ctx->getStart()->getLine(), nullptr);

    // Clear alias for new rule
    alias_.clear();
  }

  chain_ = false;

  // Visit variables and operators and actions
  std::string error;
  current_rule_->visitVariableMode(CurrentRule::VisitVariableMode::SecRule);
  current_rule_->visitActionMode(CurrentRule::VisitActionMode::SecRule);
  TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
  if (!error.empty()) {
    // Drop the failed created rule
    current_rule_->finalize(false);
    return error;
  }

  return EMPTY_STRING;
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

  return EMPTY_STRING;
}

std::any
Visitor::visitSec_rule_remove_by_msg(Antlr4Gen::SecLangParser::Sec_rule_remove_by_msgContext* ctx) {
  parser_->secRuleRemoveByMsg(ctx->STRING()->getText());
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_rule_remove_by_tag(Antlr4Gen::SecLangParser::Sec_rule_remove_by_tagContext* ctx) {
  parser_->secRuleRemoveByTag(ctx->STRING()->getText());
  return EMPTY_STRING;
}

std::any Visitor::visitSec_rule_update_action_by_id(
    Antlr4Gen::SecLangParser::Sec_rule_update_action_by_idContext* ctx) {
  uint64_t id = 0;
  uint64_t chain_index = 0;
  if (ctx->ID_AND_CHAIN_INDEX()) {
    std::string id_and_chain_str = ctx->ID_AND_CHAIN_INDEX()->getText();
    auto pos = id_and_chain_str.find(':');
    if (pos != std::string::npos) {
      id = ::atoll(id_and_chain_str.substr(0, pos).c_str());
      chain_index = ::atoll(id_and_chain_str.substr(pos + 1).c_str());
      current_rule_ = std::make_unique<CurrentRule>(parser_, id);
      if (current_rule_->get()) {
        // If the chain index is out of range, return itself
        Rule* chain_rule = current_rule_->get()->chainRule(chain_index);
        if (chain_rule) {
          current_rule_ = std::make_unique<CurrentRule>(parser_, chain_rule);
        }
      }
    }
  } else {
    id = ::atoll(ctx->INT()->getText().c_str());
    current_rule_ = std::make_unique<CurrentRule>(parser_, id);
  }

  if (current_rule_->get()) {
    // Visit actions
    current_rule_->visitActionMode(CurrentRule::VisitActionMode::SecRuleUpdateAction);
    std::string error;
    TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
    if (!error.empty()) {
      return error;
    }
  }

  return EMPTY_STRING;
}

std::any Visitor::visitSec_rule_update_target_by_id(
    Antlr4Gen::SecLangParser::Sec_rule_update_target_by_idContext* ctx) {
  uint64_t id = ::atoll(ctx->INT()->getText().c_str());
  current_rule_ = std::make_unique<CurrentRule>(parser_, id);

  if (current_rule_->get()) {
    // Visit variables
    std::string error;
    TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
    if (!error.empty()) {
      return error;
    }
  }

  return EMPTY_STRING;
}

std::any Visitor::visitSec_rule_update_target_by_msg(
    Antlr4Gen::SecLangParser::Sec_rule_update_target_by_msgContext* ctx) {
  auto rules = parser_->findRuleByMsg(ctx->STRING()->getText());
  for (auto rule : rules) {
    current_rule_ = std::make_unique<CurrentRule>(parser_, rule);

    // Visit variables
    std::string error;
    TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
    if (!error.empty()) {
      return error;
    }
  }

  return EMPTY_STRING;
}

std::any Visitor::visitSec_rule_update_target_by_tag(
    Antlr4Gen::SecLangParser::Sec_rule_update_target_by_tagContext* ctx) {
  auto rules = parser_->findRuleByTag(ctx->STRING()->getText());
  for (auto rule : rules) {
    current_rule_ = std::make_unique<CurrentRule>(parser_, rule);
    // Visit variables
    std::string error;
    TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
    if (!error.empty()) {
      return error;
    }
  }

  return EMPTY_STRING;
}

std::any Visitor::visitSec_marker(Antlr4Gen::SecLangParser::Sec_markerContext* ctx) {
  if (current_rule_) {
    current_rule_->finalize(true);
  }

  parser_->secMarker(ctx->STRING()->getText());
  return EMPTY_STRING;
}

std::any Visitor::visitVariable_args(Antlr4Gen::SecLangParser::Variable_argsContext* ctx) {
  return appendVariable<Variable::Args>(ctx);
};

std::any Visitor::visitVariable_args_combined_size(
    Antlr4Gen::SecLangParser::Variable_args_combined_sizeContext* ctx) {
  return appendVariable<Variable::ArgsCombinedSize>(ctx);
};

std::any Visitor::visitVariable_args_get(Antlr4Gen::SecLangParser::Variable_args_getContext* ctx) {
  return appendVariable<Variable::ArgsGet>(ctx);
};

std::any Visitor::visitVariable_args_get_names(
    Antlr4Gen::SecLangParser::Variable_args_get_namesContext* ctx) {
  return appendVariable<Variable::ArgsGetNames>(ctx);
};

std::any
Visitor::visitVariable_args_names(Antlr4Gen::SecLangParser::Variable_args_namesContext* ctx) {
  return appendVariable<Variable::ArgsNames>(ctx);
};

std::any
Visitor::visitVariable_args_post(Antlr4Gen::SecLangParser::Variable_args_postContext* ctx) {
  return appendVariable<Variable::ArgsPost>(ctx);
};

std::any Visitor::visitVariable_args_post_names(
    Antlr4Gen::SecLangParser::Variable_args_post_namesContext* ctx) {
  return appendVariable<Variable::ArgsPostNames>(ctx);
};

std::any
Visitor::visitVariable_auth_type(Antlr4Gen::SecLangParser::Variable_auth_typeContext* ctx) {
  return appendVariable<Variable::AuthType>(ctx);
};

std::any Visitor::visitVariable_duration(Antlr4Gen::SecLangParser::Variable_durationContext* ctx) {
  return appendVariable<Variable::Duration>(ctx);
};

std::any Visitor::visitVariable_env(Antlr4Gen::SecLangParser::Variable_envContext* ctx) {
  return appendVariable<Variable::Env>(ctx);
};

std::any Visitor::visitVariable_files(Antlr4Gen::SecLangParser::Variable_filesContext* ctx) {
  return appendVariable<Variable::Files>(ctx);
};

std::any Visitor::visitVariable_files_combined_size(
    Antlr4Gen::SecLangParser::Variable_files_combined_sizeContext* ctx) {
  return appendVariable<Variable::FilesCombinedSize>(ctx);
};

std::any
Visitor::visitVariable_files_names(Antlr4Gen::SecLangParser::Variable_files_namesContext* ctx) {
  return appendVariable<Variable::FilesNames>(ctx);
};

std::any
Visitor::visitVariable_full_request(Antlr4Gen::SecLangParser::Variable_full_requestContext* ctx) {
  return appendVariable<Variable::FullRequest>(ctx);
};

std::any Visitor::visitVariable_full_request_length(
    Antlr4Gen::SecLangParser::Variable_full_request_lengthContext* ctx) {
  return appendVariable<Variable::FullRequestLength>(ctx);
};

std::any
Visitor::visitVariable_files_sizes(Antlr4Gen::SecLangParser::Variable_files_sizesContext* ctx) {
  return appendVariable<Variable::FilesSizes>(ctx);
};

std::any Visitor::visitVariable_files_tmpnames(
    Antlr4Gen::SecLangParser::Variable_files_tmpnamesContext* ctx) {
  return appendVariable<Variable::FilesTmpNames>(ctx);
};

std::any Visitor::visitVariable_files_tmp_content(
    Antlr4Gen::SecLangParser::Variable_files_tmp_contentContext* ctx) {
  return appendVariable<Variable::FilesTmpContent>(ctx);
};

std::any Visitor::visitVariable_geo(Antlr4Gen::SecLangParser::Variable_geoContext* ctx) {
  return appendVariable<Variable::Geo>(ctx);
};

std::any Visitor::visitVariable_highest_severity(
    Antlr4Gen::SecLangParser::Variable_highest_severityContext* ctx) {
  return appendVariable<Variable::HighestSeverity>(ctx);
};

std::any Visitor::visitVariable_inbound_data_error(
    Antlr4Gen::SecLangParser::Variable_inbound_data_errorContext* ctx) {
  return appendVariable<Variable::InboundDataError>(ctx);
};

std::any
Visitor::visitVariable_matched_var(Antlr4Gen::SecLangParser::Variable_matched_varContext* ctx) {
  return appendVariable<Variable::MatchedVar>(ctx);
};

std::any
Visitor::visitVariable_matched_vars(Antlr4Gen::SecLangParser::Variable_matched_varsContext* ctx) {
  return appendVariable<Variable::MatchedVars>(ctx);
};

std::any Visitor::visitVariable_matched_var_name(
    Antlr4Gen::SecLangParser::Variable_matched_var_nameContext* ctx) {
  return appendVariable<Variable::MatchedVarName>(ctx);
};

std::any Visitor::visitVariable_matched_vars_names(
    Antlr4Gen::SecLangParser::Variable_matched_vars_namesContext* ctx) {
  return appendVariable<Variable::MatchedVarsNames>(ctx);
};

std::any
Visitor::visitVariable_modsec_build(Antlr4Gen::SecLangParser::Variable_modsec_buildContext* ctx) {
  return appendVariable<Variable::ModSecBuild>(ctx);
};

std::any Visitor::visitVariable_msc_pcre_limits_exceeded(
    Antlr4Gen::SecLangParser::Variable_msc_pcre_limits_exceededContext* ctx) {
  return appendVariable<Variable::MscPcreLimitsExceeded>(ctx);
};

std::any Visitor::visitVariable_multipart_crlf_lf_lines(
    Antlr4Gen::SecLangParser::Variable_multipart_crlf_lf_linesContext* ctx) {
  return appendVariable<Variable::MultipartCrlfLfLines>(ctx);
};

std::any Visitor::visitVariable_multipart_filename(
    Antlr4Gen::SecLangParser::Variable_multipart_filenameContext* ctx) {
  return appendVariable<Variable::MultipartFileName>(ctx);
};

std::any Visitor::visitVariable_multipart_name(
    Antlr4Gen::SecLangParser::Variable_multipart_nameContext* ctx) {
  return appendVariable<Variable::MultipartName>(ctx);
};

std::any Visitor::visitVariable_multipart_part_headers(
    Antlr4Gen::SecLangParser::Variable_multipart_part_headersContext* ctx) {
  return appendVariable<Variable::MultipartPartHeaders>(ctx);
};

std::any Visitor::visitVariable_multipart_strict_error(
    Antlr4Gen::SecLangParser::Variable_multipart_strict_errorContext* ctx) {
  return appendVariable<Variable::MultipartStrictError>(ctx);
};

std::any Visitor::visitVariable_multipart_unmatched_boundary(
    Antlr4Gen::SecLangParser::Variable_multipart_unmatched_boundaryContext* ctx) {
  return appendVariable<Variable::MultipartUnmatchedBoundary>(ctx);
};

std::any Visitor::visitVariable_outbound_data_error(
    Antlr4Gen::SecLangParser::Variable_outbound_data_errorContext* ctx) {
  return appendVariable<Variable::OutboundDataError>(ctx);
};

std::any
Visitor::visitVariable_path_info(Antlr4Gen::SecLangParser::Variable_path_infoContext* ctx) {
  return appendVariable<Variable::PathInfo>(ctx);
};

std::any
Visitor::visitVariable_query_string(Antlr4Gen::SecLangParser::Variable_query_stringContext* ctx) {
  return appendVariable<Variable::QueryString>(ctx);
};

std::any
Visitor::visitVariable_remote_addr(Antlr4Gen::SecLangParser::Variable_remote_addrContext* ctx) {
  return appendVariable<Variable::RemoteAddr>(ctx);
};

std::any
Visitor::visitVariable_remote_host(Antlr4Gen::SecLangParser::Variable_remote_hostContext* ctx) {
  return appendVariable<Variable::RemoteHost>(ctx);
};

std::any
Visitor::visitVariable_remote_port(Antlr4Gen::SecLangParser::Variable_remote_portContext* ctx) {
  return appendVariable<Variable::RemotePort>(ctx);
};

std::any
Visitor::visitVariable_remote_user(Antlr4Gen::SecLangParser::Variable_remote_userContext* ctx) {
  return appendVariable<Variable::RemoteUser>(ctx);
};

std::any
Visitor::visitVariable_reqbody_error(Antlr4Gen::SecLangParser::Variable_reqbody_errorContext* ctx) {
  return appendVariable<Variable::ReqBodyError>(ctx);
};

std::any Visitor::visitVariable_reqbody_error_msg(
    Antlr4Gen::SecLangParser::Variable_reqbody_error_msgContext* ctx) {
  return appendVariable<Variable::ReqBodyErrorMsg>(ctx);
};

std::any Visitor::visitVariable_reqbody_processor(
    Antlr4Gen::SecLangParser::Variable_reqbody_processorContext* ctx) {
  return appendVariable<Variable::ReqBodyProcessor>(ctx);
};

std::any Visitor::visitVariable_request_basename(
    Antlr4Gen::SecLangParser::Variable_request_basenameContext* ctx) {
  return appendVariable<Variable::RequestBaseName>(ctx);
};

std::any
Visitor::visitVariable_request_body(Antlr4Gen::SecLangParser::Variable_request_bodyContext* ctx) {
  return appendVariable<Variable::RequestBody>(ctx);
};

std::any Visitor::visitVariable_request_body_length(
    Antlr4Gen::SecLangParser::Variable_request_body_lengthContext* ctx) {
  return appendVariable<Variable::RequestBodyLength>(ctx);
};

std::any Visitor::visitVariable_request_cookies(
    Antlr4Gen::SecLangParser::Variable_request_cookiesContext* ctx) {
  return appendVariable<Variable::RequestCookies>(ctx);
};

std::any Visitor::visitVariable_request_cookies_names(
    Antlr4Gen::SecLangParser::Variable_request_cookies_namesContext* ctx) {
  return appendVariable<Variable::RequestCookiesNames>(ctx);
};

std::any Visitor::visitVariable_request_filename(
    Antlr4Gen::SecLangParser::Variable_request_filenameContext* ctx) {
  return appendVariable<Variable::RequestFileName>(ctx);
};

std::any Visitor::visitVariable_request_headers(
    Antlr4Gen::SecLangParser::Variable_request_headersContext* ctx) {
  return appendVariable<Variable::RequestHeaders>(ctx);
};

std::any Visitor::visitVariable_request_headers_names(
    Antlr4Gen::SecLangParser::Variable_request_headers_namesContext* ctx) {
  return appendVariable<Variable::RequestHeadersNames>(ctx);
};

std::any
Visitor::visitVariable_request_line(Antlr4Gen::SecLangParser::Variable_request_lineContext* ctx) {
  return appendVariable<Variable::RequestLine>(ctx);
};

std::any Visitor::visitVariable_request_method(
    Antlr4Gen::SecLangParser::Variable_request_methodContext* ctx) {
  return appendVariable<Variable::RequestMothod>(ctx);
};

std::any Visitor::visitVariable_request_protocol(
    Antlr4Gen::SecLangParser::Variable_request_protocolContext* ctx) {
  return appendVariable<Variable::RequestProtocol>(ctx);
};

std::any
Visitor::visitVariable_request_uri(Antlr4Gen::SecLangParser::Variable_request_uriContext* ctx) {
  return appendVariable<Variable::RequestUri>(ctx);
};

std::any Visitor::visitVariable_request_uri_raw(
    Antlr4Gen::SecLangParser::Variable_request_uri_rawContext* ctx) {
  return appendVariable<Variable::RequestUriRaw>(ctx);
};

std::any
Visitor::visitVariable_response_body(Antlr4Gen::SecLangParser::Variable_response_bodyContext* ctx) {
  return appendVariable<Variable::ResponseBody>(ctx);
};

std::any Visitor::visitVariable_response_content_length(
    Antlr4Gen::SecLangParser::Variable_response_content_lengthContext* ctx) {
  return appendVariable<Variable::ResponseContentLength>(ctx);
};

std::any Visitor::visitVariable_response_content_type(
    Antlr4Gen::SecLangParser::Variable_response_content_typeContext* ctx) {
  return appendVariable<Variable::ResponseContentType>(ctx);
};

std::any Visitor::visitVariable_response_headers(
    Antlr4Gen::SecLangParser::Variable_response_headersContext* ctx) {
  return appendVariable<Variable::ResponseHeaders>(ctx);
};

std::any Visitor::visitVariable_response_headers_names(
    Antlr4Gen::SecLangParser::Variable_response_headers_namesContext* ctx) {
  return appendVariable<Variable::ResponseHeadersNames>(ctx);
};

std::any Visitor::visitVariable_response_protocol(
    Antlr4Gen::SecLangParser::Variable_response_protocolContext* ctx) {
  return appendVariable<Variable::ResponseProtocol>(ctx);
};

std::any Visitor::visitVariable_response_status(
    Antlr4Gen::SecLangParser::Variable_response_statusContext* ctx) {
  return appendVariable<Variable::ResponseStatus>(ctx);
};

std::any Visitor::visitVariable_rule(Antlr4Gen::SecLangParser::Variable_ruleContext* ctx) {
  return appendVariable<Variable::Rule>(ctx);
};

std::any
Visitor::visitVariable_server_addr(Antlr4Gen::SecLangParser::Variable_server_addrContext* ctx) {
  return appendVariable<Variable::ServerAddr>(ctx);
};

std::any
Visitor::visitVariable_server_name(Antlr4Gen::SecLangParser::Variable_server_nameContext* ctx) {
  return appendVariable<Variable::ServerName>(ctx);
};

std::any
Visitor::visitVariable_server_port(Antlr4Gen::SecLangParser::Variable_server_portContext* ctx) {
  return appendVariable<Variable::ServerPort>(ctx);
};

std::any Visitor::visitVariable_session(Antlr4Gen::SecLangParser::Variable_sessionContext* ctx) {
  return appendVariable<Variable::Session>(ctx);
};

std::any
Visitor::visitVariable_sessionid(Antlr4Gen::SecLangParser::Variable_sessionidContext* ctx) {
  return appendVariable<Variable::SessionId>(ctx);
};

std::any
Visitor::visitVariable_status_line(Antlr4Gen::SecLangParser::Variable_status_lineContext* ctx) {
  return appendVariable<Variable::StatusLine>(ctx);
};

std::any Visitor::visitVariable_time(Antlr4Gen::SecLangParser::Variable_timeContext* ctx) {
  return appendVariable<Variable::Time>(ctx);
};

std::any Visitor::visitVariable_time_day(Antlr4Gen::SecLangParser::Variable_time_dayContext* ctx) {
  return appendVariable<Variable::TimeDay>(ctx);
};

std::any
Visitor::visitVariable_time_epoch(Antlr4Gen::SecLangParser::Variable_time_epochContext* ctx) {
  return appendVariable<Variable::TimeEpoch>(ctx);
};

std::any
Visitor::visitVariable_time_hour(Antlr4Gen::SecLangParser::Variable_time_hourContext* ctx) {
  return appendVariable<Variable::TimeHour>(ctx);
};

std::any Visitor::visitVariable_time_min(Antlr4Gen::SecLangParser::Variable_time_minContext* ctx) {
  return appendVariable<Variable::TimeMin>(ctx);
};

std::any Visitor::visitVariable_time_mon(Antlr4Gen::SecLangParser::Variable_time_monContext* ctx) {
  return appendVariable<Variable::TimeMon>(ctx);
};

std::any Visitor::visitVariable_time_sec(Antlr4Gen::SecLangParser::Variable_time_secContext* ctx) {
  return appendVariable<Variable::TimeSec>(ctx);
};

std::any
Visitor::visitVariable_time_wday(Antlr4Gen::SecLangParser::Variable_time_wdayContext* ctx) {
  return appendVariable<Variable::TimeWDay>(ctx);
};

std::any
Visitor::visitVariable_time_year(Antlr4Gen::SecLangParser::Variable_time_yearContext* ctx) {
  return appendVariable<Variable::TimeYear>(ctx);
};

std::any Visitor::visitVariable_tx(Antlr4Gen::SecLangParser::Variable_txContext* ctx) {
  return appendTxVariable(ctx, parser_->getCurrentNamespace());
};

std::any
Visitor::visitVariable_unique_id(Antlr4Gen::SecLangParser::Variable_unique_idContext* ctx) {
  return appendVariable<Variable::UniqueId>(ctx);
};

std::any Visitor::visitVariable_urlencoded_error(
    Antlr4Gen::SecLangParser::Variable_urlencoded_errorContext* ctx) {
  return appendVariable<Variable::UrlenCodedError>(ctx);
};

std::any Visitor::visitVariable_userid(Antlr4Gen::SecLangParser::Variable_useridContext* ctx) {
  return appendVariable<Variable::UserId>(ctx);
};

std::any Visitor::visitVariable_webappid(Antlr4Gen::SecLangParser::Variable_webappidContext* ctx) {
  return appendVariable<Variable::WebAppId>(ctx);
};

std::any Visitor::visitVariable_xml(Antlr4Gen::SecLangParser::Variable_xmlContext* ctx) {
  return appendVariable<Variable::Xml>(ctx);
};

std::any Visitor::visitVariable_reqbody_processor_error(
    Antlr4Gen::SecLangParser::Variable_reqbody_processor_errorContext* ctx) {
  return appendVariable<Variable::ReqbodyProcessorError>(ctx);
}

std::any Visitor::visitVariable_multipart_boundary_quoted(
    Antlr4Gen::SecLangParser::Variable_multipart_boundary_quotedContext* ctx) {
  return appendVariable<Variable::MultipartBoundaryQuoted>(ctx);
}

std::any Visitor::visitVariable_multipart_boundary_whitespace(
    Antlr4Gen::SecLangParser::Variable_multipart_boundary_whitespaceContext* ctx) {
  return appendVariable<Variable::MultipartBoundaryWhitespace>(ctx);
}

std::any Visitor::visitVariable_multipart_data_before(
    Antlr4Gen::SecLangParser::Variable_multipart_data_beforeContext* ctx) {
  return appendVariable<Variable::MultipartDataBefore>(ctx);
}

std::any Visitor::visitVariable_multipart_data_after(
    Antlr4Gen::SecLangParser::Variable_multipart_data_afterContext* ctx) {
  return appendVariable<Variable::MultipartDataAfter>(ctx);
}

std::any Visitor::visitVariable_multipart_header_folding(
    Antlr4Gen::SecLangParser::Variable_multipart_header_foldingContext* ctx) {
  return appendVariable<Variable::MultipartHeaderFolding>(ctx);
}

std::any Visitor::visitVariable_multipart_lf_line(
    Antlr4Gen::SecLangParser::Variable_multipart_lf_lineContext* ctx) {
  return appendVariable<Variable::MultipartLfLine>(ctx);
}

std::any Visitor::visitVariable_multipart_missing_semicolon(
    Antlr4Gen::SecLangParser::Variable_multipart_missing_semicolonContext* ctx) {
  return appendVariable<Variable::MultipartMissingSemicolon>(ctx);
}

std::any Visitor::visitVariable_multipart_invalid_quoting(
    Antlr4Gen::SecLangParser::Variable_multipart_invalid_quotingContext* ctx) {
  return appendVariable<Variable::MultipartInvalidQuoting>(ctx);
}

std::any Visitor::visitVariable_multipart_invalid_part(
    Antlr4Gen::SecLangParser::Variable_multipart_invalid_partContext* ctx) {
  return appendVariable<Variable::MultipartInvalidPart>(ctx);
}

std::any Visitor::visitVariable_multipart_invalid_header_folding(
    Antlr4Gen::SecLangParser::Variable_multipart_invalid_header_foldingContext* ctx) {
  return appendVariable<Variable::MultipartInvalidHeaderFolding>(ctx);
}

std::any Visitor::visitVariable_multipart_file_limit_exceeded(
    Antlr4Gen::SecLangParser::Variable_multipart_file_limit_exceededContext* ctx) {
  return appendVariable<Variable::MultipartFileLimitExceeded>(ctx);
}

std::any Visitor::visitVariable_global(Antlr4Gen::SecLangParser::Variable_globalContext* ctx) {
  return appendVariable<Variable::Global>(ctx);
}

std::any Visitor::visitVariable_resource(Antlr4Gen::SecLangParser::Variable_resourceContext* ctx) {
  return appendVariable<Variable::Resource>(ctx);
}

std::any Visitor::visitVariable_ip(Antlr4Gen::SecLangParser::Variable_ipContext* ctx) {
  return appendVariable<Variable::Ip>(ctx);
}

std::any Visitor::visitVariable_user(Antlr4Gen::SecLangParser::Variable_userContext* ctx) {
  return appendVariable<Variable::User>(ctx);
}

std::any Visitor::visitVariable_ptree(Antlr4Gen::SecLangParser::Variable_ptreeContext* ctx) {
  return appendVariable<Variable::PTree>(ctx);
}

std::any Visitor::visitVariable_gtx(Antlr4Gen::SecLangParser::Variable_gtxContext* ctx) {
  return appendTxVariable(ctx, "");
}

std::any Visitor::visitVariable_matched_vptree(
    Antlr4Gen::SecLangParser::Variable_matched_vptreeContext* ctx) {
  return appendVariable<Variable::MatchedVPTree>(ctx);
}

std::any Visitor::visitVariable_matched_optree(
    Antlr4Gen::SecLangParser::Variable_matched_optreeContext* ctx) {
  return appendVariable<Variable::MatchedOPTree>(ctx);
}

std::any Visitor::visitVariable_alias(Antlr4Gen::SecLangParser::Variable_aliasContext* ctx) {
  std::string alias_name = ctx->VAR_ALIAS()->getText();
  auto iter = alias_.find(alias_name);
  if (iter == alias_.end()) {
    RETURN_ERROR("Alias '" + alias_name + "' is not defined.");
  }

  // Extract sub_name from alias value (remove "matched_optree" and "matched_vptree" prefix)
  std::string sub_name = iter->second.substr(14);
  if (ctx->variable_ptree_expression()) {
    if (!sub_name.empty() && sub_name.back() != '/') {
      sub_name += "." + ctx->variable_ptree_expression()->getText();
    } else {
      sub_name = ctx->variable_ptree_expression()->getText();
    }
  }

  if (iter->second.starts_with("matched_optree")) {
    return appendAliasVariable<Variable::MatchedOPTree>(ctx, std::move(sub_name));
  } else if (iter->second.starts_with("matched_vptree")) {
    return appendAliasVariable<Variable::MatchedVPTree>(ctx, std::move(sub_name));
  } else {
    RETURN_ERROR("Alias '" + alias_name +
                 "' is not a valid variable alias for matched optree or vptree.");
  }

  return EMPTY_STRING;
}

std::any Visitor::visitOp_begins_with(Antlr4Gen::SecLangParser::Op_begins_withContext* ctx) {
  return appendOperator<Operator::BeginsWith>(ctx);
}

std::any Visitor::visitOp_contains(Antlr4Gen::SecLangParser::Op_containsContext* ctx) {
  return appendOperator<Operator::Contains>(ctx);
}

std::any Visitor::visitOp_contains_word(Antlr4Gen::SecLangParser::Op_contains_wordContext* ctx) {
  return appendOperator<Operator::ContainsWord>(ctx);
}

std::any Visitor::visitOp_detect_sqli(Antlr4Gen::SecLangParser::Op_detect_sqliContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op = std::make_unique<Operator::DetectSqli>(
      std::string(), ctx->NOT() != nullptr, parser_->currLoadFile());
  current_rule_->get()->appendOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_detect_xss(Antlr4Gen::SecLangParser::Op_detect_xssContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op = std::make_unique<Operator::DetectXSS>(
      std::string(), ctx->NOT() != nullptr, parser_->currLoadFile());
  current_rule_->get()->appendOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_ends_with(Antlr4Gen::SecLangParser::Op_ends_withContext* ctx) {
  return appendOperator<Operator::EndsWith>(ctx);
}

std::any Visitor::visitOp_fuzzy_hash(Antlr4Gen::SecLangParser::Op_fuzzy_hashContext* ctx) {
  return appendOperator<Operator::FuzzyHash>(ctx);
}

std::any Visitor::visitOp_eq(Antlr4Gen::SecLangParser::Op_eqContext* ctx) {
  return appendOperator<Operator::Eq>(ctx);
}

std::any Visitor::visitOp_ge(Antlr4Gen::SecLangParser::Op_geContext* ctx) {
  return appendOperator<Operator::Ge>(ctx);
}

std::any Visitor::visitOp_geo_lookup(Antlr4Gen::SecLangParser::Op_geo_lookupContext* ctx) {
  return appendOperator<Operator::GeoLookup>(ctx);
}

std::any Visitor::visitOp_gt(Antlr4Gen::SecLangParser::Op_gtContext* ctx) {
  return appendOperator<Operator::Gt>(ctx);
}

std::any Visitor::visitOp_inspect_file(Antlr4Gen::SecLangParser::Op_inspect_fileContext* ctx) {
  return appendOperator<Operator::InspectFile>(ctx);
}

std::any Visitor::visitOp_ip_match(Antlr4Gen::SecLangParser::Op_ip_matchContext* ctx) {
  return appendOperator<Operator::IpMatch>(ctx);
}

std::any Visitor::visitOp_ip_match_f(Antlr4Gen::SecLangParser::Op_ip_match_fContext* ctx) {
  return appendOperator<Operator::IpMatchFromFile>(ctx);
}

std::any
Visitor::visitOp_ip_match_from_file(Antlr4Gen::SecLangParser::Op_ip_match_from_fileContext* ctx) {
  return appendOperator<Operator::IpMatchFromFile>(ctx);
}

std::any Visitor::visitOp_le(Antlr4Gen::SecLangParser::Op_leContext* ctx) {
  return appendOperator<Operator::Le>(ctx);
}

std::any Visitor::visitOp_lt(Antlr4Gen::SecLangParser::Op_ltContext* ctx) {
  return appendOperator<Operator::Lt>(ctx);
}

std::any Visitor::visitOp_no_match(Antlr4Gen::SecLangParser::Op_no_matchContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op = std::make_unique<Operator::NoMatch>(
      std::string(), ctx->NOT() != nullptr, parser_->currLoadFile());
  current_rule_->get()->appendOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_pm(Antlr4Gen::SecLangParser::Op_pmContext* ctx) {
  return appendOperator<Operator::Pm>(ctx);
}

std::any Visitor::visitOp_pmf(Antlr4Gen::SecLangParser::Op_pmfContext* ctx) {
  return appendOperator<Operator::PmFromFile>(ctx);
}

std::any Visitor::visitOp_pm_from_file(Antlr4Gen::SecLangParser::Op_pm_from_fileContext* ctx) {
  return appendOperator<Operator::PmFromFile>(ctx);
}

std::any Visitor::visitOp_rbl(Antlr4Gen::SecLangParser::Op_rblContext* ctx) {
  return appendOperator<Operator::Rbl>(ctx);
}

std::any Visitor::visitOp_rsub(Antlr4Gen::SecLangParser::Op_rsubContext* ctx) {
  return appendOperator<Operator::Rsub>(ctx);
}

std::any Visitor::visitOp_rx(Antlr4Gen::SecLangParser::Op_rxContext* ctx) {
  return appendOperator<Operator::Rx>(ctx);
}

std::any Visitor::visitOp_rx_global(Antlr4Gen::SecLangParser::Op_rx_globalContext* ctx) {
  return appendOperator<Operator::RxGlobal>(ctx);
}

std::any Visitor::visitOp_streq(Antlr4Gen::SecLangParser::Op_streqContext* ctx) {
  return appendOperator<Operator::Streq>(ctx);
}

std::any Visitor::visitOp_strmatch(Antlr4Gen::SecLangParser::Op_strmatchContext* ctx) {
  return appendOperator<Operator::Strmatch>(ctx);
}

std::any
Visitor::visitOp_unconditional_match(Antlr4Gen::SecLangParser::Op_unconditional_matchContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op = std::make_unique<Operator::UnconditionalMatch>(
      std::string(), ctx->NOT() != nullptr, parser_->currLoadFile());
  current_rule_->get()->appendOperator(std::move(op));
  return EMPTY_STRING;
}

std::any
Visitor::visitOp_validate_byte_range(Antlr4Gen::SecLangParser::Op_validate_byte_rangeContext* ctx) {
  return appendOperator<Operator::ValidateByteRange>(ctx);
}

std::any Visitor::visitOp_validate_dtd(Antlr4Gen::SecLangParser::Op_validate_dtdContext* ctx) {
  return appendOperator<Operator::ValidateDTD>(ctx);
}

std::any
Visitor::visitOp_validate_schema(Antlr4Gen::SecLangParser::Op_validate_schemaContext* ctx) {
  return appendOperator<Operator::ValidateSchema>(ctx);
}

std::any Visitor::visitOp_validate_url_encoding(
    Antlr4Gen::SecLangParser::Op_validate_url_encodingContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op = std::make_unique<Operator::ValidateUrlEncoding>(
      std::string(), ctx->NOT() != nullptr, parser_->currLoadFile());
  current_rule_->get()->appendOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_validate_utf8_encoding(
    Antlr4Gen::SecLangParser::Op_validate_utf8_encodingContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op = std::make_unique<Operator::ValidateUtf8Encoding>(
      std::string(), ctx->NOT() != nullptr, parser_->currLoadFile());
  current_rule_->get()->appendOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_verify_cc(Antlr4Gen::SecLangParser::Op_verify_ccContext* ctx) {
  return appendOperator<Operator::VerifyCC>(ctx);
}

std::any Visitor::visitOp_verify_cpf(Antlr4Gen::SecLangParser::Op_verify_cpfContext* ctx) {
  return appendOperator<Operator::VerifyCPF>(ctx);
}

std::any Visitor::visitOp_verify_ssn(Antlr4Gen::SecLangParser::Op_verify_ssnContext* ctx) {
  return appendOperator<Operator::VerifySSN>(ctx);
}

std::any Visitor::visitOp_within(Antlr4Gen::SecLangParser::Op_withinContext* ctx) {
  return appendOperator<Operator::Within>(ctx);
}

std::any Visitor::visitOp_rx_default(Antlr4Gen::SecLangParser::Op_rx_defaultContext* ctx) {
  auto macro = getMacro(ctx->string_with_macro()->getText(), ctx->string_with_macro()->variable(),
                        ctx->string_with_macro()->STRING().empty());

  if (!macro.has_value()) {
    RETURN_ERROR(macro.error());
  }

  std::unique_ptr<Operator::OperatorBase> op;
  if (macro.value()) {
    op = std::unique_ptr<Operator::OperatorBase>(
        new Operator::Rx(std::move(macro.value()), false, parser_->currLoadFile()));
  } else {
    op = std::unique_ptr<Operator::OperatorBase>(
        new Operator::Rx(ctx->string_with_macro()->getText(), false, parser_->currLoadFile()));
  }

  current_rule_->get()->appendOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_xor(Antlr4Gen::SecLangParser::Op_xorContext* ctx) {
  return appendOperator<Operator::Xor>(ctx);
}

std::any
Visitor::visitAction_meta_data_id(Antlr4Gen::SecLangParser::Action_meta_data_idContext* ctx) {
  // The SecRuleUpdateAction cannot be used to change the id of a rule
  if (current_rule_->visitActionMode() == CurrentRule::VisitActionMode::SecRuleUpdateAction) {
    return EMPTY_STRING;
  }

  uint64_t id = 0;
  if (ctx->INT()) {
    id = ::atoll(ctx->INT()->getText().c_str());
  } else {
    std::string id_str = ctx->STRING()->getText();
    if (std::all_of(id_str.begin(), id_str.end(), ::isdigit)) {
      id = ::atoll(id_str.c_str());
    } else {
      should_visit_next_child_ = false;
      return std::string("id must be a number");
    }
  }

  // Ensure the id is unique
  auto rule = parser_->findRuleById(id);
  if (rule) {
    // If the rule already exists, we cannot set the id again
    should_visit_next_child_ = false;
    return std::string("Rule with id " + std::to_string(id) + " already exists");
  }

  current_rule_->get()->id(id);

  return EMPTY_STRING;
};

std::any
Visitor::visitAction_meta_data_phase(Antlr4Gen::SecLangParser::Action_meta_data_phaseContext* ctx) {
  // The SecRuleUpdateAction cannot be used to change the phase of a rule
  if (current_rule_->visitActionMode() == CurrentRule::VisitActionMode::SecRuleUpdateAction) {
    return EMPTY_STRING;
  }

  current_rule_->get()->phase(::atoll(ctx->INT()->getText().c_str()));
  return EMPTY_STRING;
};

std::any
Visitor::visitAction_meta_data_msg(Antlr4Gen::SecLangParser::Action_meta_data_msgContext* ctx) {
  // Clear the old msg
  if (current_rule_->visitActionMode() == CurrentRule::VisitActionMode::SecRuleUpdateAction) {
    parser_->clearRuleMsgIndex({current_rule_->get()->phase(), current_rule_->get()->index()});
    current_rule_->get()->msg("");
  }

  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> macro =
      getMacro(ctx->string_with_macro()->getText(), ctx->string_with_macro()->variable(),
               ctx->string_with_macro()->STRING().empty());

  if (!macro.has_value()) {
    RETURN_ERROR(macro.error());
  }

  if (macro.value()) {
    current_rule_->get()->msg(std::move(macro.value()));
  } else {
    current_rule_->get()->msg(ctx->string_with_macro()->getText());
    // Only set the msg index for update action. The created rule's msg index will be set when the
    // rule is added to parser(Parser::secRule)
    if (current_rule_->visitActionMode() == CurrentRule::VisitActionMode::SecRuleUpdateAction) {
      parser_->setRuleMsgIndex({current_rule_->get()->phase(), current_rule_->get()->index()});
    }
  }

  return EMPTY_STRING;
};

std::any
Visitor::visitAction_meta_data_tag(Antlr4Gen::SecLangParser::Action_meta_data_tagContext* ctx) {
  auto& tags = current_rule_->get()->tags();
  std::string tag = ctx->STRING()->getText();
  current_rule_->get()->tags(std::move(tag));

  return EMPTY_STRING;
};

std::any
Visitor::visitAction_meta_data_ver(Antlr4Gen::SecLangParser::Action_meta_data_verContext* ctx) {
  current_rule_->get()->ver(ctx->STRING()->getText());
  return EMPTY_STRING;
};

std::any
Visitor::visitAction_meta_data_rev(Antlr4Gen::SecLangParser::Action_meta_data_revContext* ctx) {
  current_rule_->get()->rev(ctx->STRING()->getText());
  return EMPTY_STRING;
};

std::any Visitor::visitAction_meta_data_accuracy(
    Antlr4Gen::SecLangParser::Action_meta_data_accuracyContext* ctx) {
  current_rule_->get()->accuracy(::atoll(ctx->LEVEL()->getText().c_str()));
  return EMPTY_STRING;
};

std::any Visitor::visitAction_meta_data_maturity(
    Antlr4Gen::SecLangParser::Action_meta_data_maturityContext* ctx) {
  current_rule_->get()->maturity(::atoll(ctx->LEVEL()->getText().c_str()));
  return EMPTY_STRING;
};

std::any Visitor::visitAction_meta_data_severity_emergency(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_emergencyContext* ctx) {
  current_rule_->get()->severity(Wge::Rule::Severity::EMERGENCY);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_alert(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_alertContext* ctx) {
  current_rule_->get()->severity(Wge::Rule::Severity::ALERT);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_critical(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_criticalContext* ctx) {
  current_rule_->get()->severity(Wge::Rule::Severity::CRITICAL);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_error(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_errorContext* ctx) {
  current_rule_->get()->severity(Wge::Rule::Severity::ERROR);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_waring(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_waringContext* ctx) {
  current_rule_->get()->severity(Wge::Rule::Severity::WARNING);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_notice(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_noticeContext* ctx) {
  current_rule_->get()->severity(Wge::Rule::Severity::NOTICE);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_info(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_infoContext* ctx) {
  current_rule_->get()->severity(Wge::Rule::Severity::INFO);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_debug(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_debugContext* ctx) {
  current_rule_->get()->severity(Wge::Rule::Severity::DEBUG);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_number(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_numberContext* ctx) {
  uint32_t serverity_level = ::atol(ctx->SEVERITY_LEVEL()->getText().c_str());
  current_rule_->get()->severity(static_cast<Wge::Rule::Severity>(serverity_level));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_setvar_create(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_createContext* ctx) {
  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> key_macro =
      getMacro(ctx->action_non_disruptive_setvar_varname()->getText(),
               ctx->action_non_disruptive_setvar_varname()->variable(),
               ctx->action_non_disruptive_setvar_varname()->VAR_NAME().empty());

  if (!key_macro.has_value()) {
    RETURN_ERROR(key_macro.error());
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  if (ctx->ALWAYS() || ctx->UNMATCHED()) {
    if (current_rule_->visitActionMode() != CurrentRule::VisitActionMode::SecRule) {
      RETURN_ERROR("The ALWAYS and UNMATCHED branches are only allowed in SecRule actions.");
    }
    branch =
        ctx->ALWAYS() ? Action::ActionBase::Branch::Always : Action::ActionBase::Branch::Unmatched;
  }

  if (key_macro.value()) {
    current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
        branch, parser_->getCurrentNamespace(), std::move(key_macro.value()), Common::Variant(),
        Action::SetVar::EvaluateType::Create));
  } else {
    std::string key = ctx->action_non_disruptive_setvar_varname()->getText();
    size_t index = parser_->getTxVariableIndex(parser_->getCurrentNamespace(), key, true).value();
    current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
        branch, parser_->getCurrentNamespace(), std::move(key), index, Common::Variant(),
        Action::SetVar::EvaluateType::Create));
  }

  return EMPTY_STRING;
};

std::any Visitor::visitAction_non_disruptive_setvar_create_init(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_create_initContext* ctx) {
  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> key_macro =
      getMacro(ctx->action_non_disruptive_setvar_varname()->getText(),
               ctx->action_non_disruptive_setvar_varname()->variable(),
               ctx->action_non_disruptive_setvar_varname()->VAR_NAME().empty());

  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> value_macro =
      getMacro(ctx->action_non_disruptive_setvar_create_init_value()->getText(),
               ctx->action_non_disruptive_setvar_create_init_value()->variable(),
               ctx->action_non_disruptive_setvar_create_init_value()->VAR_VALUE().empty());

  if (!key_macro.has_value()) {
    RETURN_ERROR(key_macro.error());
  }

  if (!value_macro.has_value()) {
    RETURN_ERROR(value_macro.error());
  }

  // If the value is all of digits, then convert the value_variant to int. Otherwise, convert the
  // value_variant to string.
  std::string value_string;
  Common::Variant value_variant;
  if (!value_macro.value()) {
    if (ctx->action_non_disruptive_setvar_create_init_value()->VAR_RAW_VALUE()) {
      value_string =
          ctx->action_non_disruptive_setvar_create_init_value()->VAR_RAW_VALUE()->getText();
    } else {
      value_string =
          ctx->action_non_disruptive_setvar_create_init_value()->VAR_VALUE().front()->getText();
    }
    Common::Variant variant;
    if (std::all_of(value_string.begin(), value_string.end(), ::isdigit)) {
      value_variant = ::atoll(value_string.c_str());
    } else {
      value_variant = value_string;
    }
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  if (ctx->ALWAYS() || ctx->UNMATCHED()) {
    if (current_rule_->visitActionMode() != CurrentRule::VisitActionMode::SecRule) {
      RETURN_ERROR("The ALWAYS and UNMATCHED branches are only allowed in SecRule actions.");
    }
    branch =
        ctx->ALWAYS() ? Action::ActionBase::Branch::Always : Action::ActionBase::Branch::Unmatched;
  }

  if (key_macro.value()) {
    if (value_macro.value()) {
      current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
          branch, parser_->getCurrentNamespace(), std::move(key_macro.value()),
          std::move(value_macro.value()), Action::SetVar::EvaluateType::CreateAndInit));
    } else {
      current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
          branch, parser_->getCurrentNamespace(), std::move(key_macro.value()),
          std::move(value_variant), Action::SetVar::EvaluateType::CreateAndInit));
    }
  } else {
    std::string key = ctx->action_non_disruptive_setvar_varname()->getText();
    size_t index = parser_->getTxVariableIndex(parser_->getCurrentNamespace(), key, true).value();
    if (value_macro.value()) {
      current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
          branch, parser_->getCurrentNamespace(), std::move(key), index,
          std::move(value_macro.value()), Action::SetVar::EvaluateType::CreateAndInit));
    } else {
      current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
          branch, parser_->getCurrentNamespace(), std::move(key), index, std::move(value_variant),
          Action::SetVar::EvaluateType::CreateAndInit));
    }
  }

  return EMPTY_STRING;
};

std::any Visitor::visitAction_non_disruptive_setvar_remove(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_removeContext* ctx) {
  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> key_macro =
      getMacro(ctx->action_non_disruptive_setvar_varname()->getText(),
               ctx->action_non_disruptive_setvar_varname()->variable(),
               ctx->action_non_disruptive_setvar_varname()->VAR_NAME().empty());

  if (!key_macro.has_value()) {
    RETURN_ERROR(key_macro.error());
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  if (ctx->ALWAYS() || ctx->UNMATCHED()) {
    if (current_rule_->visitActionMode() != CurrentRule::VisitActionMode::SecRule) {
      RETURN_ERROR("The ALWAYS and UNMATCHED branches are only allowed in SecRule actions.");
    }
    branch =
        ctx->ALWAYS() ? Action::ActionBase::Branch::Always : Action::ActionBase::Branch::Unmatched;
  }

  if (key_macro.value()) {
    current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
        branch, parser_->getCurrentNamespace(), std::move(key_macro.value()), Common::Variant(),
        Action::SetVar::EvaluateType::Remove));
  } else {
    std::string key = ctx->action_non_disruptive_setvar_varname()->getText();
    size_t index = parser_->getTxVariableIndex(parser_->getCurrentNamespace(), key, true).value();
    current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
        branch, parser_->getCurrentNamespace(), std::move(key), index, Common::Variant(),
        Action::SetVar::EvaluateType::Remove));
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_setvar_increase(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_increaseContext* ctx) {
  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> key_macro =
      getMacro(ctx->action_non_disruptive_setvar_varname()->getText(),
               ctx->action_non_disruptive_setvar_varname()->variable(),
               ctx->action_non_disruptive_setvar_varname()->VAR_NAME().empty());

  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> value_macro =
      ctx->variable() ? getMacro(ctx->variable()->getText(), {ctx->variable()}, true) : nullptr;

  if (!key_macro.has_value()) {
    RETURN_ERROR(key_macro.error());
  }

  if (!value_macro.has_value()) {
    RETURN_ERROR(value_macro.error());
  }

  // If the value is all of digits, then convert the value_variant to int. Otherwise, convert the
  // value_variant to string.
  Common::Variant value_variant;
  if (!value_macro.value()) {
    std::string value_string = ctx->VAR_VALUE()->getText();
    Common::Variant variant;
    if (std::all_of(value_string.begin(), value_string.end(), ::isdigit)) {
      value_variant = ::atoll(value_string.c_str());
    } else {
      value_variant = value_string;
    }
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  if (ctx->ALWAYS() || ctx->UNMATCHED()) {
    if (current_rule_->visitActionMode() != CurrentRule::VisitActionMode::SecRule) {
      RETURN_ERROR("The ALWAYS and UNMATCHED branches are only allowed in SecRule actions.");
    }
    branch =
        ctx->ALWAYS() ? Action::ActionBase::Branch::Always : Action::ActionBase::Branch::Unmatched;
  }

  if (key_macro.value()) {
    if (value_macro.value()) {
      current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
          branch, parser_->getCurrentNamespace(), std::move(key_macro.value()),
          std::move(value_macro.value()), Action::SetVar::EvaluateType::Increase));
    } else {
      current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
          branch, parser_->getCurrentNamespace(), std::move(key_macro.value()),
          std::move(value_variant), Action::SetVar::EvaluateType::Increase));
    }
  } else {
    std::string key = ctx->action_non_disruptive_setvar_varname()->getText();
    size_t index = parser_->getTxVariableIndex(parser_->getCurrentNamespace(), key, true).value();
    if (value_macro.value()) {
      current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
          branch, parser_->getCurrentNamespace(), std::move(key), index,
          std::move(value_macro.value()), Action::SetVar::EvaluateType::Increase));
    } else {
      current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
          branch, parser_->getCurrentNamespace(), std::move(key), index, std::move(value_variant),
          Action::SetVar::EvaluateType::Increase));
    }
  }

  return EMPTY_STRING;
};

std::any Visitor::visitAction_non_disruptive_setvar_decrease(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_decreaseContext* ctx) {
  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> key_macro =
      getMacro(ctx->action_non_disruptive_setvar_varname()->getText(),
               ctx->action_non_disruptive_setvar_varname()->variable(),
               ctx->action_non_disruptive_setvar_varname()->VAR_NAME().empty());

  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> value_macro =
      ctx->variable() ? getMacro(ctx->variable()->getText(), {ctx->variable()}, true) : nullptr;

  if (!key_macro.has_value()) {
    RETURN_ERROR(key_macro.error());
  }

  if (!value_macro.has_value()) {
    RETURN_ERROR(value_macro.error());
  }

  // If the value is all of digits, then convert the value_variant to int. Otherwise, convert the
  // value_variant to string.
  Common::Variant value_variant;
  if (!value_macro.value()) {
    std::string value_string = ctx->VAR_VALUE()->getText();
    Common::Variant variant;
    if (std::all_of(value_string.begin(), value_string.end(), ::isdigit)) {
      value_variant = ::atoll(value_string.c_str());
    } else {
      value_variant = value_string;
    }
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  if (ctx->ALWAYS() || ctx->UNMATCHED()) {
    if (current_rule_->visitActionMode() != CurrentRule::VisitActionMode::SecRule) {
      RETURN_ERROR("The ALWAYS and UNMATCHED branches are only allowed in SecRule actions.");
    }
    branch =
        ctx->ALWAYS() ? Action::ActionBase::Branch::Always : Action::ActionBase::Branch::Unmatched;
  }

  if (key_macro.value()) {
    if (value_macro.value()) {
      current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
          branch, parser_->getCurrentNamespace(), std::move(key_macro.value()),
          std::move(value_macro.value()), Action::SetVar::EvaluateType::Decrease));
    } else {
      current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
          branch, parser_->getCurrentNamespace(), std::move(key_macro.value()),
          std::move(value_variant), Action::SetVar::EvaluateType::Decrease));
    }
  } else {
    std::string key = ctx->action_non_disruptive_setvar_varname()->getText();
    size_t index = parser_->getTxVariableIndex(parser_->getCurrentNamespace(), key, true).value();
    if (value_macro.value()) {
      current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
          branch, parser_->getCurrentNamespace(), std::move(key), index,
          std::move(value_macro.value()), Action::SetVar::EvaluateType::Decrease));
    } else {
      current_rule_->get()->appendAction(std::make_unique<Action::SetVar>(
          branch, parser_->getCurrentNamespace(), std::move(key), index, std::move(value_variant),
          Action::SetVar::EvaluateType::Decrease));
    }
  }

  return EMPTY_STRING;
};

std::any Visitor::visitAction_non_disruptive_setenv(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setenvContext* ctx) {
  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> value_macro =
      ctx->variable() ? getMacro(ctx->variable()->getText(), {ctx->variable()}, true) : nullptr;

  if (!value_macro.has_value()) {
    RETURN_ERROR(value_macro.error());
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  if (ctx->ALWAYS() || ctx->UNMATCHED()) {
    if (current_rule_->visitActionMode() != CurrentRule::VisitActionMode::SecRule) {
      RETURN_ERROR("The ALWAYS and UNMATCHED branches are only allowed in SecRule actions.");
    }
    branch =
        ctx->ALWAYS() ? Action::ActionBase::Branch::Always : Action::ActionBase::Branch::Unmatched;
  }

  if (value_macro.value()) {
    current_rule_->get()->appendAction(std::make_unique<Action::SetEnv>(
        branch, ctx->VAR_NAME()->getText(), std::move(value_macro.value())));
  } else {
    current_rule_->get()->appendAction(std::make_unique<Action::SetEnv>(
        branch, ctx->VAR_NAME()->getText(), ctx->VAR_VALUE()->getText()));
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_setuid(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setuidContext* ctx) {
  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> value_macro =
      ctx->variable() ? getMacro(ctx->variable()->getText(), {ctx->variable()}, true) : nullptr;

  if (!value_macro.has_value()) {
    RETURN_ERROR(value_macro.error());
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  if (ctx->ALWAYS() || ctx->UNMATCHED()) {
    if (current_rule_->visitActionMode() != CurrentRule::VisitActionMode::SecRule) {
      RETURN_ERROR("The ALWAYS and UNMATCHED branches are only allowed in SecRule actions.");
    }
    branch =
        ctx->ALWAYS() ? Action::ActionBase::Branch::Always : Action::ActionBase::Branch::Unmatched;
  }

  if (value_macro.value()) {
    current_rule_->get()->appendAction(
        std::make_unique<Action::SetUid>(branch, std::move(value_macro.value())));
  } else {
    current_rule_->get()->appendAction(
        std::make_unique<Action::SetUid>(branch, ctx->STRING()->getText()));
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_setrsc(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setrscContext* ctx) {
  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> value_macro =
      ctx->variable() ? getMacro(ctx->variable()->getText(), {ctx->variable()}, true) : nullptr;

  if (!value_macro.has_value()) {
    RETURN_ERROR(value_macro.error());
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  if (ctx->ALWAYS() || ctx->UNMATCHED()) {
    if (current_rule_->visitActionMode() != CurrentRule::VisitActionMode::SecRule) {
      RETURN_ERROR("The ALWAYS and UNMATCHED branches are only allowed in SecRule actions.");
    }
    branch =
        ctx->ALWAYS() ? Action::ActionBase::Branch::Always : Action::ActionBase::Branch::Unmatched;
  }

  if (value_macro.value()) {
    current_rule_->get()->appendAction(
        std::make_unique<Action::SetRsc>(branch, std::move(value_macro.value())));
  } else {
    current_rule_->get()->appendAction(
        std::make_unique<Action::SetRsc>(branch, ctx->STRING()->getText()));
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_setsid(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setsidContext* ctx) {
  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> value_macro =
      ctx->variable() ? getMacro(ctx->variable()->getText(), {ctx->variable()}, true) : nullptr;

  if (!value_macro.has_value()) {
    RETURN_ERROR(value_macro.error());
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  if (ctx->ALWAYS() || ctx->UNMATCHED()) {
    if (current_rule_->visitActionMode() != CurrentRule::VisitActionMode::SecRule) {
      RETURN_ERROR("The ALWAYS and UNMATCHED branches are only allowed in SecRule actions.");
    }
    branch =
        ctx->ALWAYS() ? Action::ActionBase::Branch::Always : Action::ActionBase::Branch::Unmatched;
  }

  if (value_macro.value()) {
    current_rule_->get()->appendAction(
        std::make_unique<Action::SetSid>(branch, std::move(value_macro.value())));
  } else {
    current_rule_->get()->appendAction(
        std::make_unique<Action::SetSid>(branch, ctx->STRING()->getText()));
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_base64_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_base64_decodeContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Base64Decode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_sql_hex_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_sql_hex_decodeContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::SqlHexDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_base64_decode_ext(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_base64_decode_extContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Base64DecodeExt>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_base64_encode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_base64_encodeContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Base64Encode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_cmdline(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_cmdlineContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::CmdLine>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_compress_whitespace(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_compress_whitespaceContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::CompressWhiteSpace>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_css_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_css_decodeContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::CssDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_escape_seq_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_escape_seq_decodeContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::EscapeSeqDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_hex_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_hex_decodeContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::HexDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_hex_encode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_hex_encodeContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::HexEncode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_html_entity_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_html_entity_decodeContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::HtmlEntityDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_js_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_js_decodeContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::JsDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_length(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_lengthContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Length>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_lowercase(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_lowercaseContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::LowerCase>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_md5(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_md5Context* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Md5>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_none(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_noneContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.clear();
  current_rule_->get()->isIgnoreDefaultTransform(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_normalise_path(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_normalise_pathContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::NormalisePath>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_normalize_path(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_normalize_pathContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::NormalizePath>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_normalise_pathwin(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_normalise_pathwinContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::NormalisePathWin>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_normalize_pathwin(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_normalize_pathwinContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::NormalizePathWin>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_parity_even_7bit(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_parity_even_7bitContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::ParityEven7Bit>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_parity_odd_7bit(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_parity_odd_7bitContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::ParityOdd7Bit>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_parity_zero_7bit(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_parity_zero_7bitContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::ParityZero7Bit>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_remove_nulls(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_remove_nullsContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::RemoveNulls>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_remove_whitespace(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_remove_whitespaceContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::RemoveWhitespace>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_replace_comments(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_replace_commentsContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::ReplaceComments>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_remove_commentschar(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_remove_commentscharContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::RemoveCommentsChar>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_remove_comments(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_remove_commentsContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::RemoveComments>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_replace_nulls(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_replace_nullsContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::ReplaceNulls>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_url_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_url_decodeContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::UrlDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_uppercase(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_uppercaseContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::UpperCase>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_url_decode_uni(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_url_decode_uniContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::UrlDecodeUni>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_url_encode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_url_encodeContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::UrlEncode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_utf8_to_unicode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_utf8_to_unicodeContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Utf8ToUnicode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_sha1(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_sha1Context* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Sha1>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_trim_left(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_trim_leftContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::TrimLeft>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_trim_right(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_trim_rightContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::TrimRight>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_trim(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_trimContext* ctx) {
  auto& transforms = current_rule_->get()->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Trim>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_audit_engine(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_audit_engineContext* ctx) {
  using Option = Wge::AuditLogConfig::AuditEngine;
  Option option = Option::Off;

  std::string option_str = ctx->AUDIT_ENGINE()->getText();
  if (option_str == "On") {
    option = Option::On;
  } else if (option_str == "RelevantOnly") {
    option = Option::RelevantOnly;
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  auto* parent_ctx =
      dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
  if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
    branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                  : Action::ActionBase::Branch::Unmatched;
  }

  current_rule_->get()->appendAction(
      std::make_unique<Action::Ctl>(branch, Action::Ctl::CtlType::AuditEngine, option));

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_audit_log_parts(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_audit_log_partsContext* ctx) {
  std::string parts = ctx->AUDIT_PARTS()->getText();

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  auto* parent_ctx =
      dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
  if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
    branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                  : Action::ActionBase::Branch::Unmatched;
  }

  current_rule_->get()->appendAction(
      std::make_unique<Action::Ctl>(branch, Action::Ctl::CtlType::AuditLogParts, parts));

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_force_request_body_variable(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_force_request_body_variableContext* ctx) {
  // Not implemented in ModSecurity v3 (REQUEST_BODY is always populated in v3)
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_parse_xml_into_args(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_parse_xml_into_argsContext* ctx) {
  using Option = Wge::ParseXmlIntoArgsOption;
  Option option = Option::Off;

  std::string option_str = ctx->OPTION()->getText();
  if (option_str == "On") {
    option = Option::On;
  } else if (option_str == "OnlyArgs") {
    option = Option::OnlyArgs;
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  auto* parent_ctx =
      dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
  if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
    branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                  : Action::ActionBase::Branch::Unmatched;
  }

  current_rule_->get()->appendAction(
      std::make_unique<Action::Ctl>(branch, Action::Ctl::CtlType::ParseXmlIntoArgs, option));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_request_body_access(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_accessContext* ctx) {
  using Option = Wge::EngineConfig::Option;
  Option option = Option::Off;

  std::string option_str = ctx->OPTION()->getText();
  if (option_str == "On") {
    option = Option::On;
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  auto* parent_ctx =
      dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
  if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
    branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                  : Action::ActionBase::Branch::Unmatched;
  }

  current_rule_->get()->appendAction(
      std::make_unique<Action::Ctl>(branch, Action::Ctl::CtlType::RequestBodyAccess, option));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_request_body_processor_url_encode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_processor_url_encodeContext*
        ctx) {

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  auto* parent_ctx =
      dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
  if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
    branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                  : Action::ActionBase::Branch::Unmatched;
  }

  current_rule_->get()->appendAction(std::make_unique<Action::Ctl>(
      branch, Action::Ctl::CtlType::RequestBodyProcessor, BodyProcessorType::UrlEncoded));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_request_body_processor_multi_part(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_processor_multi_partContext*
        ctx) {
  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  auto* parent_ctx =
      dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
  if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
    branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                  : Action::ActionBase::Branch::Unmatched;
  }

  current_rule_->get()->appendAction(std::make_unique<Action::Ctl>(
      branch, Action::Ctl::CtlType::RequestBodyProcessor, BodyProcessorType::MultiPart));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_request_body_processor_xml(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_processor_xmlContext* ctx) {
  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  auto* parent_ctx =
      dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
  if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
    branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                  : Action::ActionBase::Branch::Unmatched;
  }

  current_rule_->get()->appendAction(std::make_unique<Action::Ctl>(
      branch, Action::Ctl::CtlType::RequestBodyProcessor, BodyProcessorType::Xml));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_request_body_processor_json(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_processor_jsonContext* ctx) {
  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  auto* parent_ctx =
      dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
  if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
    branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                  : Action::ActionBase::Branch::Unmatched;
  }

  current_rule_->get()->appendAction(std::make_unique<Action::Ctl>(
      branch, Action::Ctl::CtlType::RequestBodyProcessor, BodyProcessorType::Json));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_rule_engine(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_engineContext* ctx) {
  Wge::EngineConfig::Option option = optionStr2EnumValue(ctx->OPTION()->getText());

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  auto* parent_ctx =
      dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
  if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
    branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                  : Action::ActionBase::Branch::Unmatched;
  }

  current_rule_->get()->appendAction(
      std::make_unique<Action::Ctl>(branch, Action::Ctl::CtlType::RuleEngine, option));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_rule_remove_by_id(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_remove_by_idContext* ctx) {
  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  auto* parent_ctx =
      dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
  if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
    branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                  : Action::ActionBase::Branch::Unmatched;
  }

  if (ctx->INT()) {
    uint64_t id = ::atoll(ctx->INT()->getText().c_str());
    current_rule_->get()->appendAction(
        std::make_unique<Action::Ctl>(branch, Action::Ctl::CtlType::RuleRemoveById, id));
  } else {
    std::string id_range_str = ctx->INT_RANGE()->getText();
    auto pos = id_range_str.find('-');
    if (pos != std::string::npos) {
      uint64_t first = ::atoll(id_range_str.substr(0, pos).c_str());
      uint64_t last = ::atoll(id_range_str.substr(pos + 1).c_str());
      current_rule_->get()->appendAction(std::make_unique<Action::Ctl>(
          branch, Action::Ctl::CtlType::RuleRemoveByIdRange, std::make_pair(first, last)));
    }
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_rule_remove_by_tag(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_remove_by_tagContext* ctx) {
  std::string tag = ctx->STRING()->getText();

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  auto* parent_ctx =
      dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
  if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
    branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                  : Action::ActionBase::Branch::Unmatched;
  }

  current_rule_->get()->appendAction(
      std::make_unique<Action::Ctl>(branch, Action::Ctl::CtlType::RuleRemoveByTag, std::move(tag)));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_rule_remove_target_by_id(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_remove_target_by_idContext* ctx) {
  auto old_visit_variable_mode = current_rule_->visitVariableMode();

  uint64_t id = ::atoll(ctx->INT()->getText().c_str());

  auto variables = ctx->variables()->variable();
  std::any visit_result;
  try {
    std::vector<std::shared_ptr<Variable::VariableBase>> variable_objects;

    current_rule_->visitVariableMode(CurrentRule::VisitVariableMode::Ctl);
    for (auto variable : variables) {
      visit_result = visitChildren(variable);
      auto var_obj = std::any_cast<std::shared_ptr<Variable::VariableBase>>(visit_result);
      variable_objects.emplace_back(var_obj);
    }

    Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
    auto* parent_ctx =
        dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
    if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
      branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                    : Action::ActionBase::Branch::Unmatched;
    }

    current_rule_->get()->appendAction(
        std::make_unique<Action::Ctl>(branch, Action::Ctl::CtlType::RuleRemoveTargetById,
                                      std::make_pair(id, std::move(variable_objects))));
  } catch (const std::bad_any_cast& ex) {
    assert(false);
    current_rule_->visitVariableMode(old_visit_variable_mode);
    return std::format("Expect a variable object, but not. return: {}",
                       std::any_cast<std::string>(visit_result));
  }

  current_rule_->visitVariableMode(old_visit_variable_mode);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_rule_remove_target_by_tag(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_remove_target_by_tagContext* ctx) {
  auto old_visit_variable_mode = current_rule_->visitVariableMode();

  std::string tag = ctx->STRING()->getText();

  auto variables = ctx->variables()->variable();
  std::any visit_result;
  try {
    std::vector<std::shared_ptr<Variable::VariableBase>> variable_objects;

    current_rule_->visitVariableMode(CurrentRule::VisitVariableMode::Ctl);
    for (auto variable : variables) {
      visit_result = visitChildren(variable);
      auto var_obj = std::any_cast<std::shared_ptr<Variable::VariableBase>>(visit_result);
      variable_objects.emplace_back(var_obj);
    }

    Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
    auto* parent_ctx =
        dynamic_cast<Antlr4Gen::SecLangParser::Action_non_disruptive_ctlContext*>(ctx->parent);
    if (parent_ctx && (parent_ctx->ALWAYS() || parent_ctx->UNMATCHED())) {
      branch = parent_ctx->ALWAYS() ? Action::ActionBase::Branch::Always
                                    : Action::ActionBase::Branch::Unmatched;
    }

    current_rule_->get()->appendAction(
        std::make_unique<Action::Ctl>(branch, Action::Ctl::CtlType::RuleRemoveTargetByTag,
                                      std::make_pair(std::move(tag), std::move(variable_objects))));
  } catch (const std::bad_any_cast& ex) {
    current_rule_->visitVariableMode(old_visit_variable_mode);
    return std::format("Expect a variable object, but not. return: {}",
                       std::any_cast<std::string>(visit_result));
  }

  current_rule_->visitVariableMode(old_visit_variable_mode);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_audit_log(
    Antlr4Gen::SecLangParser::Action_non_disruptive_audit_logContext* ctx) {
  current_rule_->get()->auditLog(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_log(
    Antlr4Gen::SecLangParser::Action_non_disruptive_logContext* ctx) {
  current_rule_->get()->log(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_no_audit_log(
    Antlr4Gen::SecLangParser::Action_non_disruptive_no_audit_logContext* ctx) {
  current_rule_->get()->noAuditLog(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_no_log(
    Antlr4Gen::SecLangParser::Action_non_disruptive_no_logContext* ctx) {
  current_rule_->get()->noLog(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_logdata(
    Antlr4Gen::SecLangParser::Action_non_disruptive_logdataContext* ctx) {
  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> macro =
      getMacro(ctx->string_with_macro()->getText(), ctx->string_with_macro()->variable(),
               ctx->string_with_macro()->STRING().empty());

  if (!macro.has_value()) {
    RETURN_ERROR(macro.error());
  }

  if (macro.value()) {
    current_rule_->get()->logData(std::move(macro.value()));
  } else {
    current_rule_->get()->logData(ctx->string_with_macro()->getText());
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_capture(
    Antlr4Gen::SecLangParser::Action_non_disruptive_captureContext* ctx) {
  current_rule_->get()->capture(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_multi_match(
    Antlr4Gen::SecLangParser::Action_non_disruptive_multi_matchContext* ctx) {
  current_rule_->get()->multiMatch(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_initcol(
    Antlr4Gen::SecLangParser::Action_non_disruptive_initcolContext* ctx) {
  Wge::PersistentStorage::Storage::Type type;
  if (ctx->persistent_storage_collection()->INIT_COL_GLOBAL()) {
    type = Wge::PersistentStorage::Storage::Type::GLOBAL;
  } else if (ctx->persistent_storage_collection()->INIT_COL_RESOURCE()) {
    type = Wge::PersistentStorage::Storage::Type::RESOURCE;
  } else if (ctx->persistent_storage_collection()->INIT_COL_IP()) {
    type = Wge::PersistentStorage::Storage::Type::IP;
  } else if (ctx->persistent_storage_collection()->INIT_COL_SESSION()) {
    type = Wge::PersistentStorage::Storage::Type::SESSION;
  } else if (ctx->persistent_storage_collection()->INIT_COL_USER()) {
    type = Wge::PersistentStorage::Storage::Type::USER;
  } else {
    RETURN_ERROR("Invalid persistent storage collection type");
  }

  std::string name = ctx->persistent_storage_collection()->getText();

  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> macro =
      getMacro(ctx->string_with_macro()->getText(), ctx->string_with_macro()->variable(),
               ctx->string_with_macro()->STRING().empty());

  if (!macro.has_value()) {
    RETURN_ERROR(macro.error());
  }

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  if (ctx->ALWAYS() || ctx->UNMATCHED()) {
    if (current_rule_->visitActionMode() != CurrentRule::VisitActionMode::SecRule) {
      RETURN_ERROR("The ALWAYS and UNMATCHED branches are only allowed in SecRule actions.");
    }
    branch =
        ctx->ALWAYS() ? Action::ActionBase::Branch::Always : Action::ActionBase::Branch::Unmatched;
  }

  if (macro.value()) {
    current_rule_->get()->appendAction(
        std::make_unique<Action::InitCol>(branch, type, std::move(name), std::move(macro.value())));
  } else {
    current_rule_->get()->appendAction(std::make_unique<Action::InitCol>(
        branch, type, std::move(name), ctx->string_with_macro()->getText()));
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_disruptive_allow(
    Antlr4Gen::SecLangParser::Action_disruptive_allowContext* ctx) {
  if (ctx->Allow()) {
    current_rule_->get()->disruptive(Rule::Disruptive::ALLOW);
  } else if (ctx->AllowPhase()) {
    current_rule_->get()->disruptive(Rule::Disruptive::ALLOW_PHASE);
  } else if (ctx->AllowRequest()) {
    current_rule_->get()->disruptive(Rule::Disruptive::ALLOW_REQUEST);
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_disruptive_block(
    Antlr4Gen::SecLangParser::Action_disruptive_blockContext* ctx) {
  current_rule_->get()->disruptive(Rule::Disruptive::BLOCK);
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_disruptive_deny(Antlr4Gen::SecLangParser::Action_disruptive_denyContext* ctx) {
  current_rule_->get()->disruptive(Rule::Disruptive::DENY);
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_disruptive_drop(Antlr4Gen::SecLangParser::Action_disruptive_dropContext* ctx) {
  current_rule_->get()->disruptive(Rule::Disruptive::DROP);
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_disruptive_pass(Antlr4Gen::SecLangParser::Action_disruptive_passContext* ctx) {
  current_rule_->get()->disruptive(Rule::Disruptive::PASS);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_disruptive_redirect(
    Antlr4Gen::SecLangParser::Action_disruptive_redirectContext* ctx) {
  current_rule_->get()->disruptive(Rule::Disruptive::REDIRECT);
  current_rule_->get()->redirect(ctx->STRING()->getText());
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_data_status(Antlr4Gen::SecLangParser::Action_data_statusContext* ctx) {
  current_rule_->get()->status(ctx->INT()->getText());
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_data_xml_ns(Antlr4Gen::SecLangParser::Action_data_xml_nsContext* ctx) {
  current_rule_->get()->xmlns(ctx->STRING()->getText());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_flow_chain(Antlr4Gen::SecLangParser::Action_flow_chainContext* ctx) {
  chain_ = true;

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  if (ctx->ALWAYS() || ctx->UNMATCHED()) {
    if (current_rule_->visitActionMode() != CurrentRule::VisitActionMode::SecRule) {
      RETURN_ERROR("The ALWAYS and UNMATCHED branches are only allowed in SecRule actions.");
    }
    branch =
        ctx->ALWAYS() ? Action::ActionBase::Branch::Always : Action::ActionBase::Branch::Unmatched;
  }

  switch (branch) {
  case Action::ActionBase::Branch::Matched:
    current_rule_->get()->matchedChain(true);
    break;
  case Action::ActionBase::Branch::Unmatched:
    current_rule_->get()->unmatchedChain(true);
    break;
  case Action::ActionBase::Branch::Always:
    current_rule_->get()->matchedChain(true);
    current_rule_->get()->unmatchedChain(true);
    break;
  default:
    break;
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_flow_skip(Antlr4Gen::SecLangParser::Action_flow_skipContext* ctx) {
  current_rule_->get()->skip(::atol(ctx->INT()->getText().c_str()));
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_flow_skip_after(Antlr4Gen::SecLangParser::Action_flow_skip_afterContext* ctx) {
  current_rule_->get()->skipAfter(ctx->STRING()->getText());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_extension_first_match(
    Antlr4Gen::SecLangParser::Action_extension_first_matchContext* ctx) {
  if (current_rule_->get()->allMatch()) {
    RETURN_ERROR("Cannot use firstMatch and allMatch together.");
  } else {
    current_rule_->get()->firstMatch(true);
  }
  return EMPTY_STRING;
}

std::any Visitor::visitAction_extension_empty_match(
    Antlr4Gen::SecLangParser::Action_extension_empty_matchContext* ctx) {
  current_rule_->get()->emptyMatch(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_extension_all_match(
    Antlr4Gen::SecLangParser::Action_extension_all_matchContext* ctx) {
  if (current_rule_->get()->firstMatch()) {
    RETURN_ERROR("Cannot use firstMatch and allMatch together.");
  } else {
    current_rule_->get()->allMatch(true);
  }
  return EMPTY_STRING;
}

std::any Visitor::visitAction_extension_multi_chain(
    Antlr4Gen::SecLangParser::Action_extension_multi_chainContext* ctx) {
  chain_ = true;

  Action::ActionBase::Branch branch = Action::ActionBase::Branch::Matched;
  if (ctx->ALWAYS() || ctx->UNMATCHED()) {
    if (current_rule_->visitActionMode() != CurrentRule::VisitActionMode::SecRule) {
      RETURN_ERROR("The ALWAYS and UNMATCHED branches are only allowed in SecRule actions.");
    }
    branch =
        ctx->ALWAYS() ? Action::ActionBase::Branch::Always : Action::ActionBase::Branch::Unmatched;
  }

  switch (branch) {
  case Action::ActionBase::Branch::Matched:
    current_rule_->get()->matchedMultiChain(true);
    break;
  case Action::ActionBase::Branch::Unmatched:
    current_rule_->get()->unmatchedMultiChain(true);
    break;
  case Action::ActionBase::Branch::Always:
    current_rule_->get()->matchedMultiChain(true);
    current_rule_->get()->unmatchedMultiChain(true);
    break;
  default:
    break;
  }

  return EMPTY_STRING;
}

std::any
Visitor::visitAction_extension_alias(Antlr4Gen::SecLangParser::Action_extension_aliasContext* ctx) {
  std::string matched_tree;
  std::string low_case_matched_tree;
  if (ctx->variable_matched_optree()) {
    matched_tree = ctx->variable_matched_optree()->getText();
    low_case_matched_tree = "matched_optree" + matched_tree.substr(14);
  } else if (ctx->variable_matched_vptree()) {
    matched_tree = ctx->variable_matched_vptree()->getText();
    low_case_matched_tree = "matched_vptree" + matched_tree.substr(14);
  }

  alias_[ctx->ALIAS_NAME()->getText()] = low_case_matched_tree;

  return EMPTY_STRING;
}

std::any Visitor::visitSec_audit_engine(Antlr4Gen::SecLangParser::Sec_audit_engineContext* ctx) {
  using Option = Wge::AuditLogConfig::AuditEngine;
  Option option = Option::Off;

  std::string option_str = ctx->AUDIT_ENGINE()->getText();
  if (option_str == "On") {
    option = Option::On;
  } else if (option_str == "RelevantOnly") {
    option = Option::RelevantOnly;
  }
  parser_->secAuditEngine(option);
  return EMPTY_STRING;
}

std::any Visitor::visitSec_audit_log(Antlr4Gen::SecLangParser::Sec_audit_logContext* ctx) {
  std::string path = ctx->STRING()->getText();
  parser_->secAuditLog(std::move(path));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_audit_log2(Antlr4Gen::SecLangParser::Sec_audit_log2Context* ctx) {
  std::string path = ctx->STRING()->getText();
  parser_->secAuditLog2(std::move(path));
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_audit_log_dir_mode(Antlr4Gen::SecLangParser::Sec_audit_log_dir_modeContext* ctx) {
  int mode = ::strtol(ctx->OCTAL()->getText().c_str(), nullptr, 8);
  parser_->secAuditLogDirMode(mode);
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_audit_log_format(Antlr4Gen::SecLangParser::Sec_audit_log_formatContext* ctx) {
  using Format = Wge::AuditLogConfig::AuditFormat;
  Format format = Format::Native;

  std::string format_str = ctx->AUDIT_FORMAT()->getText().c_str();
  if (format_str == "JSON") {
    format = Format::Json;
  }
  parser_->secAuditLogFormat(format);
  return EMPTY_STRING;
}

std::any Visitor::visitSec_audit_log_file_mode(
    Antlr4Gen::SecLangParser::Sec_audit_log_file_modeContext* ctx) {
  int mode = ::strtol(ctx->OCTAL()->getText().c_str(), nullptr, 8);
  parser_->secAuditLogFileMode(mode);
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_audit_log_parts(Antlr4Gen::SecLangParser::Sec_audit_log_partsContext* ctx) {
  std::string parts = ctx->AUDIT_PARTS()->getText();
  parser_->secAuditLogParts(parts);
  return EMPTY_STRING;
}

std::any Visitor::visitSec_audit_log_relevant_status(
    Antlr4Gen::SecLangParser::Sec_audit_log_relevant_statusContext* ctx) {
  std::string pattern = ctx->STRING()->getText();
  parser_->secAuditLogRelevantStatus(std::move(pattern));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_audit_log_storage_dir(
    Antlr4Gen::SecLangParser::Sec_audit_log_storage_dirContext* ctx) {
  std::string dir = ctx->STRING()->getText();
  parser_->secAuditLogStorageDir(std::move(dir));
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_audit_log_type(Antlr4Gen::SecLangParser::Sec_audit_log_typeContext* ctx) {
  using Type = Wge::AuditLogConfig::AuditLogType;
  Type type = Type::Serial;

  std::string type_str = ctx->AUDIT_TYPE()->getText();
  if (type_str == "Concurrent") {
    type = Type::Concurrent;
  } else if (type_str == "HTTPS") {
    type = Type::Https;
  }
  parser_->secAuditLogType(type);
  return EMPTY_STRING;
}

std::any Visitor::visitSec_component_signature(
    Antlr4Gen::SecLangParser::Sec_component_signatureContext* ctx) {
  std::string signature = ctx->STRING()->getText();
  parser_->secComponentSignature(std::move(signature));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_rule_update_operator_by_id(
    Antlr4Gen::SecLangParser::Sec_rule_update_operator_by_idContext* ctx) {
  auto ids = ctx->INT();
  for (auto id : ids) {
    std::string id_str = id->getText();
    uint64_t id_num = ::atoll(id_str.c_str());
    current_rule_ = std::make_unique<CurrentRule>(parser_, id_num);
    if (current_rule_->get()) {
      // Clear existing operators
      current_rule_->get()->clearOperators();

      // Visit operator
      std::string error;
      current_rule_->visitOperatorMode(CurrentRule::VisitOperatorMode::SecRuleUpdateOperator);
      TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
      if (!error.empty()) {
        return error;
      }
    }
  }

  auto id_ranges = ctx->INT_RANGE();
  for (auto range : id_ranges) {
    std::string id_range_str = range->getText();
    auto pos = id_range_str.find('-');
    if (pos != std::string::npos) {
      uint64_t first = ::atoll(id_range_str.substr(0, pos).c_str());
      uint64_t last = ::atoll(id_range_str.substr(pos + 1).c_str());
      for (auto id = first; id <= last; ++id) {
        current_rule_ = std::make_unique<CurrentRule>(parser_, id);
        if (current_rule_->get()) {
          // Clear existing operators
          current_rule_->get()->clearOperators();

          // Visit operator
          std::string error;
          current_rule_->visitOperatorMode(CurrentRule::VisitOperatorMode::SecRuleUpdateOperator);
          TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
          if (!error.empty()) {
            return error;
          }
        }
      }
    }
  }

  auto id_and_chaind_index_array = ctx->ID_AND_CHAIN_INDEX();
  for (auto id_and_chain : id_and_chaind_index_array) {
    std::string id_and_chain_str = id_and_chain->getText();
    auto pos = id_and_chain_str.find(':');
    if (pos == std::string::npos) {
      continue;
    }
    uint64_t id = ::atoll(id_and_chain_str.substr(0, pos).c_str());
    uint64_t chain_index = ::atoll(id_and_chain_str.substr(pos + 1).c_str());
    current_rule_ = std::make_unique<CurrentRule>(parser_, id);
    if (current_rule_->get()) {
      Rule* chain_rule = current_rule_->get()->chainRule(chain_index);
      if (chain_rule) {
        current_rule_ = std::make_unique<CurrentRule>(parser_, chain_rule);

        // Clear existing operators
        current_rule_->get()->clearOperators();

        // Visit operator
        std::string error;
        current_rule_->visitOperatorMode(CurrentRule::VisitOperatorMode::SecRuleUpdateOperator);
        TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
        if (!error.empty()) {
          return error;
        }
      }
    }
  }

  return EMPTY_STRING;
}

std::any Visitor::visitSec_rule_update_operator_by_tag(
    Antlr4Gen::SecLangParser::Sec_rule_update_operator_by_tagContext* ctx) {
  auto rules = parser_->findRuleByTag(ctx->STRING()->getText());
  for (auto rule : rules) {
    current_rule_ = std::make_unique<CurrentRule>(parser_, rule);

    // Clear existing operators
    current_rule_->get()->clearOperators();

    // Visit operator
    std::string error;
    current_rule_->visitOperatorMode(CurrentRule::VisitOperatorMode::SecRuleUpdateOperator);
    TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
    if (!error.empty()) {
      return error;
    }
  }

  return EMPTY_STRING;
}

std::any Visitor::visitSec_tx_namespace(Antlr4Gen::SecLangParser::Sec_tx_namespaceContext* ctx) {
  std::string ns = ctx->STRING()->getText();
  parser_->setCurrentNamespace(ns);
  return EMPTY_STRING;
}

bool Visitor::optionStr2Bool(const std::string& option_str) {
  if (option_str == "On") {
    return true;
  }

  return false;
}

EngineConfig::Option Visitor::optionStr2EnumValue(const std::string& option_str) {
  EngineConfig::Option option = EngineConfig::Option::Off;
  if (option_str == "On") {
    option = EngineConfig::Option::On;
  } else if (option_str == "DetectionOnly") {
    option = EngineConfig::Option::DetectionOnly;
  }
  return option;
}

EngineConfig::BodyLimitAction Visitor::bodyLimitActionStr2EnumValue(const std::string& action_str) {
  EngineConfig::BodyLimitAction action = EngineConfig::BodyLimitAction::ProcessPartial;
  if (action_str == "Reject") {
    action = EngineConfig::BodyLimitAction::Reject;
  }
  return action;
}

std::expected<std::unique_ptr<Macro::MacroBase>, std::string> Visitor::getMacro(
    std::string&& text,
    const std::vector<Wge::Antlr4::Antlr4Gen::SecLangParser::VariableContext*>& macro_ctx_array,
    bool no_string) {
  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> result;

  auto old_visit_variable_mode = current_rule_->visitVariableMode();
  current_rule_->visitVariableMode(CurrentRule::VisitVariableMode::Macro);

  std::string macro_name;
  try {
    if (!macro_ctx_array.empty()) {
      if (no_string && macro_ctx_array.size() == 1) {
        std::any visit_result = visitChildren(macro_ctx_array.front());
        macro_name = macro_ctx_array.front()->getText();
        result = std::unique_ptr<Macro::MacroBase>(std::any_cast<Macro::MacroBase*>(visit_result));
      } else {
        std::vector<std::unique_ptr<Macro::MacroBase>> macros;
        for (auto& macro_ctx : macro_ctx_array) {
          std::any visit_result = visitChildren(macro_ctx);
          macro_name = macro_ctx->getText();
          std::unique_ptr<Macro::MacroBase> macro_ptr(
              std::any_cast<Macro::MacroBase*>(visit_result));
          macros.emplace_back(std::move(macro_ptr));
        }
        result = std::unique_ptr<Macro::MacroBase>(
            new Macro::MultiMacro(std::move(text), std::move(macros)));
      }
    }
  } catch (const std::bad_any_cast& ex) {
    assert(false);
    result = std::unexpected(std::format("Expect a macro object: %{{{}}}, but not.", macro_name));
  }

  current_rule_->visitVariableMode(old_visit_variable_mode);
  return result;
}

void Visitor::setRuleNeedPushMatched(Variable::VariableBase* variable) {
  auto main_name = variable->mainName();
  const bool is_matched_variable = main_name == Variable::MatchedVar::main_name_ ||
                                   main_name == Variable::MatchedVarName::main_name_ ||
                                   main_name == Variable::MatchedVars::main_name_ ||
                                   main_name == Variable::MatchedVarsNames::main_name_;
  if (is_matched_variable) {
    auto parent = current_rule_->parent();
    if (parent) {
      parent->isNeedPushMatched(true);
    } else {
      current_rule_->get()->isNeedPushMatched(true);
    }
  }
}
} // namespace Wge::Antlr4