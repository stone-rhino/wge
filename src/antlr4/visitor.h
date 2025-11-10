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
#pragma once

#include "antlr4_gen/SecLangParserBaseVisitor.h"
#include "parser.h"

#include "../common/empty_string.h"
#include "../macro/macro_include.h"
#include "../operator/pm_from_file.h"

#define RETURN_ERROR(msg)                                                                          \
  should_visit_next_child_ = false;                                                                \
  return std::format("[{}:{}:{}] {}", parser_->currLoadFile(), ctx->getStart()->getLine(),         \
                     ctx->getStart()->getCharPositionInLine(), msg);

namespace Wge::Antlr4 {
class Visitor : public Antlr4Gen::SecLangParserBaseVisitor {
public:
  Visitor(Parser* parser) : parser_(parser) {}

public:
  bool shouldVisitNextChild(antlr4::tree::ParseTree* /*node*/,
                            const std::any& /*currentResult*/) override {
    return should_visit_next_child_;
  }

public:
  std::any visitInclude(Antlr4Gen::SecLangParser::IncludeContext* ctx) override;

  // Engine configurations
public:
  std::any visitSec_reqeust_body_access(
      Antlr4Gen::SecLangParser::Sec_reqeust_body_accessContext* ctx) override;

  std::any visitSec_response_body_mime_type(
      Antlr4Gen::SecLangParser::Sec_response_body_mime_typeContext* ctx) override;

  std::any visitSec_response_body_mime_type_clear(
      Antlr4Gen::SecLangParser::Sec_response_body_mime_type_clearContext* ctx) override;

  std::any visitSec_response_body_access(
      Antlr4Gen::SecLangParser::Sec_response_body_accessContext* ctx) override;

  std::any visitSec_rule_engine(Antlr4Gen::SecLangParser::Sec_rule_engineContext* ctx) override;

  std::any visitSec_tmp_save_uploaded_files(
      Antlr4Gen::SecLangParser::Sec_tmp_save_uploaded_filesContext* ctx) override;

  std::any
  visitSec_upload_file_limit(Antlr4Gen::SecLangParser::Sec_upload_file_limitContext* ctx) override;

  std::any
  visitSec_upload_keep_files(Antlr4Gen::SecLangParser::Sec_upload_keep_filesContext* ctx) override;

  std::any visitSec_xml_external_entity(
      Antlr4Gen::SecLangParser::Sec_xml_external_entityContext* ctx) override;

  std::any visitSec_request_body_limit(
      Antlr4Gen::SecLangParser::Sec_request_body_limitContext* ctx) override;

  std::any visitSec_request_body_no_files_limit(
      Antlr4Gen::SecLangParser::Sec_request_body_no_files_limitContext* ctx) override;

  std::any visitSec_request_body_json_depth_limit(
      Antlr4Gen::SecLangParser::Sec_request_body_json_depth_limitContext* ctx) override;

  std::any visitSec_request_body_action(
      Antlr4Gen::SecLangParser::Sec_request_body_actionContext* ctx) override;

  std::any visitSec_response_body_limit(
      Antlr4Gen::SecLangParser::Sec_response_body_limitContext* ctx) override;

  std::any visitSec_response_body_action(
      Antlr4Gen::SecLangParser::Sec_response_body_actionContext* ctx) override;

  std::any visitSec_status_engine(Antlr4Gen::SecLangParser::Sec_status_engineContext* ctx) override;

  std::any visitSec_tmp_dir(Antlr4Gen::SecLangParser::Sec_tmp_dirContext* ctx) override;

  std::any visitSec_data_dir(Antlr4Gen::SecLangParser::Sec_data_dirContext* ctx) override;

  std::any visitSec_cookie_format(Antlr4Gen::SecLangParser::Sec_cookie_formatContext* ctx) override;

  std::any
  visitSec_arguments_limit(Antlr4Gen::SecLangParser::Sec_arguments_limitContext* ctx) override;

  std::any visitSec_argument_separator(
      Antlr4Gen::SecLangParser::Sec_argument_separatorContext* ctx) override;

  std::any
  visitSec_unicode_map_file(Antlr4Gen::SecLangParser::Sec_unicode_map_fileContext* ctx) override;

  std::any visitSec_parse_xml_into_args(
      Antlr4Gen::SecLangParser::Sec_parse_xml_into_argsContext* ctx) override;

  std::any
  visitSec_pcre_match_limit(Antlr4Gen::SecLangParser::Sec_pcre_match_limitContext* ctx) override;

  std::any visitSec_pcre_match_limit_recursion(
      Antlr4Gen::SecLangParser::Sec_pcre_match_limit_recursionContext* ctx) override;

  std::any visitSec_collection_timeout(
      Antlr4Gen::SecLangParser::Sec_collection_timeoutContext* ctx) override;

  std::any
  visitSec_pmf_serialize_dir(Antlr4Gen::SecLangParser::Sec_pmf_serialize_dirContext* ctx) override;

  // Engine action
public:
  std::any visitSec_action(Antlr4Gen::SecLangParser::Sec_actionContext* ctx) override;
  std::any
  visitSec_default_action(Antlr4Gen::SecLangParser::Sec_default_actionContext* ctx) override;

  // Rule directives
public:
  std::any visitSec_rule(Antlr4Gen::SecLangParser::Sec_ruleContext* ctx) override;

  std::any
  visitSec_rule_remove_by_id(Antlr4Gen::SecLangParser::Sec_rule_remove_by_idContext* ctx) override;

  std::any visitSec_rule_remove_by_msg(
      Antlr4Gen::SecLangParser::Sec_rule_remove_by_msgContext* ctx) override;

  std::any visitSec_rule_remove_by_tag(
      Antlr4Gen::SecLangParser::Sec_rule_remove_by_tagContext* ctx) override;

  std::any visitSec_rule_update_action_by_id(
      Antlr4Gen::SecLangParser::Sec_rule_update_action_by_idContext* ctx) override;

  std::any visitSec_rule_update_target_by_id(
      Antlr4Gen::SecLangParser::Sec_rule_update_target_by_idContext* ctx) override;

