#include "visitor.h"

#include <format>
#include <unordered_map>

#include "../action/actions_include.h"
#include "../common/log.h"
#include "../common/try.h"
#include "../common/variant.h"
#include "../macro/macro_include.h"
#include "../operator/operator_include.h"
#include "../transformation/transform_include.h"
#include "../variable/variables_include.h"

#define RETURN_ERROR(msg)                                                                          \
  should_visit_next_child_ = false;                                                                \
  return std::format("{} line: {}", msg, ctx->getStart()->getLine());

namespace SrSecurity::Antlr4 {

std::any Visitor::visitInclude(Antlr4Gen::SecLangParser::IncludeContext* ctx) {
  std::string file_path = ctx->STRING()->getText();
  return parser_->loadFromFile(file_path);
}

std::any Visitor::visitSec_reqeust_body_access(
    Antlr4Gen::SecLangParser::Sec_reqeust_body_accessContext* ctx) {
  parser_->secRequestBodyAccess(optionStr2EnumValue(ctx->OPTION()->getText()));
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
  parser_->secResponseBodyAccess(optionStr2EnumValue(ctx->OPTION()->getText()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_rule_engine(Antlr4Gen::SecLangParser::Sec_rule_engineContext* ctx) {
  parser_->secRuleEngine(optionStr2EnumValue(ctx->OPTION()->getText()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_tmp_save_uploaded_files(
    Antlr4Gen::SecLangParser::Sec_tmp_save_uploaded_filesContext* ctx) {
  parser_->secTmpSaveUploadedFiles(optionStr2EnumValue(ctx->OPTION()->getText()));
  return EMPTY_STRING;
}

std::any
Visitor::visitSec_upload_keep_files(Antlr4Gen::SecLangParser::Sec_upload_keep_filesContext* ctx) {
  parser_->secUploadKeepFiles(optionStr2EnumValue(ctx->OPTION()->getText()));
  return EMPTY_STRING;
}

std::any Visitor::visitSec_xml_external_entity(
    Antlr4Gen::SecLangParser::Sec_xml_external_entityContext* ctx) {
  parser_->secXmlExternalEntity(optionStr2EnumValue(ctx->OPTION()->getText()));
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
  SRSECURITY_LOG_WARN("SecStatusEngine is not supported yet.");
  return EMPTY_STRING;
}

std::any Visitor::visitSec_tmp_dir(Antlr4Gen::SecLangParser::Sec_tmp_dirContext* ctx) {
  // Not supported in v3
  SRSECURITY_LOG_WARN("SecTmpDir is not supported yet.");
  return EMPTY_STRING;
}

std::any Visitor::visitSec_data_dir(Antlr4Gen::SecLangParser::Sec_data_dirContext* ctx) {
  // Not supported in v3
  SRSECURITY_LOG_WARN("SecDataDir is not supported yet.");
  return EMPTY_STRING;
}

std::any Visitor::visitSec_cookie_format(Antlr4Gen::SecLangParser::Sec_cookie_formatContext* ctx) {
  // Not supported in v3
  SRSECURITY_LOG_WARN("SecCookieFormat is not supported yet.");
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
  // Get line number
  int line = ctx->getStart()->getLine();

  // Add an empty rule, and sets actions by visitChildren
  current_rule_iter_ = parser_->secAction(line);

  visit_action_mode_ = VisitActionMode::SecAction;
  std::string error;
  TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
  if (!error.empty()) {
    parser_->removeBackRule();
    return error;
  }

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
  SRSECURITY_LOG_WARN("SecCookieFormat is not supported yet.");
  return EMPTY_STRING;
}

std::any Visitor::visitSec_rule(Antlr4Gen::SecLangParser::Sec_ruleContext* ctx) {
  // Get line number
  int line = ctx->getStart()->getLine();

  // Add an empty rule, and sets variable and operators and actions by visitChildren
  if (chain_) {
    current_rule_iter_ = (*current_rule_iter_)->appendChainRule(line);
  } else {
    current_rule_iter_ = parser_->secRule(line);
  }

  chain_ = false;

  // Visit variables and operators and actions
  std::string error;
  visit_variable_mode_ = VisitVariableMode::SecRule;
  visit_action_mode_ = VisitActionMode::SecRule;
  TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
  if (!error.empty()) {
    parser_->removeBackRule();
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
    visit_action_mode_ = VisitActionMode::SecRuleUpdateAction;
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
  current_rule_iter_ = parser_->findRuleById(id);

  if (current_rule_iter_ != parser_->rules().end()) {
    // Visit variables
    std::string error;
    visit_variable_mode_ = VisitVariableMode::SecUpdateTarget;
    TRY_NOCATCH(error = std::any_cast<std::string>(visitChildren(ctx)));
    if (!error.empty()) {
      return error;
    }
  }

  return EMPTY_STRING;
}

std::any Visitor::visitSec_rule_update_target_by_msg(
    Antlr4Gen::SecLangParser::Sec_rule_update_target_by_msgContext* ctx) {
  auto range = parser_->findRuleByMsg(ctx->STRING()->getText());
  visit_variable_mode_ = VisitVariableMode::SecUpdateTarget;
  for (auto iter = range.first; iter != range.second; ++iter) {
    current_rule_iter_ = iter->second;
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
  auto range = parser_->findRuleByTag(ctx->STRING()->getText());
  visit_variable_mode_ = VisitVariableMode::SecUpdateTarget;
  for (auto iter = range.first; iter != range.second; ++iter) {
    current_rule_iter_ = iter->second;
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
  return appendVariable<Variable::ReqBodyProcessor>(ctx);
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
  return appendVariable<Variable::RequestBody>(ctx);
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
  return appendVariable<Variable::RequestHeaders>(ctx);
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
  return appendVariable<Variable::Tx>(ctx);
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

std::any Visitor::visitOp_begins_with(Antlr4Gen::SecLangParser::Op_begins_withContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::BeginsWith(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_contains(Antlr4Gen::SecLangParser::Op_containsContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Contains(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_contains_word(Antlr4Gen::SecLangParser::Op_contains_wordContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::ContainsWord(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_detect_sqli(Antlr4Gen::SecLangParser::Op_detect_sqliContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::DetectSqli(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_detect_xss(Antlr4Gen::SecLangParser::Op_detect_xssContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::DetectXSS(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_ends_with(Antlr4Gen::SecLangParser::Op_ends_withContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::EndsWith(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_fuzzy_hash(Antlr4Gen::SecLangParser::Op_fuzzy_hashContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::FuzzyHash(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_eq(Antlr4Gen::SecLangParser::Op_eqContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Eq(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_ge(Antlr4Gen::SecLangParser::Op_geContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Ge(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_geo_lookup(Antlr4Gen::SecLangParser::Op_geo_lookupContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::GeoLookup(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_gt(Antlr4Gen::SecLangParser::Op_gtContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Gt(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_inspect_file(Antlr4Gen::SecLangParser::Op_inspect_fileContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::InspectFile(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_ip_match(Antlr4Gen::SecLangParser::Op_ip_matchContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::IpMatch(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_ip_match_f(Antlr4Gen::SecLangParser::Op_ip_match_fContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::IpMatchFromFile(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any
Visitor::visitOp_ip_match_from_file(Antlr4Gen::SecLangParser::Op_ip_match_from_fileContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::IpMatchFromFile(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_le(Antlr4Gen::SecLangParser::Op_leContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Le(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_lt(Antlr4Gen::SecLangParser::Op_ltContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Lt(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_no_match(Antlr4Gen::SecLangParser::Op_no_matchContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::NoMatch(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_pm(Antlr4Gen::SecLangParser::Op_pmContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Pm(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_pmf(Antlr4Gen::SecLangParser::Op_pmfContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::PmFromFile(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_pm_from_file(Antlr4Gen::SecLangParser::Op_pm_from_fileContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::PmFromFile(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_rbl(Antlr4Gen::SecLangParser::Op_rblContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Rbl(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_rsub(Antlr4Gen::SecLangParser::Op_rsubContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Rsub(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_rx(Antlr4Gen::SecLangParser::Op_rxContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Rx(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_rx_global(Antlr4Gen::SecLangParser::Op_rx_globalContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::RxGlobal(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_streq(Antlr4Gen::SecLangParser::Op_streqContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Streq(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_strmatch(Antlr4Gen::SecLangParser::Op_strmatchContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Strmatch(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any
Visitor::visitOp_unconditional_match(Antlr4Gen::SecLangParser::Op_unconditional_matchContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::UnconditionalMatch(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any
Visitor::visitOp_validate_byte_range(Antlr4Gen::SecLangParser::Op_validate_byte_rangeContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::ValidateByteRange(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_validate_dtd(Antlr4Gen::SecLangParser::Op_validate_dtdContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::ValidateDTD(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any
Visitor::visitOp_validate_schema(Antlr4Gen::SecLangParser::Op_validate_schemaContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::ValidateSchema(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_validate_url_encoding(
    Antlr4Gen::SecLangParser::Op_validate_url_encodingContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::ValidateUrlEncoding(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_validate_utf8_encoding(
    Antlr4Gen::SecLangParser::Op_validate_utf8_encodingContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::ValidateUtf8Encoding(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_verify_cc(Antlr4Gen::SecLangParser::Op_verify_ccContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::VerifyCC(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_verify_cpf(Antlr4Gen::SecLangParser::Op_verify_cpfContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::VerifyCPF(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_verify_ssn(Antlr4Gen::SecLangParser::Op_verify_ssnContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::VerifySSN(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_within(Antlr4Gen::SecLangParser::Op_withinContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(
      new Operator::Within(ctx->STRING()->getText(), ctx->NOT() != nullptr));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any Visitor::visitOp_rx_default(Antlr4Gen::SecLangParser::Op_rx_defaultContext* ctx) {
  std::unique_ptr<Operator::OperatorBase> op(new Operator::Rx(ctx->STRING()->getText(), false));
  (*current_rule_iter_)->setOperator(std::move(op));
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_meta_data_id(Antlr4Gen::SecLangParser::Action_meta_data_idContext* ctx) {
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

  (*current_rule_iter_)->id(id);
  if (visit_action_mode_ == VisitActionMode::SecRule) {
    parser_->setRuleIdIndex(current_rule_iter_);
  }
  return EMPTY_STRING;
};

std::any
Visitor::visitAction_meta_data_phase(Antlr4Gen::SecLangParser::Action_meta_data_phaseContext* ctx) {
  (*current_rule_iter_)->phase(::atoll(ctx->INT()->getText().c_str()));
  return EMPTY_STRING;
};

std::any
Visitor::visitAction_meta_data_msg(Antlr4Gen::SecLangParser::Action_meta_data_msgContext* ctx) {
  auto macro_ctx_array = ctx->action_meta_data_msg_value()->action_non_disruptive_setvar_macro();
  if (!macro_ctx_array.empty()) {
    std::any visit_result;
    try {
      std::vector<std::shared_ptr<Macro::MacroBase>> macros;
      for (auto& macro_ctx : macro_ctx_array) {
        visit_result = visitChildren(macro_ctx);
        macros.emplace_back(std::any_cast<std::shared_ptr<Macro::MacroBase>>(visit_result));
      }

      std::shared_ptr<Macro::MultiMacro> multi_macro = std::make_shared<Macro::MultiMacro>(
          ctx->action_meta_data_msg_value()->getText(), std::move(macros));

      (*current_rule_iter_)->msg(multi_macro);
    } catch (const std::bad_any_cast& ex) {
      RETURN_ERROR("Expect a macro object, but not.");
    }
  } else {
    (*current_rule_iter_)->msg(ctx->action_meta_data_msg_value()->STRING().front()->getText());
    if (visit_action_mode_ == VisitActionMode::SecRule) {
      parser_->setRuleMsgIndex(current_rule_iter_);
    }
  }

  return EMPTY_STRING;
};

std::any
Visitor::visitAction_meta_data_tag(Antlr4Gen::SecLangParser::Action_meta_data_tagContext* ctx) {
  auto& tags = (*current_rule_iter_)->tags();
  auto result = tags.emplace(ctx->STRING()->getText());
  if (visit_action_mode_ == VisitActionMode::SecRule) {
    if (result.second) {
      parser_->setRuleTagIndex(current_rule_iter_, *result.first);
    }
  }

  return EMPTY_STRING;
};

std::any
Visitor::visitAction_meta_data_ver(Antlr4Gen::SecLangParser::Action_meta_data_verContext* ctx) {
  (*current_rule_iter_)->ver(ctx->STRING()->getText());
  return EMPTY_STRING;
};

std::any
Visitor::visitAction_meta_data_rev(Antlr4Gen::SecLangParser::Action_meta_data_revContext* ctx) {
  (*current_rule_iter_)->rev(ctx->STRING()->getText());
  return EMPTY_STRING;
};

std::any Visitor::visitAction_meta_data_accuracy(
    Antlr4Gen::SecLangParser::Action_meta_data_accuracyContext* ctx) {
  (*current_rule_iter_)->accuracy(::atoll(ctx->LEVEL()->getText().c_str()));
  return EMPTY_STRING;
};

std::any Visitor::visitAction_meta_data_maturity(
    Antlr4Gen::SecLangParser::Action_meta_data_maturityContext* ctx) {
  (*current_rule_iter_)->maturity(::atoll(ctx->LEVEL()->getText().c_str()));
  return EMPTY_STRING;
};

std::any Visitor::visitAction_meta_data_severity_emergency(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_emergencyContext* ctx) {
  (*current_rule_iter_)->severity(SrSecurity::Rule::Severity::EMERGENCY);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_alert(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_alertContext* ctx) {
  (*current_rule_iter_)->severity(SrSecurity::Rule::Severity::ALERT);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_critical(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_criticalContext* ctx) {
  (*current_rule_iter_)->severity(SrSecurity::Rule::Severity::CRITICAL);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_error(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_errorContext* ctx) {
  (*current_rule_iter_)->severity(SrSecurity::Rule::Severity::ERROR);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_waring(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_waringContext* ctx) {
  (*current_rule_iter_)->severity(SrSecurity::Rule::Severity::WARNING);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_notice(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_noticeContext* ctx) {
  (*current_rule_iter_)->severity(SrSecurity::Rule::Severity::NOTICE);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_info(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_infoContext* ctx) {
  (*current_rule_iter_)->severity(SrSecurity::Rule::Severity::INFO);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_debug(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_debugContext* ctx) {
  (*current_rule_iter_)->severity(SrSecurity::Rule::Severity::DEBUG);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_meta_data_severity_number(
    Antlr4Gen::SecLangParser::Action_meta_data_severity_numberContext* ctx) {
  uint32_t serverity_level = ::atol(ctx->SEVERITY_LEVEL()->getText().c_str());
  (*current_rule_iter_)->severity(static_cast<SrSecurity::Rule::Severity>(serverity_level));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_setvar_create(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_createContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(std::make_unique<Action::SetVar>(
      ctx->VAR_NAME()->getText(), Common::Variant(), Action::SetVar::EvaluateType::Create));
  return EMPTY_STRING;
};

std::any Visitor::visitAction_non_disruptive_setvar_create_init(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_create_initContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();

  auto macro_ctx_array =
      ctx->action_non_disruptive_setvar_create_init_value()->action_non_disruptive_setvar_macro();
  if (!macro_ctx_array.empty()) {
    std::any visit_result;
    try {
      if (macro_ctx_array.size() == 1) {
        visit_result = visitChildren(macro_ctx_array.front());
        std::shared_ptr<Macro::MacroBase> macro =
            std::any_cast<std::shared_ptr<Macro::MacroBase>>(visit_result);
        actions.emplace_back(std::make_unique<Action::SetVar>(
            ctx->VAR_NAME()->getText(), macro, Action::SetVar::EvaluateType::CreateAndInit));
      } else {
        std::vector<std::shared_ptr<Macro::MacroBase>> macros;
        for (auto& macro_ctx : macro_ctx_array) {
          visit_result = visitChildren(macro_ctx);
          macros.emplace_back(std::any_cast<std::shared_ptr<Macro::MacroBase>>(visit_result));
        }

        std::shared_ptr<Macro::MultiMacro> multi_macro = std::make_shared<Macro::MultiMacro>(
            ctx->action_non_disruptive_setvar_create_init_value()->getText(), std::move(macros));

        actions.emplace_back(std::make_unique<Action::SetVar>(
            ctx->VAR_NAME()->getText(), multi_macro, Action::SetVar::EvaluateType::CreateAndInit));
      }

    } catch (const std::bad_any_cast& ex) {
      RETURN_ERROR("Expect a macro object, but not.");
    }
  } else {
    std::string value_string =
        ctx->action_non_disruptive_setvar_create_init_value()->VAR_VALUE().front()->getText();
    Common::Variant variant;
    if (std::all_of(value_string.begin(), value_string.end(), ::isdigit)) {
      variant = ::atoi(value_string.c_str());
    } else {
      variant = value_string;
    }
    actions.emplace_back(
        std::make_unique<Action::SetVar>(ctx->VAR_NAME()->getText(), std::move(variant),
                                         Action::SetVar::EvaluateType::CreateAndInit));
  }

  return EMPTY_STRING;
};

std::any Visitor::visitAction_non_disruptive_setvar_remove(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_removeContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(std::make_unique<Action::SetVar>(
      ctx->VAR_NAME()->getText(), Common::Variant(), Action::SetVar::EvaluateType::Remove));
  return EMPTY_STRING;
};

std::any Visitor::visitAction_non_disruptive_setvar_increase(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_increaseContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();

  if (ctx->action_non_disruptive_setvar_macro()) {
    std::any visit_result = visitChildren(ctx->action_non_disruptive_setvar_macro());
    try {
      std::shared_ptr<Macro::MacroBase> macro =
          std::any_cast<std::shared_ptr<Macro::MacroBase>>(visit_result);
      actions.emplace_back(std::make_unique<Action::SetVar>(
          ctx->VAR_NAME()->getText(), macro, Action::SetVar::EvaluateType::Increase));
    } catch (const std::bad_any_cast& ex) {
      RETURN_ERROR("Expect a macro object, but not.");
    }
  } else {
    Common::Variant variant(::atoi(ctx->VAR_VALUE()->getText().c_str()));
    actions.emplace_back(std::make_unique<Action::SetVar>(
        ctx->VAR_NAME()->getText(), std::move(variant), Action::SetVar::EvaluateType::Increase));
  }
  return EMPTY_STRING;
};

std::any Visitor::visitAction_non_disruptive_setvar_decrease(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_decreaseContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();

  if (ctx->action_non_disruptive_setvar_macro()) {
    std::any visit_result = visitChildren(ctx->action_non_disruptive_setvar_macro());
    try {
      std::shared_ptr<Macro::MacroBase> macro =
          std::any_cast<std::shared_ptr<Macro::MacroBase>>(visit_result);
      actions.emplace_back(std::make_unique<Action::SetVar>(
          ctx->VAR_NAME()->getText(), macro, Action::SetVar::EvaluateType::Decrease));
    } catch (const std::bad_any_cast& ex) {
      RETURN_ERROR("Expect a macro object, but not.");
    }
  } else {
    Common::Variant variant(::atoi(ctx->VAR_VALUE()->getText().c_str()));
    actions.emplace_back(std::make_unique<Action::SetVar>(
        ctx->VAR_NAME()->getText(), std::move(variant), Action::SetVar::EvaluateType::Decrease));
  }
  return EMPTY_STRING;
};

std::any Visitor::visitAction_non_disruptive_setvar_macro_tx(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_txContext* ctx) {
  std::shared_ptr<Macro::MacroBase> macro = std::make_shared<Macro::Tx>(ctx->STRING()->getText());
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_remote_addr(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_remote_addrContext* ctx) {
  RETURN_ERROR("Not implemented!");
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_user_id(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_user_idContext* ctx) {
  RETURN_ERROR("Not implemented!");
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_highest_severity(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_highest_severityContext* ctx) {
  RETURN_ERROR("Not implemented!");
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_matched_var(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_matched_varContext* ctx) {
  RETURN_ERROR("Not implemented!");
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_matched_var_name(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_matched_var_nameContext* ctx) {
  RETURN_ERROR("Not implemented!");
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_strict_error(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_multipart_strict_errorContext*
        ctx) {
  RETURN_ERROR("Not implemented!");
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_reqbody_processor_error(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_reqbody_processor_errorContext*
        ctx) {
  std::shared_ptr<Macro::MacroBase> macro = std::make_shared<Macro::MultipartStrictError>(
      MultipartStrictError::ErrorType::ReqbodyProcessorError);
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_boundary_quoted(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_multipart_boundary_quotedContext*
        ctx) {
  std::shared_ptr<Macro::MacroBase> macro = std::make_shared<Macro::MultipartStrictError>(
      MultipartStrictError::ErrorType::BoundaryQuoted);
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_boundary_whitespace(
    Antlr4Gen::SecLangParser::
        Action_non_disruptive_setvar_macro_multipart_boundary_whitespaceContext* ctx) {
  std::shared_ptr<Macro::MacroBase> macro = std::make_shared<Macro::MultipartStrictError>(
      MultipartStrictError::ErrorType::BoundaryWhitespace);
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_data_before(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_multipart_data_beforeContext*
        ctx) {
  std::shared_ptr<Macro::MacroBase> macro =
      std::make_shared<Macro::MultipartStrictError>(MultipartStrictError::ErrorType::DataBefore);
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_data_after(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_multipart_data_afterContext* ctx) {
  std::shared_ptr<Macro::MacroBase> macro =
      std::make_shared<Macro::MultipartStrictError>(MultipartStrictError::ErrorType::DataAfter);
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_header_folding(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_multipart_header_foldingContext*
        ctx) {
  std::shared_ptr<Macro::MacroBase> macro =
      std::make_shared<Macro::MultipartStrictError>(MultipartStrictError::ErrorType::HeaderFolding);
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_lf_line(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_multipart_lf_lineContext* ctx) {
  std::shared_ptr<Macro::MacroBase> macro =
      std::make_shared<Macro::MultipartStrictError>(MultipartStrictError::ErrorType::LfLine);
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_missing_semicolon(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_multipart_missing_semicolonContext*
        ctx) {
  std::shared_ptr<Macro::MacroBase> macro = std::make_shared<Macro::MultipartStrictError>(
      MultipartStrictError::ErrorType::MissingSemicolon);
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_invalid_quoting(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_multipart_invalid_quotingContext*
        ctx) {
  std::shared_ptr<Macro::MacroBase> macro = std::make_shared<Macro::MultipartStrictError>(
      MultipartStrictError::ErrorType::InvalidQuoting);
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_invalid_part(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_multipart_invalid_partContext*
        ctx) {
  std::shared_ptr<Macro::MacroBase> macro =
      std::make_shared<Macro::MultipartStrictError>(MultipartStrictError::ErrorType::InvalidPart);
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_invalid_header_folding(
    Antlr4Gen::SecLangParser::
        Action_non_disruptive_setvar_macro_multipart_invalid_header_foldingContext* ctx) {
  std::shared_ptr<Macro::MacroBase> macro = std::make_shared<Macro::MultipartStrictError>(
      MultipartStrictError::ErrorType::InvalidHeaderFolding);
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_multipart_file_limit_exceeded(
    Antlr4Gen::SecLangParser::
        Action_non_disruptive_setvar_macro_multipart_file_limit_exceededContext* ctx) {
  std::shared_ptr<Macro::MacroBase> macro = std::make_shared<Macro::MultipartStrictError>(
      MultipartStrictError::ErrorType::FileLimitExceeded);
  return macro;
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_rule(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_ruleContext* ctx) {
  RETURN_ERROR("Not implemented!");
}

std::any Visitor::visitAction_non_disruptive_setvar_macro_session(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_macro_sessionContext* ctx) {
  RETURN_ERROR("Not implemented!");
}

std::any Visitor::visitAction_non_disruptive_setenv(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setenvContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();

  if (ctx->action_non_disruptive_setvar_macro()) {
    std::any visit_result = visitChildren(ctx->action_non_disruptive_setvar_macro());
    try {
      std::shared_ptr<Macro::MacroBase> macro =
          std::any_cast<std::shared_ptr<Macro::MacroBase>>(visit_result);
      actions.emplace_back(std::make_unique<Action::SetEnv>(ctx->VAR_NAME()->getText(), macro));
    } catch (const std::bad_any_cast& ex) {
      RETURN_ERROR("Expect a macro object, but not.");
    }
  } else {
    actions.emplace_back(
        std::make_unique<Action::SetEnv>(ctx->VAR_NAME()->getText(), ctx->VAR_VALUE()->getText()));
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_setuid(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setuidContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();

  if (ctx->action_non_disruptive_setvar_macro()) {
    std::any visit_result = visitChildren(ctx->action_non_disruptive_setvar_macro());
    try {
      std::shared_ptr<Macro::MacroBase> macro =
          std::any_cast<std::shared_ptr<Macro::MacroBase>>(visit_result);
      actions.emplace_back(std::make_unique<Action::SetUid>(macro));
    } catch (const std::bad_any_cast& ex) {
      RETURN_ERROR("Expect a macro object, but not.");
    }
  } else {
    actions.emplace_back(std::make_unique<Action::SetUid>(ctx->STRING()->getText()));
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_setrsc(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setrscContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();

  if (ctx->action_non_disruptive_setvar_macro()) {
    std::any visit_result = visitChildren(ctx->action_non_disruptive_setvar_macro());
    try {
      std::shared_ptr<Macro::MacroBase> macro =
          std::any_cast<std::shared_ptr<Macro::MacroBase>>(visit_result);
      actions.emplace_back(std::make_unique<Action::SetRsc>(macro));
    } catch (const std::bad_any_cast& ex) {
      RETURN_ERROR("Expect a macro object, but not.");
    }
  } else {
    actions.emplace_back(std::make_unique<Action::SetRsc>(ctx->STRING()->getText()));
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_setsid(
    Antlr4Gen::SecLangParser::Action_non_disruptive_setsidContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();

  if (ctx->action_non_disruptive_setvar_macro()) {
    std::any visit_result = visitChildren(ctx->action_non_disruptive_setvar_macro());
    try {
      std::shared_ptr<Macro::MacroBase> macro =
          std::any_cast<std::shared_ptr<Macro::MacroBase>>(visit_result);
      actions.emplace_back(std::make_unique<Action::SetSid>(macro));
    } catch (const std::bad_any_cast& ex) {
      RETURN_ERROR("Expect a macro object, but not.");
    }
  } else {
    actions.emplace_back(std::make_unique<Action::SetSid>(ctx->STRING()->getText()));
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_base64_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_base64_decodeContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Base64Decode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_sql_hex_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_sql_hex_decodeContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::SqlHexDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_base64_decode_ext(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_base64_decode_extContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Base64DecodeExt>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_base64_encode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_base64_encodeContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Base64Encode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_cmdline(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_cmdlineContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::CmdLine>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_compress_whitespace(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_compress_whitespaceContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::CompressWhiteSpace>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_css_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_css_decodeContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::CssDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_escape_seq_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_escape_seq_decodeContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::EscapeSeqDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_hex_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_hex_decodeContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::HexDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_hex_encode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_hex_encodeContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::HexEncode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_html_entity_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_html_entity_decodeContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::HtmlEntityDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_js_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_js_decodeContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::JsDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_length(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_lengthContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Length>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_lowercase(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_lowercaseContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::LowerCase>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_md5(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_md5Context* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Md5>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_none(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_noneContext* ctx) {
  (*current_rule_iter_)->isIgnoreDefaultTransform(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_normalise_path(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_normalise_pathContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::NormalisePath>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_normalize_path(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_normalize_pathContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::NormalizePath>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_normalise_pathwin(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_normalise_pathwinContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::NormalisePathWin>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_normalize_pathwin(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_normalize_pathwinContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::NormalizePathWin>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_parity_even_7bit(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_parity_even_7bitContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::ParityEven7Bit>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_parity_odd_7bit(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_parity_odd_7bitContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::ParityOdd7Bit>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_parity_zero_7bit(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_parity_zero_7bitContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::ParityZero7Bit>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_remove_nulls(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_remove_nullsContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::RemoveNulls>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_remove_whitespace(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_remove_whitespaceContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::RemoveWhitespace>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_replace_comments(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_replace_commentsContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::ReplaceComments>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_remove_commentschar(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_remove_commentscharContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::RemoveCommentChar>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_remove_comments(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_remove_commentsContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::RemoveComments>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_replace_nulls(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_replace_nullsContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::ReplaceNulls>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_url_decode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_url_decodeContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::UrlDecode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_uppercase(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_uppercaseContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::UpperCase>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_url_decode_uni(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_url_decode_uniContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::UrlDecodeUni>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_url_encode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_url_encodeContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::UrlEncode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_utf8_to_unicode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_utf8_to_unicodeContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Utf8ToUnicode>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_sha1(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_sha1Context* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Sha1>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_trim_left(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_trim_leftContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::TrimLeft>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_trim_right(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_trim_rightContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::TrimRight>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_t_trim(
    Antlr4Gen::SecLangParser::Action_non_disruptive_t_trimContext* ctx) {
  auto& transforms = (*current_rule_iter_)->transforms();
  transforms.emplace_back(std::make_unique<Transformation::Trim>());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_audit_engine(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_audit_engineContext* ctx) {
  using Option = SrSecurity::AuditLogConfig::AuditEngine;
  Option option = Option::Off;

  std::string option_str = ctx->AUDIT_ENGINE()->getText();
  if (option_str == "On") {
    option = Option::On;
  } else if (option_str == "RelevantOnly") {
    option = Option::RelevantOnly;
  }

  auto& actions = (*current_rule_iter_)->actions();

  actions.emplace_back(std::make_unique<Action::Ctl>(Action::Ctl::CtlType::AuditEngine, option));

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_audit_log_parts(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_audit_log_partsContext* ctx) {
  std::string parts = ctx->AUDIT_PARTS()->getText();

  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(std::make_unique<Action::Ctl>(Action::Ctl::CtlType::AuditLogParts, parts));

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_force_request_body_variable(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_force_request_body_variableContext* ctx) {
  // Not implemented in ModSecurity v3 (REQUEST_BODY is always populated in v3)
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_request_body_access(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_accessContext* ctx) {
  using Option = SrSecurity::EngineConfig::Option;
  Option option = Option::Off;

  std::string option_str = ctx->OPTION()->getText();
  if (option_str == "On") {
    option = Option::On;
  }

  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(
      std::make_unique<Action::Ctl>(Action::Ctl::CtlType::RequestBodyAccess, option));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_request_body_processor_url_encode(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_processor_url_encodeContext*
        ctx) {
  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(std::make_unique<Action::Ctl>(Action::Ctl::CtlType::RequestBodyProcessor,
                                                     BodyProcessorType::UrlEncoded));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_request_body_processor_multi_part(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_processor_multi_partContext*
        ctx) {
  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(std::make_unique<Action::Ctl>(Action::Ctl::CtlType::RequestBodyProcessor,
                                                     BodyProcessorType::MultiPart));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_request_body_processor_xml(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_processor_xmlContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(std::make_unique<Action::Ctl>(Action::Ctl::CtlType::RequestBodyProcessor,
                                                     BodyProcessorType::Xml));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_request_body_processor_json(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_processor_jsonContext* ctx) {
  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(std::make_unique<Action::Ctl>(Action::Ctl::CtlType::RequestBodyProcessor,
                                                     BodyProcessorType::Json));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_rule_engine(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_engineContext* ctx) {
  SrSecurity::EngineConfig::Option option = optionStr2EnumValue(ctx->OPTION()->getText());
  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(std::make_unique<Action::Ctl>(Action::Ctl::CtlType::RuleEngine, option));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_rule_remove_by_id(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_remove_by_idContext* ctx) {
  if (ctx->INT()) {
    uint64_t id = ::atoll(ctx->INT()->getText().c_str());
    auto& actions = (*current_rule_iter_)->actions();
    actions.emplace_back(std::make_unique<Action::Ctl>(Action::Ctl::CtlType::RuleRemoveById, id));
  } else {
    std::string id_range_str = ctx->INT_RANGE()->getText();
    auto pos = id_range_str.find('-');
    if (pos != std::string::npos) {
      uint64_t first = ::atoll(id_range_str.substr(0, pos).c_str());
      uint64_t last = ::atoll(id_range_str.substr(pos + 1).c_str());
      auto& actions = (*current_rule_iter_)->actions();
      actions.emplace_back(std::make_unique<Action::Ctl>(Action::Ctl::CtlType::RuleRemoveByIdRange,
                                                         std::make_pair(first, last)));
    }
  }

  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_rule_remove_by_tag(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_remove_by_tagContext* ctx) {
  std::string tag = ctx->STRING()->getText();
  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(
      std::make_unique<Action::Ctl>(Action::Ctl::CtlType::RuleRemoveByTag, std::move(tag)));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_rule_remove_target_by_id(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_remove_target_by_idContext* ctx) {
  auto old_visit_variable_mode = visit_variable_mode_;

  uint64_t id = ::atoll(ctx->INT()->getText().c_str());

  auto variables = ctx->variables()->variable();
  std::any visit_result;
  try {
    std::vector<std::shared_ptr<Variable::VariableBase>> variable_objects;

    visit_variable_mode_ = VisitVariableMode::Ctl;
    for (auto variable : variables) {
      visit_result = visitChildren(variable);
      auto var_obj = std::any_cast<std::shared_ptr<Variable::VariableBase>>(visit_result);
      variable_objects.emplace_back(var_obj);
    }

    auto& actions = (*current_rule_iter_)->actions();
    actions.emplace_back(
        std::make_unique<Action::Ctl>(Action::Ctl::CtlType::RuleRemoveTargetById,
                                      std::make_pair(id, std::move(variable_objects))));
  } catch (const std::bad_any_cast& ex) {
    visit_variable_mode_ = old_visit_variable_mode;
    return std::format("Expect a variable object, but not. return: {}",
                       std::any_cast<std::string>(visit_result));
  }

  visit_variable_mode_ = old_visit_variable_mode;
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_ctl_rule_remove_target_by_tag(
    Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_remove_target_by_tagContext* ctx) {
  auto old_visit_variable_mode = visit_variable_mode_;

  std::string tag = ctx->STRING()->getText();

  auto variables = ctx->variables()->variable();
  std::any visit_result;
  try {
    std::vector<std::shared_ptr<Variable::VariableBase>> variable_objects;

    visit_variable_mode_ = VisitVariableMode::Ctl;
    for (auto variable : variables) {
      visit_result = visitChildren(variable);
      auto var_obj = std::any_cast<std::shared_ptr<Variable::VariableBase>>(visit_result);
      variable_objects.emplace_back(var_obj);
    }

    auto& actions = (*current_rule_iter_)->actions();
    actions.emplace_back(
        std::make_unique<Action::Ctl>(Action::Ctl::CtlType::RuleRemoveTargetByTag,
                                      std::make_pair(std::move(tag), std::move(variable_objects))));
  } catch (const std::bad_any_cast& ex) {
    visit_variable_mode_ = old_visit_variable_mode;
    return std::format("Expect a variable object, but not. return: {}",
                       std::any_cast<std::string>(visit_result));
  }

  visit_variable_mode_ = old_visit_variable_mode;
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_audit_log(
    Antlr4Gen::SecLangParser::Action_non_disruptive_audit_logContext* ctx) {
  (*current_rule_iter_)->auditLog(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_log(
    Antlr4Gen::SecLangParser::Action_non_disruptive_logContext* ctx) {
  (*current_rule_iter_)->log(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_no_audit_log(
    Antlr4Gen::SecLangParser::Action_non_disruptive_no_audit_logContext* ctx) {
  (*current_rule_iter_)->auditLog(false);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_no_log(
    Antlr4Gen::SecLangParser::Action_non_disruptive_no_logContext* ctx) {
  (*current_rule_iter_)->log(false);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_capture(
    Antlr4Gen::SecLangParser::Action_non_disruptive_captureContext* ctx) {
  (*current_rule_iter_)->capture(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_multi_match(
    Antlr4Gen::SecLangParser::Action_non_disruptive_multi_matchContext* ctx) {
  (*current_rule_iter_)->multiMatch(true);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_non_disruptive_initcol(
    Antlr4Gen::SecLangParser::Action_non_disruptive_initcolContext* ctx) {
  std::string name = ctx->STRING(0)->getText();
  std::string value = ctx->STRING(1)->getText();
  auto& actions = (*current_rule_iter_)->actions();
  actions.emplace_back(std::make_unique<Action::InitCol>(std::move(name), std::move(value)));
  return EMPTY_STRING;
}

std::any Visitor::visitAction_disruptive_allow(
    Antlr4Gen::SecLangParser::Action_disruptive_allowContext* ctx) {
  (*current_rule_iter_)->disruptive(Rule::Disruptive::ALLOW);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_disruptive_block(
    Antlr4Gen::SecLangParser::Action_disruptive_blockContext* ctx) {
  (*current_rule_iter_)->disruptive(Rule::Disruptive::BLOCK);
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_disruptive_deny(Antlr4Gen::SecLangParser::Action_disruptive_denyContext* ctx) {
  (*current_rule_iter_)->disruptive(Rule::Disruptive::DENY);
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_disruptive_drop(Antlr4Gen::SecLangParser::Action_disruptive_dropContext* ctx) {
  (*current_rule_iter_)->disruptive(Rule::Disruptive::DROP);
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_disruptive_pass(Antlr4Gen::SecLangParser::Action_disruptive_passContext* ctx) {
  (*current_rule_iter_)->disruptive(Rule::Disruptive::PASS);
  return EMPTY_STRING;
}

std::any Visitor::visitAction_disruptive_redirect(
    Antlr4Gen::SecLangParser::Action_disruptive_redirectContext* ctx) {
  (*current_rule_iter_)->disruptive(Rule::Disruptive::REDIRECT);
  (*current_rule_iter_)->redirect(ctx->STRING()->getText());
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_data_status(Antlr4Gen::SecLangParser::Action_data_statusContext* ctx) {
  (*current_rule_iter_)->status(ctx->INT()->getText());
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_data_xml_ns(Antlr4Gen::SecLangParser::Action_data_xml_nsContext* ctx) {
  (*current_rule_iter_)->xmlns(ctx->STRING()->getText());
  return EMPTY_STRING;
}

std::any Visitor::visitAction_flow_chain(Antlr4Gen::SecLangParser::Action_flow_chainContext* ctx) {
  chain_ = true;
  return EMPTY_STRING;
}

std::any Visitor::visitAction_flow_skip(Antlr4Gen::SecLangParser::Action_flow_skipContext* ctx) {
  (*current_rule_iter_)->skip(::atol(ctx->INT()->getText().c_str()));
  return EMPTY_STRING;
}

std::any
Visitor::visitAction_flow_skip_after(Antlr4Gen::SecLangParser::Action_flow_skip_afterContext* ctx) {
  (*current_rule_iter_)->skipAfter(ctx->STRING()->getText());
  return EMPTY_STRING;
}

std::any Visitor::visitSec_audit_engine(Antlr4Gen::SecLangParser::Sec_audit_engineContext* ctx) {
  using Option = SrSecurity::AuditLogConfig::AuditEngine;
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
  using Format = SrSecurity::AuditLogConfig::AuditFormat;
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
  using Type = SrSecurity::AuditLogConfig::AuditLogType;
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
} // namespace SrSecurity::Antlr4