  std::any visitSec_rule_update_target_by_msg(
      Antlr4Gen::SecLangParser::Sec_rule_update_target_by_msgContext* ctx) override;

  std::any visitSec_rule_update_target_by_tag(
      Antlr4Gen::SecLangParser::Sec_rule_update_target_by_tagContext* ctx) override;

  std::any visitSec_marker(Antlr4Gen::SecLangParser::Sec_markerContext* ctx) override;

  // SecRule variables
public:
  std::any visitVariable_args(Antlr4Gen::SecLangParser::Variable_argsContext* ctx) override;

  std::any visitVariable_args_combined_size(
      Antlr4Gen::SecLangParser::Variable_args_combined_sizeContext* ctx) override;

  std::any visitVariable_args_get(Antlr4Gen::SecLangParser::Variable_args_getContext* ctx) override;

  std::any visitVariable_args_get_names(
      Antlr4Gen::SecLangParser::Variable_args_get_namesContext* ctx) override;

  std::any
  visitVariable_args_names(Antlr4Gen::SecLangParser::Variable_args_namesContext* ctx) override;

  std::any
  visitVariable_args_post(Antlr4Gen::SecLangParser::Variable_args_postContext* ctx) override;

  std::any visitVariable_args_post_names(
      Antlr4Gen::SecLangParser::Variable_args_post_namesContext* ctx) override;

  std::any
  visitVariable_auth_type(Antlr4Gen::SecLangParser::Variable_auth_typeContext* ctx) override;

  std::any visitVariable_duration(Antlr4Gen::SecLangParser::Variable_durationContext* ctx) override;

  std::any visitVariable_env(Antlr4Gen::SecLangParser::Variable_envContext* ctx) override;

  std::any visitVariable_files(Antlr4Gen::SecLangParser::Variable_filesContext* ctx) override;

  std::any visitVariable_files_combined_size(
      Antlr4Gen::SecLangParser::Variable_files_combined_sizeContext* ctx) override;

  std::any
  visitVariable_files_names(Antlr4Gen::SecLangParser::Variable_files_namesContext* ctx) override;

  std::any
  visitVariable_full_request(Antlr4Gen::SecLangParser::Variable_full_requestContext* ctx) override;

  std::any visitVariable_full_request_length(
      Antlr4Gen::SecLangParser::Variable_full_request_lengthContext* ctx) override;

  std::any
  visitVariable_files_sizes(Antlr4Gen::SecLangParser::Variable_files_sizesContext* ctx) override;

  std::any visitVariable_files_tmpnames(
      Antlr4Gen::SecLangParser::Variable_files_tmpnamesContext* ctx) override;

  std::any visitVariable_files_tmp_content(
      Antlr4Gen::SecLangParser::Variable_files_tmp_contentContext* ctx) override;

  std::any visitVariable_geo(Antlr4Gen::SecLangParser::Variable_geoContext* ctx) override;

  std::any visitVariable_highest_severity(
      Antlr4Gen::SecLangParser::Variable_highest_severityContext* ctx) override;

  std::any visitVariable_inbound_data_error(
      Antlr4Gen::SecLangParser::Variable_inbound_data_errorContext* ctx) override;

  std::any
  visitVariable_matched_var(Antlr4Gen::SecLangParser::Variable_matched_varContext* ctx) override;

  std::any
  visitVariable_matched_vars(Antlr4Gen::SecLangParser::Variable_matched_varsContext* ctx) override;

  std::any visitVariable_matched_var_name(
      Antlr4Gen::SecLangParser::Variable_matched_var_nameContext* ctx) override;

  std::any visitVariable_matched_vars_names(
      Antlr4Gen::SecLangParser::Variable_matched_vars_namesContext* ctx) override;

  std::any
  visitVariable_modsec_build(Antlr4Gen::SecLangParser::Variable_modsec_buildContext* ctx) override;

  std::any visitVariable_msc_pcre_limits_exceeded(
      Antlr4Gen::SecLangParser::Variable_msc_pcre_limits_exceededContext* ctx) override;

  std::any visitVariable_multipart_crlf_lf_lines(
      Antlr4Gen::SecLangParser::Variable_multipart_crlf_lf_linesContext* ctx) override;

  std::any visitVariable_multipart_filename(
      Antlr4Gen::SecLangParser::Variable_multipart_filenameContext* ctx) override;

  std::any visitVariable_multipart_name(
      Antlr4Gen::SecLangParser::Variable_multipart_nameContext* ctx) override;

  std::any visitVariable_multipart_part_headers(
      Antlr4Gen::SecLangParser::Variable_multipart_part_headersContext* ctx) override;

  std::any visitVariable_multipart_strict_error(
      Antlr4Gen::SecLangParser::Variable_multipart_strict_errorContext* ctx) override;

  std::any visitVariable_multipart_unmatched_boundary(
      Antlr4Gen::SecLangParser::Variable_multipart_unmatched_boundaryContext* ctx) override;

  std::any visitVariable_outbound_data_error(
      Antlr4Gen::SecLangParser::Variable_outbound_data_errorContext* ctx) override;

  std::any
  visitVariable_path_info(Antlr4Gen::SecLangParser::Variable_path_infoContext* ctx) override;

  std::any
  visitVariable_query_string(Antlr4Gen::SecLangParser::Variable_query_stringContext* ctx) override;

  std::any
  visitVariable_remote_addr(Antlr4Gen::SecLangParser::Variable_remote_addrContext* ctx) override;

  std::any
  visitVariable_remote_host(Antlr4Gen::SecLangParser::Variable_remote_hostContext* ctx) override;

  std::any
  visitVariable_remote_port(Antlr4Gen::SecLangParser::Variable_remote_portContext* ctx) override;

  std::any
  visitVariable_remote_user(Antlr4Gen::SecLangParser::Variable_remote_userContext* ctx) override;

  std::any visitVariable_reqbody_error(
      Antlr4Gen::SecLangParser::Variable_reqbody_errorContext* ctx) override;

  std::any visitVariable_reqbody_error_msg(
      Antlr4Gen::SecLangParser::Variable_reqbody_error_msgContext* ctx) override;

  std::any visitVariable_reqbody_processor(
      Antlr4Gen::SecLangParser::Variable_reqbody_processorContext* ctx) override;

  std::any visitVariable_request_basename(
      Antlr4Gen::SecLangParser::Variable_request_basenameContext* ctx) override;

  std::any
  visitVariable_request_body(Antlr4Gen::SecLangParser::Variable_request_bodyContext* ctx) override;

  std::any visitVariable_request_body_length(
      Antlr4Gen::SecLangParser::Variable_request_body_lengthContext* ctx) override;

  std::any visitVariable_request_cookies(
      Antlr4Gen::SecLangParser::Variable_request_cookiesContext* ctx) override;

  std::any visitVariable_request_cookies_names(
      Antlr4Gen::SecLangParser::Variable_request_cookies_namesContext* ctx) override;

  std::any visitVariable_request_filename(
      Antlr4Gen::SecLangParser::Variable_request_filenameContext* ctx) override;

  std::any visitVariable_request_headers(
      Antlr4Gen::SecLangParser::Variable_request_headersContext* ctx) override;

  std::any visitVariable_request_headers_names(
      Antlr4Gen::SecLangParser::Variable_request_headers_namesContext* ctx) override;

  std::any
  visitVariable_request_line(Antlr4Gen::SecLangParser::Variable_request_lineContext* ctx) override;

  std::any visitVariable_request_method(
      Antlr4Gen::SecLangParser::Variable_request_methodContext* ctx) override;

  std::any visitVariable_request_protocol(
      Antlr4Gen::SecLangParser::Variable_request_protocolContext* ctx) override;

  std::any
  visitVariable_request_uri(Antlr4Gen::SecLangParser::Variable_request_uriContext* ctx) override;

  std::any visitVariable_request_uri_raw(
      Antlr4Gen::SecLangParser::Variable_request_uri_rawContext* ctx) override;

  std::any visitVariable_response_body(
      Antlr4Gen::SecLangParser::Variable_response_bodyContext* ctx) override;

  std::any visitVariable_response_content_length(
      Antlr4Gen::SecLangParser::Variable_response_content_lengthContext* ctx) override;

  std::any visitVariable_response_content_type(
      Antlr4Gen::SecLangParser::Variable_response_content_typeContext* ctx) override;

  std::any visitVariable_response_headers(
      Antlr4Gen::SecLangParser::Variable_response_headersContext* ctx) override;

  std::any visitVariable_response_headers_names(
      Antlr4Gen::SecLangParser::Variable_response_headers_namesContext* ctx) override;

  std::any visitVariable_response_protocol(
      Antlr4Gen::SecLangParser::Variable_response_protocolContext* ctx) override;

  std::any visitVariable_response_status(
      Antlr4Gen::SecLangParser::Variable_response_statusContext* ctx) override;

  std::any visitVariable_rule(Antlr4Gen::SecLangParser::Variable_ruleContext* ctx) override;

  std::any
  visitVariable_server_addr(Antlr4Gen::SecLangParser::Variable_server_addrContext* ctx) override;

  std::any
  visitVariable_server_name(Antlr4Gen::SecLangParser::Variable_server_nameContext* ctx) override;

  std::any
  visitVariable_server_port(Antlr4Gen::SecLangParser::Variable_server_portContext* ctx) override;

  std::any visitVariable_session(Antlr4Gen::SecLangParser::Variable_sessionContext* ctx) override;

  std::any
  visitVariable_sessionid(Antlr4Gen::SecLangParser::Variable_sessionidContext* ctx) override;

  std::any
  visitVariable_status_line(Antlr4Gen::SecLangParser::Variable_status_lineContext* ctx) override;

  std::any visitVariable_time(Antlr4Gen::SecLangParser::Variable_timeContext* ctx) override;

  std::any visitVariable_time_day(Antlr4Gen::SecLangParser::Variable_time_dayContext* ctx) override;

  std::any
  visitVariable_time_epoch(Antlr4Gen::SecLangParser::Variable_time_epochContext* ctx) override;

  std::any
  visitVariable_time_hour(Antlr4Gen::SecLangParser::Variable_time_hourContext* ctx) override;

  std::any visitVariable_time_min(Antlr4Gen::SecLangParser::Variable_time_minContext* ctx) override;

  std::any visitVariable_time_mon(Antlr4Gen::SecLangParser::Variable_time_monContext* ctx) override;

  std::any visitVariable_time_sec(Antlr4Gen::SecLangParser::Variable_time_secContext* ctx) override;

  std::any
  visitVariable_time_wday(Antlr4Gen::SecLangParser::Variable_time_wdayContext* ctx) override;

  std::any
  visitVariable_time_year(Antlr4Gen::SecLangParser::Variable_time_yearContext* ctx) override;

  std::any visitVariable_tx(Antlr4Gen::SecLangParser::Variable_txContext* ctx) override;

  std::any
  visitVariable_unique_id(Antlr4Gen::SecLangParser::Variable_unique_idContext* ctx) override;

  std::any visitVariable_urlencoded_error(
      Antlr4Gen::SecLangParser::Variable_urlencoded_errorContext* ctx) override;

  std::any visitVariable_userid(Antlr4Gen::SecLangParser::Variable_useridContext* ctx) override;

  std::any visitVariable_webappid(Antlr4Gen::SecLangParser::Variable_webappidContext* ctx) override;

  std::any visitVariable_xml(Antlr4Gen::SecLangParser::Variable_xmlContext* ctx) override;

  std::any visitVariable_reqbody_processor_error(
      Antlr4Gen::SecLangParser::Variable_reqbody_processor_errorContext* ctx) override;

  std::any visitVariable_multipart_boundary_quoted(
      Antlr4Gen::SecLangParser::Variable_multipart_boundary_quotedContext* ctx) override;

  std::any visitVariable_multipart_boundary_whitespace(
      Antlr4Gen::SecLangParser::Variable_multipart_boundary_whitespaceContext* ctx) override;

  std::any visitVariable_multipart_data_before(
      Antlr4Gen::SecLangParser::Variable_multipart_data_beforeContext* ctx) override;

  std::any visitVariable_multipart_data_after(
      Antlr4Gen::SecLangParser::Variable_multipart_data_afterContext* ctx) override;

  std::any visitVariable_multipart_header_folding(
      Antlr4Gen::SecLangParser::Variable_multipart_header_foldingContext* ctx) override;

  std::any visitVariable_multipart_lf_line(
      Antlr4Gen::SecLangParser::Variable_multipart_lf_lineContext* ctx) override;

  std::any visitVariable_multipart_missing_semicolon(
      Antlr4Gen::SecLangParser::Variable_multipart_missing_semicolonContext* ctx) override;

  std::any visitVariable_multipart_invalid_quoting(
      Antlr4Gen::SecLangParser::Variable_multipart_invalid_quotingContext* ctx) override;

  std::any visitVariable_multipart_invalid_part(
      Antlr4Gen::SecLangParser::Variable_multipart_invalid_partContext* ctx) override;

  std::any visitVariable_multipart_invalid_header_folding(
      Antlr4Gen::SecLangParser::Variable_multipart_invalid_header_foldingContext* ctx) override;

  std::any visitVariable_multipart_file_limit_exceeded(
      Antlr4Gen::SecLangParser::Variable_multipart_file_limit_exceededContext* ctx) override;

  std::any visitVariable_global(Antlr4Gen::SecLangParser::Variable_globalContext* ctx) override;

  std::any visitVariable_resource(Antlr4Gen::SecLangParser::Variable_resourceContext* ctx) override;

  std::any visitVariable_ip(Antlr4Gen::SecLangParser::Variable_ipContext* ctx) override;

  std::any visitVariable_user(Antlr4Gen::SecLangParser::Variable_userContext* ctx) override;

  // SecRule operators
public:
  std::any visitOp_begins_with(Antlr4Gen::SecLangParser::Op_begins_withContext* ctx) override;

  std::any visitOp_contains(Antlr4Gen::SecLangParser::Op_containsContext* ctx) override;

  std::any visitOp_contains_word(Antlr4Gen::SecLangParser::Op_contains_wordContext* ctx) override;

  std::any visitOp_detect_sqli(Antlr4Gen::SecLangParser::Op_detect_sqliContext* ctx) override;

  std::any visitOp_detect_xss(Antlr4Gen::SecLangParser::Op_detect_xssContext* ctx) override;

  std::any visitOp_ends_with(Antlr4Gen::SecLangParser::Op_ends_withContext* ctx) override;

  std::any visitOp_fuzzy_hash(Antlr4Gen::SecLangParser::Op_fuzzy_hashContext* ctx) override;

  std::any visitOp_eq(Antlr4Gen::SecLangParser::Op_eqContext* ctx) override;

  std::any visitOp_ge(Antlr4Gen::SecLangParser::Op_geContext* ctx) override;

  std::any visitOp_geo_lookup(Antlr4Gen::SecLangParser::Op_geo_lookupContext* ctx) override;

  std::any visitOp_gt(Antlr4Gen::SecLangParser::Op_gtContext* ctx) override;

  std::any visitOp_inspect_file(Antlr4Gen::SecLangParser::Op_inspect_fileContext* ctx) override;

  std::any visitOp_ip_match(Antlr4Gen::SecLangParser::Op_ip_matchContext* ctx) override;

  std::any visitOp_ip_match_f(Antlr4Gen::SecLangParser::Op_ip_match_fContext* ctx) override;

  std::any
  visitOp_ip_match_from_file(Antlr4Gen::SecLangParser::Op_ip_match_from_fileContext* ctx) override;

  std::any visitOp_le(Antlr4Gen::SecLangParser::Op_leContext* ctx) override;

  std::any visitOp_lt(Antlr4Gen::SecLangParser::Op_ltContext* ctx) override;

  std::any visitOp_no_match(Antlr4Gen::SecLangParser::Op_no_matchContext* ctx) override;

  std::any visitOp_pm(Antlr4Gen::SecLangParser::Op_pmContext* ctx) override;

  std::any visitOp_pmf(Antlr4Gen::SecLangParser::Op_pmfContext* ctx) override;

  std::any visitOp_pm_from_file(Antlr4Gen::SecLangParser::Op_pm_from_fileContext* ctx) override;

  std::any visitOp_rbl(Antlr4Gen::SecLangParser::Op_rblContext* ctx) override;

  std::any visitOp_rsub(Antlr4Gen::SecLangParser::Op_rsubContext* ctx) override;

  std::any visitOp_rx(Antlr4Gen::SecLangParser::Op_rxContext* ctx) override;

  std::any visitOp_rx_global(Antlr4Gen::SecLangParser::Op_rx_globalContext* ctx) override;

  std::any visitOp_streq(Antlr4Gen::SecLangParser::Op_streqContext* ctx) override;

  std::any visitOp_strmatch(Antlr4Gen::SecLangParser::Op_strmatchContext* ctx) override;

  std::any visitOp_unconditional_match(
      Antlr4Gen::SecLangParser::Op_unconditional_matchContext* ctx) override;

  std::any visitOp_validate_byte_range(
      Antlr4Gen::SecLangParser::Op_validate_byte_rangeContext* ctx) override;

  std::any visitOp_validate_dtd(Antlr4Gen::SecLangParser::Op_validate_dtdContext* ctx) override;

  std::any
  visitOp_validate_schema(Antlr4Gen::SecLangParser::Op_validate_schemaContext* ctx) override;

  std::any visitOp_validate_url_encoding(
      Antlr4Gen::SecLangParser::Op_validate_url_encodingContext* ctx) override;

  std::any visitOp_validate_utf8_encoding(
      Antlr4Gen::SecLangParser::Op_validate_utf8_encodingContext* ctx) override;

  std::any visitOp_verify_cc(Antlr4Gen::SecLangParser::Op_verify_ccContext* ctx) override;

  std::any visitOp_verify_cpf(Antlr4Gen::SecLangParser::Op_verify_cpfContext* ctx) override;

  std::any visitOp_verify_ssn(Antlr4Gen::SecLangParser::Op_verify_ssnContext* ctx) override;

  std::any visitOp_within(Antlr4Gen::SecLangParser::Op_withinContext* ctx) override;

  std::any visitOp_rx_default(Antlr4Gen::SecLangParser::Op_rx_defaultContext* ctx) override;

  // Action Group: Meta-data
public:
  std::any
  visitAction_meta_data_id(Antlr4Gen::SecLangParser::Action_meta_data_idContext* ctx) override;
  std::any visitAction_meta_data_phase(
      Antlr4Gen::SecLangParser::Action_meta_data_phaseContext* ctx) override;
  std::any
  visitAction_meta_data_msg(Antlr4Gen::SecLangParser::Action_meta_data_msgContext* ctx) override;
  std::any
  visitAction_meta_data_tag(Antlr4Gen::SecLangParser::Action_meta_data_tagContext* ctx) override;
  std::any
  visitAction_meta_data_ver(Antlr4Gen::SecLangParser::Action_meta_data_verContext* ctx) override;
  std::any
  visitAction_meta_data_rev(Antlr4Gen::SecLangParser::Action_meta_data_revContext* ctx) override;
  std::any visitAction_meta_data_accuracy(
      Antlr4Gen::SecLangParser::Action_meta_data_accuracyContext* ctx) override;
  std::any visitAction_meta_data_maturity(
      Antlr4Gen::SecLangParser::Action_meta_data_maturityContext* ctx) override;
  std::any visitAction_meta_data_severity_emergency(
      Antlr4Gen::SecLangParser::Action_meta_data_severity_emergencyContext* ctx) override;
  std::any visitAction_meta_data_severity_alert(
      Antlr4Gen::SecLangParser::Action_meta_data_severity_alertContext* ctx) override;
  std::any visitAction_meta_data_severity_critical(
      Antlr4Gen::SecLangParser::Action_meta_data_severity_criticalContext* ctx) override;
  std::any visitAction_meta_data_severity_error(
      Antlr4Gen::SecLangParser::Action_meta_data_severity_errorContext* ctx) override;
  std::any visitAction_meta_data_severity_waring(
      Antlr4Gen::SecLangParser::Action_meta_data_severity_waringContext* ctx) override;
  std::any visitAction_meta_data_severity_notice(
      Antlr4Gen::SecLangParser::Action_meta_data_severity_noticeContext* ctx) override;
  std::any visitAction_meta_data_severity_info(
      Antlr4Gen::SecLangParser::Action_meta_data_severity_infoContext* ctx) override;
  std::any visitAction_meta_data_severity_debug(
      Antlr4Gen::SecLangParser::Action_meta_data_severity_debugContext* ctx) override;
  std::any visitAction_meta_data_severity_number(
      Antlr4Gen::SecLangParser::Action_meta_data_severity_numberContext* ctx) override;

  // Action Group: Non-disruptive
public:
  // setvar
  std::any visitAction_non_disruptive_setvar_create(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_createContext* ctx) override;
  std::any visitAction_non_disruptive_setvar_create_init(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_create_initContext* ctx) override;
  std::any visitAction_non_disruptive_setvar_remove(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_removeContext* ctx) override;
  std::any visitAction_non_disruptive_setvar_increase(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_increaseContext* ctx) override;
  std::any visitAction_non_disruptive_setvar_decrease(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setvar_decreaseContext* ctx) override;

  // setenv
  std::any visitAction_non_disruptive_setenv(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setenvContext* ctx) override;

  std::any visitAction_non_disruptive_setuid(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setuidContext* ctx) override;

  std::any visitAction_non_disruptive_setrsc(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setrscContext* ctx) override;

  std::any visitAction_non_disruptive_setsid(
      Antlr4Gen::SecLangParser::Action_non_disruptive_setsidContext* ctx) override;

  std::any visitAction_non_disruptive_t_base64_decode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_base64_decodeContext* ctx) override;

  std::any visitAction_non_disruptive_t_sql_hex_decode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_sql_hex_decodeContext* ctx) override;

  std::any visitAction_non_disruptive_t_base64_decode_ext(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_base64_decode_extContext* ctx) override;

  std::any visitAction_non_disruptive_t_base64_encode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_base64_encodeContext* ctx) override;

  std::any visitAction_non_disruptive_t_cmdline(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_cmdlineContext* ctx) override;

  std::any visitAction_non_disruptive_t_compress_whitespace(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_compress_whitespaceContext* ctx) override;

  std::any visitAction_non_disruptive_t_css_decode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_css_decodeContext* ctx) override;

  std::any visitAction_non_disruptive_t_escape_seq_decode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_escape_seq_decodeContext* ctx) override;

  std::any visitAction_non_disruptive_t_hex_decode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_hex_decodeContext* ctx) override;

  std::any visitAction_non_disruptive_t_hex_encode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_hex_encodeContext* ctx) override;

  std::any visitAction_non_disruptive_t_html_entity_decode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_html_entity_decodeContext* ctx) override;

  std::any visitAction_non_disruptive_t_js_decode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_js_decodeContext* ctx) override;

  std::any visitAction_non_disruptive_t_length(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_lengthContext* ctx) override;

  std::any visitAction_non_disruptive_t_lowercase(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_lowercaseContext* ctx) override;

  std::any visitAction_non_disruptive_t_md5(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_md5Context* ctx) override;

  std::any visitAction_non_disruptive_t_none(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_noneContext* ctx) override;

  std::any visitAction_non_disruptive_t_normalise_path(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_normalise_pathContext* ctx) override;

  std::any visitAction_non_disruptive_t_normalize_path(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_normalize_pathContext* ctx) override;

  std::any visitAction_non_disruptive_t_normalise_pathwin(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_normalise_pathwinContext* ctx) override;

  std::any visitAction_non_disruptive_t_normalize_pathwin(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_normalize_pathwinContext* ctx) override;

  std::any visitAction_non_disruptive_t_parity_even_7bit(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_parity_even_7bitContext* ctx) override;

  std::any visitAction_non_disruptive_t_parity_odd_7bit(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_parity_odd_7bitContext* ctx) override;

  std::any visitAction_non_disruptive_t_parity_zero_7bit(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_parity_zero_7bitContext* ctx) override;

  std::any visitAction_non_disruptive_t_remove_nulls(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_remove_nullsContext* ctx) override;

  std::any visitAction_non_disruptive_t_remove_whitespace(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_remove_whitespaceContext* ctx) override;

  std::any visitAction_non_disruptive_t_replace_comments(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_replace_commentsContext* ctx) override;

  std::any visitAction_non_disruptive_t_remove_commentschar(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_remove_commentscharContext* ctx) override;

  std::any visitAction_non_disruptive_t_remove_comments(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_remove_commentsContext* ctx) override;

  std::any visitAction_non_disruptive_t_replace_nulls(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_replace_nullsContext* ctx) override;

  std::any visitAction_non_disruptive_t_url_decode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_url_decodeContext* ctx) override;

  std::any visitAction_non_disruptive_t_uppercase(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_uppercaseContext* ctx) override;

  std::any visitAction_non_disruptive_t_url_decode_uni(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_url_decode_uniContext* ctx) override;

  std::any visitAction_non_disruptive_t_url_encode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_url_encodeContext* ctx) override;

  std::any visitAction_non_disruptive_t_utf8_to_unicode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_utf8_to_unicodeContext* ctx) override;

  std::any visitAction_non_disruptive_t_sha1(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_sha1Context* ctx) override;

  std::any visitAction_non_disruptive_t_trim_left(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_trim_leftContext* ctx) override;

  std::any visitAction_non_disruptive_t_trim_right(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_trim_rightContext* ctx) override;

  std::any visitAction_non_disruptive_t_trim(
      Antlr4Gen::SecLangParser::Action_non_disruptive_t_trimContext* ctx) override;

  std::any visitAction_non_disruptive_ctl_audit_engine(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_audit_engineContext* ctx) override;

  std::any visitAction_non_disruptive_ctl_audit_log_parts(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_audit_log_partsContext* ctx) override;

  std::any visitAction_non_disruptive_ctl_force_request_body_variable(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_force_request_body_variableContext* ctx)
      override;

  std::any visitAction_non_disruptive_ctl_parse_xml_into_args(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_parse_xml_into_argsContext* ctx) override;

  std::any visitAction_non_disruptive_ctl_request_body_access(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_accessContext* ctx) override;

  std::any visitAction_non_disruptive_ctl_request_body_processor_url_encode(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_processor_url_encodeContext*
          ctx) override;

  std::any visitAction_non_disruptive_ctl_request_body_processor_multi_part(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_processor_multi_partContext*
          ctx) override;

  std::any visitAction_non_disruptive_ctl_request_body_processor_xml(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_processor_xmlContext* ctx)
      override;

  std::any visitAction_non_disruptive_ctl_request_body_processor_json(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_request_body_processor_jsonContext* ctx)
      override;

  std::any visitAction_non_disruptive_ctl_rule_engine(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_engineContext* ctx) override;

  std::any visitAction_non_disruptive_ctl_rule_remove_by_id(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_remove_by_idContext* ctx) override;

  std::any visitAction_non_disruptive_ctl_rule_remove_by_tag(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_remove_by_tagContext* ctx) override;

  std::any visitAction_non_disruptive_ctl_rule_remove_target_by_id(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_remove_target_by_idContext* ctx)
      override;

  std::any visitAction_non_disruptive_ctl_rule_remove_target_by_tag(
      Antlr4Gen::SecLangParser::Action_non_disruptive_ctl_rule_remove_target_by_tagContext* ctx)
      override;

  std::any visitAction_non_disruptive_audit_log(
      Antlr4Gen::SecLangParser::Action_non_disruptive_audit_logContext* ctx) override;

  std::any visitAction_non_disruptive_log(
      Antlr4Gen::SecLangParser::Action_non_disruptive_logContext* ctx) override;

  std::any visitAction_non_disruptive_no_audit_log(
      Antlr4Gen::SecLangParser::Action_non_disruptive_no_audit_logContext* ctx) override;

  std::any visitAction_non_disruptive_no_log(
      Antlr4Gen::SecLangParser::Action_non_disruptive_no_logContext* ctx) override;

  std::any visitAction_non_disruptive_logdata(
      Antlr4Gen::SecLangParser::Action_non_disruptive_logdataContext* ctx) override;

  std::any visitAction_non_disruptive_capture(
      Antlr4Gen::SecLangParser::Action_non_disruptive_captureContext* ctx) override;

  std::any visitAction_non_disruptive_multi_match(
      Antlr4Gen::SecLangParser::Action_non_disruptive_multi_matchContext* ctx) override;

  std::any visitAction_non_disruptive_initcol(
      Antlr4Gen::SecLangParser::Action_non_disruptive_initcolContext* ctx) override;
  // Action Group: Disruptive
public:
  std::any visitAction_disruptive_allow(
      Antlr4Gen::SecLangParser::Action_disruptive_allowContext* ctx) override;
  std::any visitAction_disruptive_block(
      Antlr4Gen::SecLangParser::Action_disruptive_blockContext* ctx) override;
  std::any visitAction_disruptive_deny(
      Antlr4Gen::SecLangParser::Action_disruptive_denyContext* ctx) override;
  std::any visitAction_disruptive_drop(
      Antlr4Gen::SecLangParser::Action_disruptive_dropContext* ctx) override;
  std::any visitAction_disruptive_pass(
      Antlr4Gen::SecLangParser::Action_disruptive_passContext* ctx) override;
  std::any visitAction_disruptive_redirect(
      Antlr4Gen::SecLangParser::Action_disruptive_redirectContext* ctx) override;

  // Action Group: Data
public:
  std::any
  visitAction_data_status(Antlr4Gen::SecLangParser::Action_data_statusContext* ctx) override;
  std::any
  visitAction_data_xml_ns(Antlr4Gen::SecLangParser::Action_data_xml_nsContext* ctx) override;

  // Action Grop: Flow
public:
  std::any visitAction_flow_chain(Antlr4Gen::SecLangParser::Action_flow_chainContext* ctx) override;
  std::any visitAction_flow_skip(Antlr4Gen::SecLangParser::Action_flow_skipContext* ctx) override;
  std::any visitAction_flow_skip_after(
      Antlr4Gen::SecLangParser::Action_flow_skip_afterContext* ctx) override;

  // Audit log configurations
public:
  std::any visitSec_audit_engine(Antlr4Gen::SecLangParser::Sec_audit_engineContext* ctx) override;
  std::any visitSec_audit_log(Antlr4Gen::SecLangParser::Sec_audit_logContext* ctx) override;
  std::any visitSec_audit_log2(Antlr4Gen::SecLangParser::Sec_audit_log2Context* ctx) override;
  std::any visitSec_audit_log_dir_mode(
      Antlr4Gen::SecLangParser::Sec_audit_log_dir_modeContext* ctx) override;
  std::any
  visitSec_audit_log_format(Antlr4Gen::SecLangParser::Sec_audit_log_formatContext* ctx) override;
  std::any visitSec_audit_log_file_mode(
      Antlr4Gen::SecLangParser::Sec_audit_log_file_modeContext* ctx) override;
  std::any
  visitSec_audit_log_parts(Antlr4Gen::SecLangParser::Sec_audit_log_partsContext* ctx) override;
  std::any visitSec_audit_log_relevant_status(
      Antlr4Gen::SecLangParser::Sec_audit_log_relevant_statusContext* ctx) override;
  std::any visitSec_audit_log_storage_dir(
      Antlr4Gen::SecLangParser::Sec_audit_log_storage_dirContext* ctx) override;
  std::any
  visitSec_audit_log_type(Antlr4Gen::SecLangParser::Sec_audit_log_typeContext* ctx) override;
  std::any visitSec_component_signature(
      Antlr4Gen::SecLangParser::Sec_component_signatureContext* ctx) override;

  // Extension directives
public:
  std::any visitSec_rule_update_operator_by_id(
      Antlr4Gen::SecLangParser::Sec_rule_update_operator_by_idContext* ctx) override;
  std::any visitSec_rule_update_operator_by_tag(
      Antlr4Gen::SecLangParser::Sec_rule_update_operator_by_tagContext* ctx) override;

private:
  static bool optionStr2Bool(const std::string& option_str);
  static EngineConfig::Option optionStr2EnumValue(const std::string& option_str);
  static EngineConfig::BodyLimitAction bodyLimitActionStr2EnumValue(const std::string& action_str);
  std::expected<std::unique_ptr<Macro::MacroBase>, std::string> getMacro(
      std::string&& text,
      const std::vector<Wge::Antlr4::Antlr4Gen::SecLangParser::VariableContext*>& macro_ctx_array,
      bool is_only_macro);

  void setRuleNeedPushMatched(Variable::VariableBase* variable);

  template <class VarT, class CtxT> std::any appendVariable(CtxT* ctx) {
    std::string sub_name;
    if (ctx->STRING()) {
      sub_name = ctx->STRING()->getText();
    }
    const bool is_not = ctx->NOT() != nullptr;
    const bool is_counter = ctx->VAR_COUNT() != nullptr;

    if (current_rule_->visitVariableMode() == CurrentRule::VisitVariableMode::Ctl) {
      // std::any is copyable, so we can't return a unique_ptr
      std::shared_ptr<Variable::VariableBase> variable(
          new VarT(std::move(sub_name), is_not, is_counter, parser_->currLoadFile()));
      setRuleNeedPushMatched(variable.get());

      // Only accept xxx:yyy format
      if (ctx->DOT()) {
        RETURN_ERROR(std::format("Variable name cannot contain '.': {}.{}", variable->mainName(),
                                 variable->subName()));
      }

      return variable;
    } else if (current_rule_->visitVariableMode() == CurrentRule::VisitVariableMode::Macro) {
      std::unique_ptr<Variable::VariableBase> variable(
          new VarT(std::move(sub_name), false, false, parser_->currLoadFile()));
      setRuleNeedPushMatched(variable.get());

      // Only accept xxx.yyy format
      if (ctx->COLON()) {
        RETURN_ERROR(std::format("Variable name cannot contain ':': {}.{}", variable->mainName(),
                                 variable->subName()));
      }

      std::string letera_value;
      if (variable->subName().empty()) {
        letera_value = std::format("%{{}}", variable->mainName());
      } else {
        letera_value = std::format("%{{{}:{}}}", variable->mainName(), variable->subName());
      }

      Macro::MacroBase* macro_ptr =
          new Macro::VariableMacro(std::move(letera_value), std::move(variable));

      // The raw pointer will be managed by std::unique_ptr in getMacro
      return macro_ptr;
    } else {
      std::unique_ptr<Variable::VariableBase> variable(
          new VarT(std::move(sub_name), is_not, is_counter, parser_->currLoadFile()));
      setRuleNeedPushMatched(variable.get());

      // Only accept xxx:yyy format
      if (ctx->DOT()) {
        RETURN_ERROR(std::format("Variable name cannot contain '.': {}.{}", variable->mainName(),
                                 variable->subName()));
      }

      // Append variable
      current_rule_->get()->appendVariable(std::move(variable));

      return EMPTY_STRING;
    }
  }

  template <class OperatorT, class CtxT> std::any setOperator(CtxT* ctx) {
    std::expected<std::unique_ptr<Macro::MacroBase>, std::string> macro =
        getMacro(ctx->string_with_macro()->getText(), ctx->string_with_macro()->variable(),
                 ctx->string_with_macro()->STRING().empty());

    if (!macro.has_value()) {
      RETURN_ERROR(macro.error());
    }

    std::unique_ptr<Operator::OperatorBase> op;
    if (macro.value()) {
      auto& macro_ptr = macro.value();
      if (current_rule_->visitOperatorMode() ==
          CurrentRule::VisitOperatorMode::SecRuleUpdateOperator) {
        // In the SecRuleUpdateOperator mode:
        // - If the macro type is VariableMacro and the variable is RULE.operator_value, we need
        // expand the macro to get the original value of the operator.
        // - If the macro type is VariableMacro but the varaible is not RULE.operator_value, we use
        // it directly.
        // - If the macro type is MultiMacro, we don't support it yet.
        Wge::Macro::VariableMacro* variable_macro_ptr =
            dynamic_cast<Wge::Macro::VariableMacro*>(macro_ptr.get());
        if (variable_macro_ptr) {
          std::string_view variable_main_name = variable_macro_ptr->getVariable()->mainName();
          const std::string& variable_sub_name = variable_macro_ptr->getVariable()->subName();
          if (variable_main_name == "RULE" && variable_sub_name == "operator_value") {
            std::string original_operator_literal_value =
                current_rule_->get()->getOperator()->literalValue();
            if (!original_operator_literal_value.empty()) {
              op = std::unique_ptr<Operator::OperatorBase>(
                  new OperatorT(std::move(original_operator_literal_value), ctx->NOT() != nullptr,
                                parser_->currLoadFile()));
            } else {
              op = std::unique_ptr<Operator::OperatorBase>(
                  new OperatorT(std::move(current_rule_->get()->getOperator()->macro()),
                                ctx->NOT() != nullptr, parser_->currLoadFile()));
            }
          } else {
            op = std::unique_ptr<Operator::OperatorBase>(new OperatorT(
                std::move(macro_ptr), ctx->NOT() != nullptr, parser_->currLoadFile()));
          }
        } else if (dynamic_cast<Wge::Macro::MultiMacro*>(macro_ptr.get())) {
          // We don't support MultiMacro yet.
          // FIXME(zhouyu 2025-05-09): Add support for MultiMacro in SecRuleUpdateOperator.
          // It a bit tricky because we need merge the original operator value to a new macro if the
          // macro has RULE.operator_value and the original operator value is a multi macro. I am
          // just want to finish the basic feature first.
          assert(false);
          RETURN_ERROR("MultiMacro is not supported yet in SecRuleUpdateOperator.");
        }
      } else {
        op = std::unique_ptr<Operator::OperatorBase>(
            new OperatorT(std::move(macro_ptr), ctx->NOT() != nullptr, parser_->currLoadFile()));
      }
    } else {
      op = std::unique_ptr<Operator::OperatorBase>(new OperatorT(
          ctx->string_with_macro()->getText(), ctx->NOT() != nullptr, parser_->currLoadFile()));
    }

    current_rule_->get()->setOperator(std::move(op));
    return EMPTY_STRING;
  }

private:
  class CurrentRule {
  public:
    enum class VisitVariableMode { SecRule, SecRuleUpdateTarget, Ctl, Macro };
    enum class VisitActionMode { SecRule, SecRuleUpdateAction, SecAction, SecDefaultAction };
    enum class VisitOperatorMode { SecRule, SecRuleUpdateOperator };

  public:
    CurrentRule(Parser* parser, int line, Rule* parent_rule)
        : parser_(parser), parent_rule_(parent_rule) {
      created_rule_ = std::make_unique<Rule>(parser->currLoadFile(), line);
    }

    CurrentRule(Parser* parser, uint64_t existed_rule_id) : parser_(parser) {
      existed_rule_ = parser->findRuleById(existed_rule_id);
      assert(existed_rule_);
    }

    CurrentRule(Parser* parser, Rule* rule) : parser_(parser) {
      existed_rule_ = rule;
      assert(existed_rule_);
    }

    ~CurrentRule() { finalize(true); }

  public:
    Rule* get() const { return created_rule_ ? created_rule_.get() : existed_rule_; }
    Rule* parent() const { return parent_rule_; }
    void visitVariableMode(VisitVariableMode mode) { visit_variable_mode_ = mode; }
    void visitActionMode(VisitActionMode mode) { visit_action_mode_ = mode; }
    void visitOperatorMode(VisitOperatorMode mode) { visit_operator_mode_ = mode; }
    VisitVariableMode visitVariableMode() const { return visit_variable_mode_; }
    VisitActionMode visitActionMode() const { return visit_action_mode_; }
    VisitOperatorMode visitOperatorMode() const { return visit_operator_mode_; }
    Rule* finalize(bool append) {
      Rule* appended_rule = nullptr;
      // Drop created rule if not appending
      if (!append) {
        created_rule_.reset();
        return appended_rule;
      }

      if (created_rule_) {
        if (parent_rule_) {
          // Check the chain rule count limit, ensure the chain index won't overflow
          if (std::numeric_limits<RuleChainIndexType>::max() <= parent_rule_->chainIndex()) {
            assert(false && "Too many chain rules in the rule");
            return appended_rule;
          }

          appended_rule = created_rule_.get();
          parent_rule_->appendChainRule(std::move(created_rule_));
        } else {
          switch (visit_action_mode_) {
          case VisitActionMode::SecRule:
            appended_rule = parser_->secRule(std::move(created_rule_));
            break;
          case VisitActionMode::SecAction:
            parser_->secAction(std::move(created_rule_));
            break;
          case VisitActionMode::SecDefaultAction:
            parser_->secDefaultAction(std::move(created_rule_));
            break;
          default:
            assert(false);
            break;
          }
        }
      }
      return appended_rule;
    }

  private:
    Parser* parser_;
    // The rule will be appended to parser's rule list when finalized or when a chained rule is
    // created.
    std::unique_ptr<Rule> created_rule_;
    // The rule use to update existed rule in SecRuleUpdateXXX directives.
    Rule* existed_rule_{nullptr};
    Rule* parent_rule_{nullptr};
    VisitVariableMode visit_variable_mode_{VisitVariableMode::SecRule};
    VisitActionMode visit_action_mode_{VisitActionMode::SecRule};
    VisitOperatorMode visit_operator_mode_{VisitOperatorMode::SecRule};
  };

private:
  Parser* parser_;
  std::unique_ptr<CurrentRule> current_rule_;
  bool chain_{false};
  std::unordered_multimap<std::string, std::string> action_map_;
  bool should_visit_next_child_{true};
};
} // namespace Wge::Antlr4