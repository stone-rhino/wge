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
parser grammar SecLangParser;

options {
	tokenVocab = SecLangLexer;
}

configuration: (
		include
		| engine_config
		| engine_action
		| rule_directive
		| audit_log_config
		| extension_directive
	)* EOF;

include: Include QUOTE? STRING QUOTE?;

engine_config:
	sec_reqeust_body_access
	| sec_response_body_mime_type
	| sec_response_body_mime_type_clear
	| sec_response_body_access
	| sec_rule_engine
	| sec_tmp_save_uploaded_files
	| sec_upload_file_limit
	| sec_upload_keep_files
	| sec_xml_external_entity
	| sec_request_body_limit
	| sec_request_body_no_files_limit
	| sec_request_body_json_depth_limit
	| sec_request_body_action
	| sec_response_body_limit
	| sec_response_body_action
	| sec_status_engine
	| sec_tmp_dir
	| sec_data_dir
	| sec_cookie_format
	| sec_arguments_limit
	| sec_argument_separator
	| sec_unicode_map_file
	| sec_pcre_match_limit
	| sec_pcre_match_limit_recursion
	| sec_collection_timeout;
sec_reqeust_body_access: SecRequestBodyAccess OPTION;
sec_response_body_mime_type: SecResponseBodyMimeType MIME_TYPES;
sec_response_body_mime_type_clear:
	SecResponseBodyMimeTypesClear;
sec_response_body_access: SecResponseBodyAccess OPTION;
sec_rule_engine: SecRuleEngine OPTION;
sec_tmp_save_uploaded_files: SecTmpSaveUploadedFiles OPTION;
sec_upload_file_limit: SecUploadFileLimit INT;
sec_upload_keep_files: SecUploadKeepFiles OPTION;
sec_xml_external_entity: SecXmlExternalEntity OPTION;
sec_request_body_limit: SecRequestBodyLimit INT;
sec_request_body_no_files_limit: SecRequestBodyNoFilesLimit INT;
sec_request_body_json_depth_limit:
	SecRequestBodyJsonDepthLimit INT;
sec_request_body_action:
	SecRequestBodyLimitAction BODY_LIMIT_ACTION;
sec_response_body_limit: SecResponseBodyLimit INT;
sec_response_body_action:
	SecResponseBodyLimitAction BODY_LIMIT_ACTION;
sec_status_engine: SecStatusEngine OPTION;
sec_tmp_dir: SecTmpDir STRING;
sec_data_dir: SecDataDir STRING;
sec_cookie_format: SecCookieFormat INT;
sec_arguments_limit: SecArgumentsLimit INT;
sec_argument_separator: SecArgumentSeparator STRING;
sec_unicode_map_file: SecUnicodeMapFile STRING INT;
sec_pcre_match_limit: SecPcreMatchLimit INT;
sec_pcre_match_limit_recursion: SecPcreMatchLimitRecursion INT;
sec_collection_timeout: SecCollectionTimeout INT;

engine_action: sec_action | sec_default_action;
sec_action: SecAction QUOTE action ( COMMA action)* QUOTE;
sec_default_action:
	SecDefaultAction QUOTE action (COMMA action)* QUOTE;

rule_directive:
	sec_rule
	| sec_rule_remove_by_id
	| sec_rule_remove_by_msg
	| sec_rule_remove_by_tag
	| sec_rule_update_action_by_id
	| sec_rule_update_target_by_id
	| sec_rule_update_target_by_msg
	| sec_rule_update_target_by_tag
	| sec_marker;
sec_rule_remove_by_id:
	SecRuleRemoveById (INT | INT_RANGE) (INT | INT_RANGE)*;
sec_rule_remove_by_msg: SecRuleRemoveByMsg QUOTE STRING QUOTE;
sec_rule_remove_by_tag: SecRuleRemoveByTag QUOTE STRING QUOTE;
sec_rule_update_action_by_id:
	SecRuleUpdateActionById INT QUOTE action (COMMA action)* QUOTE;
sec_rule_update_target_by_id:
	SecRuleUpdateTargetById INT variables;
sec_rule_update_target_by_msg:
	SecRuleUpdateTargetByMsg ((QUOTE STRING QUOTE) | STRING) variables;
sec_rule_update_target_by_tag:
	SecRuleUpdateTargetByTag ((QUOTE STRING QUOTE) | STRING) variables;
sec_marker: SecMarker ((QUOTE STRING QUOTE) | STRING);

sec_rule:
	SecRule variables QUOTE operator QUOTE (
		QUOTE action (COMMA? action)* QUOTE
	)?;

variables: variable (PIPE variable)*;

variable:
	variable_args
	| variable_args_combined_size
	| variable_args_get
	| variable_args_get_names
	| variable_args_names
	| variable_args_post
	| variable_args_post_names
	| variable_auth_type
	| variable_duration
	| variable_env
	| variable_files
	| variable_files_combined_size
	| variable_files_names
	| variable_full_request
	| variable_full_request_length
	| variable_files_sizes
	| variable_files_tmpnames
	| variable_files_tmp_content
	| variable_geo
	| variable_highest_severity
	| variable_inbound_data_error
	| variable_matched_var
	| variable_matched_vars
	| variable_matched_var_name
	| variable_matched_vars_names
	| variable_modsec_build
	| variable_msc_pcre_limits_exceeded
	| variable_multipart_crlf_lf_lines
	| variable_multipart_filename
	| variable_multipart_name
	| variable_multipart_part_headers
	| variable_multipart_strict_error
	| variable_multipart_unmatched_boundary
	| variable_outbound_data_error
	| variable_path_info
	| variable_query_string
	| variable_remote_addr
	| variable_remote_host
	| variable_remote_port
	| variable_remote_user
	| variable_reqbody_error
	| variable_reqbody_error_msg
	| variable_reqbody_processor
	| variable_request_basename
	| variable_request_body
	| variable_request_body_length
	| variable_request_cookies
	| variable_request_cookies_names
	| variable_request_filename
	| variable_request_headers
	| variable_request_headers_names
	| variable_request_line
	| variable_request_method
	| variable_request_protocol
	| variable_request_uri
	| variable_request_uri_raw
	| variable_response_body
	| variable_response_content_length
	| variable_response_content_type
	| variable_response_headers
	| variable_response_headers_names
	| variable_response_protocol
	| variable_response_status
	| variable_rule
	| variable_server_addr
	| variable_server_name
	| variable_server_port
	| variable_session
	| variable_sessionid
	| variable_status_line
	| variable_time
	| variable_time_day
	| variable_time_epoch
	| variable_time_hour
	| variable_time_min
	| variable_time_mon
	| variable_time_sec
	| variable_time_wday
	| variable_time_year
	| variable_tx
	| variable_unique_id
	| variable_urlencoded_error
	| variable_userid
	| variable_webappid
	| variable_xml
	| variable_reqbody_processor_error
	| variable_multipart_boundary_quoted
	| variable_multipart_boundary_whitespace
	| variable_multipart_data_before
	| variable_multipart_data_after
	| variable_multipart_header_folding
	| variable_multipart_lf_line
	| variable_multipart_missing_semicolon
	| variable_multipart_invalid_quoting
	| variable_multipart_invalid_part
	| variable_multipart_invalid_header_folding
	| variable_multipart_file_limit_exceeded
	| variable_global
	| variable_resource
	| variable_ip
	| variable_user;
variable_args: NOT? VAR_COUNT? VAR_ARGS ((COLON | DOT) STRING)?;
variable_args_combined_size:
	NOT? VAR_COUNT? VAR_ARGS_COMBINED_SIZE ((COLON | DOT) STRING)?;
variable_args_get:
	NOT? VAR_COUNT? VAR_ARGS_GET ((COLON | DOT) STRING)?;
variable_args_get_names:
	NOT? VAR_COUNT? VAR_ARGS_GET_NAMES ((COLON | DOT) STRING)?;
variable_args_names:
	NOT? VAR_COUNT? VAR_ARGS_NAMES ((COLON | DOT) STRING)?;
variable_args_post:
	NOT? VAR_COUNT? VAR_ARGS_POST ((COLON | DOT) STRING)?;
variable_args_post_names:
	NOT? VAR_COUNT? VAR_ARGS_POST_NAMES ((COLON | DOT) STRING)?;
variable_auth_type:
	NOT? VAR_COUNT? VAR_AUTH_TYPE ((COLON | DOT) STRING)?;
variable_duration:
	NOT? VAR_COUNT? VAR_DURATION ((COLON | DOT) STRING)?;
variable_env: NOT? VAR_COUNT? VAR_ENV ((COLON | DOT) STRING)?;
variable_files:
	NOT? VAR_COUNT? VAR_FILES ((COLON | DOT) STRING)?;
variable_files_combined_size:
	NOT? VAR_COUNT? VAR_FILES_COMBINED_SIZE (
		(COLON | DOT) STRING
	)?;
variable_files_names:
	NOT? VAR_COUNT? VAR_FILES_NAMES ((COLON | DOT) STRING)?;
variable_full_request:
	NOT? VAR_COUNT? VAR_FULL_REQUEST ((COLON | DOT) STRING)?;
variable_full_request_length:
	NOT? VAR_COUNT? VAR_FULL_REQUEST_LENGTH (
		(COLON | DOT) STRING
	)?;
variable_files_sizes:
	NOT? VAR_COUNT? VAR_FILES_SIZES ((COLON | DOT) STRING)?;
variable_files_tmpnames:
	NOT? VAR_COUNT? VAR_FILES_TMPNAMES ((COLON | DOT) STRING)?;
variable_files_tmp_content:
	NOT? VAR_COUNT? VAR_FILES_TMP_CONTENT ((COLON | DOT) STRING)?;
variable_geo: NOT? VAR_COUNT? VAR_GEO ((COLON | DOT) STRING)?;
variable_highest_severity:
	NOT? VAR_COUNT? VAR_HIGHEST_SEVERITY ((COLON | DOT) STRING)?;
variable_inbound_data_error:
	NOT? VAR_COUNT? VAR_INBOUND_DATA_ERROR ((COLON | DOT) STRING)?;
variable_matched_var:
	NOT? VAR_COUNT? VAR_MATCHED_VAR ((COLON | DOT) STRING)?;
variable_matched_vars:
	NOT? VAR_COUNT? VAR_MATCHED_VARS ((COLON | DOT) STRING)?;
variable_matched_var_name:
	NOT? VAR_COUNT? VAR_MATCHED_VAR_NAME ((COLON | DOT) STRING)?;
variable_matched_vars_names:
	NOT? VAR_COUNT? VAR_MATCHED_VARS_NAMES ((COLON | DOT) STRING)?;
variable_modsec_build:
	NOT? VAR_COUNT? VAR_MODSEC_BUILD ((COLON | DOT) STRING)?;
variable_msc_pcre_limits_exceeded:
	NOT? VAR_COUNT? VAR_MSC_PCRE_LIMITS_EXCEEDED (
		(COLON | DOT) STRING
	)?;
variable_multipart_crlf_lf_lines:
	NOT? VAR_COUNT? VAR_MULTIPART_CRLF_LF_LINES (
		(COLON | DOT) STRING
	)?;
variable_multipart_filename:
	NOT? VAR_COUNT? VAR_MULTIPART_FILENAME ((COLON | DOT) STRING)?;
variable_multipart_name:
	NOT? VAR_COUNT? VAR_MULTIPART_NAME ((COLON | DOT) STRING)?;
variable_multipart_part_headers:
	NOT? VAR_COUNT? VAR_MULTIPART_PART_HEADERS (
		(COLON | DOT) STRING
	)?;
variable_multipart_strict_error:
	NOT? VAR_COUNT? VAR_MULTIPART_STRICT_ERROR (
		(COLON | DOT) STRING
	)?;
variable_multipart_unmatched_boundary:
	NOT? VAR_COUNT? VAR_MULTIPART_UNMATCHED_BOUNDARY (
		(COLON | DOT) STRING
	)?;
variable_outbound_data_error:
	NOT? VAR_COUNT? VAR_OUTBOUND_DATA_ERROR (
		(COLON | DOT) STRING
	)?;
variable_path_info:
	NOT? VAR_COUNT? VAR_PATH_INFO ((COLON | DOT) STRING)?;
variable_query_string:
	NOT? VAR_COUNT? VAR_QUERY_STRING ((COLON | DOT) STRING)?;
variable_remote_addr:
	NOT? VAR_COUNT? VAR_REMOTE_ADDR ((COLON | DOT) STRING)?;
variable_remote_host:
	NOT? VAR_COUNT? VAR_REMOTE_HOST ((COLON | DOT) STRING)?;
variable_remote_port:
	NOT? VAR_COUNT? VAR_REMOTE_PORT ((COLON | DOT) STRING)?;
variable_remote_user:
	NOT? VAR_COUNT? VAR_REMOTE_USER ((COLON | DOT) STRING)?;
variable_reqbody_error:
	NOT? VAR_COUNT? VAR_REQBODY_ERROR ((COLON | DOT) STRING)?;
variable_reqbody_error_msg:
	NOT? VAR_COUNT? VAR_REQBODY_ERROR_MSG ((COLON | DOT) STRING)?;
variable_reqbody_processor:
	NOT? VAR_COUNT? VAR_REQBODY_PROCESSOR ((COLON | DOT) STRING)?;
variable_request_basename:
	NOT? VAR_COUNT? VAR_REQUEST_BASENAME ((COLON | DOT) STRING)?;
variable_request_body:
	NOT? VAR_COUNT? VAR_REQUEST_BODY ((COLON | DOT) STRING)?;
variable_request_body_length:
	NOT? VAR_COUNT? VAR_REQUEST_BODY_LENGTH (
		(COLON | DOT) STRING
	)?;
variable_request_cookies:
	NOT? VAR_COUNT? VAR_REQUEST_COOKIES ((COLON | DOT) STRING)?;
variable_request_cookies_names:
	NOT? VAR_COUNT? VAR_REQUEST_COOKIES_NAMES (
		(COLON | DOT) STRING
	)?;
variable_request_filename:
	NOT? VAR_COUNT? VAR_REQUEST_FILENAME ((COLON | DOT) STRING)?;
variable_request_headers:
	NOT? VAR_COUNT? VAR_REQUEST_HEADERS ((COLON | DOT) STRING)?;
variable_request_headers_names:
	NOT? VAR_COUNT? VAR_REQUEST_HEADERS_NAMES (
		(COLON | DOT) STRING
	)?;
variable_request_line:
	NOT? VAR_COUNT? VAR_REQUEST_LINE ((COLON | DOT) STRING)?;
variable_request_method:
	NOT? VAR_COUNT? VAR_REQUEST_METHOD ((COLON | DOT) STRING)?;
variable_request_protocol:
	NOT? VAR_COUNT? VAR_REQUEST_PROTOCOL ((COLON | DOT) STRING)?;
variable_request_uri:
	NOT? VAR_COUNT? VAR_REQUEST_URI ((COLON | DOT) STRING)?;
variable_request_uri_raw:
	NOT? VAR_COUNT? VAR_REQUEST_URI_RAW ((COLON | DOT) STRING)?;
variable_response_body:
	NOT? VAR_COUNT? VAR_RESPONSE_BODY ((COLON | DOT) STRING)?;
variable_response_content_length:
	NOT? VAR_COUNT? VAR_RESPONSE_CONTENT_LENGTH (
		(COLON | DOT) STRING
	)?;
variable_response_content_type:
	NOT? VAR_COUNT? VAR_RESPONSE_CONTENT_TYPE (
		(COLON | DOT) STRING
	)?;
variable_response_headers:
	NOT? VAR_COUNT? VAR_RESPONSE_HEADERS ((COLON | DOT) STRING)?;
variable_response_headers_names:
	NOT? VAR_COUNT? VAR_RESPONSE_HEADERS_NAMES (
		(COLON | DOT) STRING
	)?;
variable_response_protocol:
	NOT? VAR_COUNT? VAR_RESPONSE_PROTOCOL ((COLON | DOT) STRING)?;
variable_response_status:
	NOT? VAR_COUNT? VAR_RESPONSE_STATUS ((COLON | DOT) STRING)?;
variable_rule: NOT? VAR_COUNT? VAR_RULE ((COLON | DOT) STRING)?;
variable_server_addr:
	NOT? VAR_COUNT? VAR_SERVER_ADDR ((COLON | DOT) STRING)?;
variable_server_name:
	NOT? VAR_COUNT? VAR_SERVER_NAME ((COLON | DOT) STRING)?;
variable_server_port:
	NOT? VAR_COUNT? VAR_SERVER_PORT ((COLON | DOT) STRING)?;
variable_session:
	NOT? VAR_COUNT? VAR_SESSION ((COLON | DOT) STRING)?;
variable_sessionid:
	NOT? VAR_COUNT? VAR_SESSIONID ((COLON | DOT) STRING)?;
variable_status_line:
	NOT? VAR_COUNT? VAR_STATUS_LINE ((COLON | DOT) STRING)?;
variable_time: NOT? VAR_COUNT? VAR_TIME ((COLON | DOT) STRING)?;
variable_time_day:
	NOT? VAR_COUNT? VAR_TIME_DAY ((COLON | DOT) STRING)?;
variable_time_epoch:
	NOT? VAR_COUNT? VAR_TIME_EPOCH ((COLON | DOT) STRING)?;
variable_time_hour:
	NOT? VAR_COUNT? VAR_TIME_HOUR ((COLON | DOT) STRING)?;
variable_time_min:
	NOT? VAR_COUNT? VAR_TIME_MIN ((COLON | DOT) STRING)?;
variable_time_mon:
	NOT? VAR_COUNT? VAR_TIME_MON ((COLON | DOT) STRING)?;
variable_time_sec:
	NOT? VAR_COUNT? VAR_TIME_SEC ((COLON | DOT) STRING)?;
variable_time_wday:
	NOT? VAR_COUNT? VAR_TIME_WDAY ((COLON | DOT) STRING)?;
variable_time_year:
	NOT? VAR_COUNT? VAR_TIME_YEAR ((COLON | DOT) STRING)?;
variable_tx: NOT? VAR_COUNT? VAR_TX ((COLON | DOT) STRING)?;
variable_unique_id:
	NOT? VAR_COUNT? VAR_UNIQUE_ID ((COLON | DOT) STRING)?;
variable_urlencoded_error:
	NOT? VAR_COUNT? VAR_URLENCODED_ERROR ((COLON | DOT) STRING)?;
variable_userid:
	NOT? VAR_COUNT? VAR_USERID ((COLON | DOT) STRING)?;
variable_webappid:
	NOT? VAR_COUNT? VAR_WEBAPPID ((COLON | DOT) STRING)?;
variable_xml: NOT? VAR_COUNT? VAR_XML ((COLON | DOT) STRING)?;
variable_reqbody_processor_error:
	NOT? VAR_COUNT? VAR_REQBODY_PROCESSOR_ERROR (
		(COLON | DOT) STRING
	)?;
variable_multipart_boundary_quoted:
	NOT? VAR_COUNT? VAR_MULTIPART_BOUNDARY_QUOTED (
		(COLON | DOT) STRING
	)?;
variable_multipart_boundary_whitespace:
	NOT? VAR_COUNT? VAR_MULTIPART_BOUNDARY_WHITESPACE (
		(COLON | DOT) STRING
	)?;
variable_multipart_data_before:
	NOT? VAR_COUNT? VAR_MULTIPART_DATA_BEFORE (
		(COLON | DOT) STRING
	)?;
variable_multipart_data_after:
	NOT? VAR_COUNT? VAR_MULTIPART_DATA_AFTER (
		(COLON | DOT) STRING
	)?;
variable_multipart_header_folding:
	NOT? VAR_COUNT? VAR_MULTIPART_HEADER_FOLDING (
		(COLON | DOT) STRING
	)?;
variable_multipart_lf_line:
	NOT? VAR_COUNT? VAR_MULTIPART_LF_LINE ((COLON | DOT) STRING)?;
variable_multipart_missing_semicolon:
	NOT? VAR_COUNT? VAR_MULTIPART_MISSING_SEMICOLON (
		(COLON | DOT) STRING
	)?;
variable_multipart_invalid_quoting:
	NOT? VAR_COUNT? VAR_MULTIPART_INVALID_QUOTING (
		(COLON | DOT) STRING
	)?;
variable_multipart_invalid_part:
	NOT? VAR_COUNT? VAR_MULTIPART_INVALID_PART (
		(COLON | DOT) STRING
	)?;
variable_multipart_invalid_header_folding:
	NOT? VAR_COUNT? VAR_MULTIPART_INVALID_HEADER_FOLDING (
		(COLON | DOT) STRING
	)?;
variable_multipart_file_limit_exceeded:
	NOT? VAR_COUNT? VAR_MULTIPART_FILE_LIMIT_EXCEEDED (
		(COLON | DOT) STRING
	)?;
variable_global:
	NOT? VAR_COUNT? VAR_GLOBAL ((COLON | DOT) STRING)?;
variable_resource:
	NOT? VAR_COUNT? VAR_RESOURCE ((COLON | DOT) STRING)?;
variable_ip: NOT? VAR_COUNT? VAR_IP ( (COLON | DOT) STRING)?;
variable_user: NOT? VAR_COUNT? VAR_USER ( (COLON | DOT) STRING)?;

operator:
	op_begins_with
	| op_contains
	| op_contains_word
	| op_detect_sqli
	| op_detect_xss
	| op_ends_with
	| op_fuzzy_hash
	| op_eq
	| op_ge
	| op_geo_lookup
	| op_gt
	| op_inspect_file
	| op_ip_match
	| op_ip_match_f
	| op_ip_match_from_file
	| op_le
	| op_lt
	| op_no_match
	| op_pm
	| op_pmf
	| op_pm_from_file
	| op_rbl
	| op_rsub
	| op_rx
	| op_rx_global
	| op_streq
	| op_strmatch
	| op_unconditional_match
	| op_validate_byte_range
	| op_validate_dtd
	| op_validate_schema
	| op_validate_url_encoding
	| op_validate_utf8_encoding
	| op_verify_cc
	| op_verify_cpf
	| op_verify_ssn
	| op_within
	| op_rx_default
	// Extensions
	| op_rx_and_syntax_check_sql
	| op_rx_and_syntax_check_js
	| op_rx_and_syntax_check_shell
	| op_rx_and_syntax_check_java
	| op_rx_and_syntax_check_php
	| op_detect_sqli_and_syntax_check;
op_begins_with: NOT? AT OP_BEGINS_WITH string_with_macro;
op_contains: NOT? AT OP_CONTAINS string_with_macro;
op_contains_word: NOT? AT OP_CONTAINS_WORD string_with_macro;
op_detect_sqli: NOT? AT OP_DETECT_SQLI;
op_detect_xss: NOT? AT OP_DETECT_XSS;
op_ends_with: NOT? AT OP_ENDS_WITH string_with_macro;
op_fuzzy_hash: NOT? AT OP_FUZZY_HASH string_with_macro;
op_eq: NOT? AT OP_EQ string_with_macro;
op_ge: NOT? AT OP_GE string_with_macro;
op_geo_lookup: NOT? AT OP_GEO_LOOKUP string_with_macro;
op_gt: NOT? AT OP_GT string_with_macro;
op_inspect_file: NOT? AT OP_INSPECT_FILE string_with_macro;
op_ip_match: NOT? AT OP_IP_MATCH string_with_macro;
op_ip_match_f: NOT? AT OP_IP_MATCH_F string_with_macro;
op_ip_match_from_file:
	NOT? AT OP_IP_MATCH_FROM_FILE string_with_macro;
op_le: NOT? AT OP_LE string_with_macro;
op_lt: NOT? AT OP_LT string_with_macro;
op_no_match: NOT? AT OP_NO_MATCH;
op_pm: NOT? AT OP_PM string_with_macro;
op_pmf: NOT? AT OP_PMF string_with_macro;
op_pm_from_file: NOT? AT OP_PM_FROM_FILE string_with_macro;
op_rbl: NOT? AT OP_RBL string_with_macro;
op_rsub: NOT? AT OP_RSUB string_with_macro;
op_rx: NOT? AT OP_RX string_with_macro;
op_rx_global: NOT? AT OP_RX_GLOBAL string_with_macro;
op_streq: NOT? AT OP_STREQ string_with_macro;
op_strmatch: NOT? AT OP_STRMATCH string_with_macro;
op_unconditional_match: NOT? AT OP_UNCONDITIONAL_MATCH;
op_validate_byte_range:
	NOT? AT OP_VALIDATE_BYTE_RANGE string_with_macro;
op_validate_dtd: NOT? AT OP_VALIDATE_DTD string_with_macro;
op_validate_schema:
	NOT? AT OP_VALIDATE_SCHEMA string_with_macro;
op_validate_url_encoding: NOT? AT OP_VALIDATE_URL_ENCODING;
op_validate_utf8_encoding: NOT? AT OP_VALIDATE_UTF8_ENCODING;
op_verify_cc: NOT? AT OP_VERIFY_CC string_with_macro;
op_verify_cpf: NOT? AT OP_VERIFY_CPF string_with_macro;
op_verify_ssn: NOT? AT OP_VERIFY_SSN string_with_macro;
op_within: NOT? AT OP_WITHIN string_with_macro;
op_rx_default: string_with_macro;
// Extensions
op_rx_and_syntax_check_sql:
	NOT? AT OP_RX_AND_SYNTAX_CHECK_SQL string_with_macro;
op_rx_and_syntax_check_js:
	NOT? AT OP_RX_AND_SYNTAX_CHECK_JS string_with_macro;
op_rx_and_syntax_check_shell:
	NOT? AT OP_RX_AND_SYNTAX_CHECK_SHELL string_with_macro;
op_rx_and_syntax_check_java:
	NOT? AT OP_RX_AND_SYNTAX_CHECK_JAVA string_with_macro;
op_rx_and_syntax_check_php:
	NOT? AT OP_RX_AND_SYNTAX_CHECK_PHP string_with_macro;
op_detect_sqli_and_syntax_check:
	NOT? AT OP_DETECT_SQLI_AND_SYNTAX_CHECK;

action:
	action_meta_data
	| action_non_disruptive
	| action_disruptive
	| action_data
	| action_flow;

action_meta_data:
	action_meta_data_id
	| action_meta_data_phase
	| action_meta_data_severity
	| action_meta_data_msg
	| action_meta_data_tag
	| action_meta_data_ver
	| action_meta_data_rev
	| action_meta_data_accuracy
	| action_meta_data_maturity;
action_meta_data_id:
	Id COLON (INT | (SINGLE_QUOTE STRING SINGLE_QUOTE));
action_meta_data_phase: Phase COLON INT;
action_meta_data_severity:
	Severity COLON (
		(
			SINGLE_QUOTE (
				action_meta_data_severity_emergency
				| action_meta_data_severity_alert
				| action_meta_data_severity_critical
				| action_meta_data_severity_error
				| action_meta_data_severity_waring
				| action_meta_data_severity_notice
				| action_meta_data_severity_info
				| action_meta_data_severity_debug
			) SINGLE_QUOTE
		)
		| action_meta_data_severity_number
	);
action_meta_data_msg:
	Msg COLON SINGLE_QUOTE string_with_macro SINGLE_QUOTE;
string_with_macro:
	STRING
	| (
		STRING? PER_CENT LEFT_BRACKET variable RIGHT_BRACKET STRING?
	)+;
action_meta_data_tag:
	Tag COLON SINGLE_QUOTE STRING SINGLE_QUOTE;
action_meta_data_ver:
	Ver COLON SINGLE_QUOTE STRING SINGLE_QUOTE;
action_meta_data_rev:
	Rev COLON SINGLE_QUOTE STRING SINGLE_QUOTE;
action_meta_data_accuracy: Accuracy COLON LEVEL;
action_meta_data_maturity: Maturity COLON LEVEL;
action_meta_data_severity_emergency: EMERGENCY;
action_meta_data_severity_alert: ALERT;
action_meta_data_severity_critical: CRITICAL;
action_meta_data_severity_error: ERROR;
action_meta_data_severity_waring: WARNING;
action_meta_data_severity_notice: NOTICE;
action_meta_data_severity_info: INFO;
action_meta_data_severity_debug: DEBUG;
action_meta_data_severity_number: SEVERITY_LEVEL;

action_non_disruptive:
	action_non_disruptive_setvar
	| action_non_disruptive_setenv
	| action_non_disruptive_setuid
	| action_non_disruptive_setrsc
	| action_non_disruptive_setsid
	| action_non_disruptive_t
	| action_non_disruptive_ctl
	| action_non_disruptive_audit_log
	| action_non_disruptive_log
	| action_non_disruptive_no_audit_log
	| action_non_disruptive_no_log
	| action_non_disruptive_logdata
	| action_non_disruptive_capture
	| action_non_disruptive_multi_match
	| action_non_disruptive_initcol;
action_non_disruptive_setvar:
	action_non_disruptive_setvar_create
	| action_non_disruptive_setvar_create_init
	| action_non_disruptive_setvar_remove
	| action_non_disruptive_setvar_increase
	| action_non_disruptive_setvar_decrease;
action_non_disruptive_setvar_create:
	Setvar COLON (
		(
			SINGLE_QUOTE TX DOT action_non_disruptive_setvar_varname SINGLE_QUOTE
		)
		| (TX DOT action_non_disruptive_setvar_varname)
	);
action_non_disruptive_setvar_varname: (
		VAR_NAME
		| (
			(
				VAR_NAME? PER_CENT LEFT_BRACKET variable RIGHT_BRACKET VAR_NAME?
			)+
		)
	);
action_non_disruptive_setvar_create_init:
	Setvar COLON (
		(
			SINGLE_QUOTE TX DOT action_non_disruptive_setvar_varname ASSIGN
				action_non_disruptive_setvar_create_init_value SINGLE_QUOTE
		)
		| (
			TX DOT action_non_disruptive_setvar_varname ASSIGN
				action_non_disruptive_setvar_create_init_value
		)
	);
action_non_disruptive_setvar_create_init_value: (
		VAR_VALUE
		| (
			(
				VAR_VALUE? PER_CENT LEFT_BRACKET variable RIGHT_BRACKET VAR_VALUE?
			)+
		)
	);
action_non_disruptive_setvar_remove:
	Setvar COLON (
		(
			SINGLE_QUOTE NOT TX DOT action_non_disruptive_setvar_varname SINGLE_QUOTE
		)
		| (NOT TX DOT action_non_disruptive_setvar_varname)
	);
action_non_disruptive_setvar_increase:
	Setvar COLON (
		(
			SINGLE_QUOTE TX DOT action_non_disruptive_setvar_varname ASSIGN PLUS (
				VAR_VALUE
				| ( PER_CENT LEFT_BRACKET variable RIGHT_BRACKET)
			) SINGLE_QUOTE
		)
		| (
			TX DOT action_non_disruptive_setvar_varname ASSIGN PLUS (
				VAR_VALUE
				| ( PER_CENT LEFT_BRACKET variable RIGHT_BRACKET)
			)
		)
	);
action_non_disruptive_setvar_decrease:
	Setvar COLON (
		(
			SINGLE_QUOTE TX DOT action_non_disruptive_setvar_varname ASSIGN MINUS (
				VAR_VALUE
				| ( PER_CENT LEFT_BRACKET variable RIGHT_BRACKET)
			) SINGLE_QUOTE
		)
		| (
			TX DOT action_non_disruptive_setvar_varname ASSIGN MINUS (
				VAR_VALUE
				| ( PER_CENT LEFT_BRACKET variable RIGHT_BRACKET)
			)
		)
	);

action_non_disruptive_setenv:
	Setenv COLON SINGLE_QUOTE VAR_NAME ASSIGN (
		VAR_VALUE
		| ( PER_CENT LEFT_BRACKET variable RIGHT_BRACKET)
	) SINGLE_QUOTE;

action_non_disruptive_setuid:
	Setuid COLON (
		(SINGLE_QUOTE STRING SINGLE_QUOTE)
		| ( PER_CENT LEFT_BRACKET variable RIGHT_BRACKET)
	);
action_non_disruptive_setrsc:
	Setrsc COLON (
		(SINGLE_QUOTE STRING SINGLE_QUOTE)
		| ( PER_CENT LEFT_BRACKET variable RIGHT_BRACKET)
	);
action_non_disruptive_setsid:
	Setsid COLON (
		(SINGLE_QUOTE STRING SINGLE_QUOTE)
		| ( PER_CENT LEFT_BRACKET variable RIGHT_BRACKET)
	);
action_non_disruptive_t:
	T COLON (
		action_non_disruptive_t_base64_decode
		| action_non_disruptive_t_sql_hex_decode
		| action_non_disruptive_t_base64_decode_ext
		| action_non_disruptive_t_base64_encode
		| action_non_disruptive_t_cmdline
		| action_non_disruptive_t_compress_whitespace
		| action_non_disruptive_t_css_decode
		| action_non_disruptive_t_escape_seq_decode
		| action_non_disruptive_t_hex_decode
		| action_non_disruptive_t_hex_encode
		| action_non_disruptive_t_html_entity_decode
		| action_non_disruptive_t_js_decode
		| action_non_disruptive_t_length
		| action_non_disruptive_t_lowercase
		| action_non_disruptive_t_md5
		| action_non_disruptive_t_none
		| action_non_disruptive_t_normalise_path
		| action_non_disruptive_t_normalize_path
		| action_non_disruptive_t_normalise_pathwin
		| action_non_disruptive_t_normalize_pathwin
		| action_non_disruptive_t_parity_even_7bit
		| action_non_disruptive_t_parity_odd_7bit
		| action_non_disruptive_t_parity_zero_7bit
		| action_non_disruptive_t_remove_nulls
		| action_non_disruptive_t_remove_whitespace
		| action_non_disruptive_t_replace_comments
		| action_non_disruptive_t_remove_commentschar
		| action_non_disruptive_t_remove_comments
		| action_non_disruptive_t_replace_nulls
		| action_non_disruptive_t_url_decode
		| action_non_disruptive_t_uppercase
		| action_non_disruptive_t_url_decode_uni
		| action_non_disruptive_t_url_encode
		| action_non_disruptive_t_utf8_to_unicode
		| action_non_disruptive_t_sha1
		| action_non_disruptive_t_trim_left
		| action_non_disruptive_t_trim_right
		| action_non_disruptive_t_trim
	);
action_non_disruptive_t_base64_decode: BASE64_DECODE;
action_non_disruptive_t_sql_hex_decode: SQL_HEX_DECODE;
action_non_disruptive_t_base64_decode_ext: BASE64_DECODE_EXT;
action_non_disruptive_t_base64_encode: BASE64_ENCODE;
action_non_disruptive_t_cmdline: CMDLINE;
action_non_disruptive_t_compress_whitespace:
	COMPRESS_WHITESPACE;
action_non_disruptive_t_css_decode: CSS_DECODE;
action_non_disruptive_t_escape_seq_decode: ESCAPE_SEQ_DECODE;
action_non_disruptive_t_hex_decode: HEX_DECODE;
action_non_disruptive_t_hex_encode: HEX_ENCODE;
action_non_disruptive_t_html_entity_decode: HTML_ENTITY_DECODE;
action_non_disruptive_t_js_decode: JS_DECODE;
action_non_disruptive_t_length: LENGTH;
action_non_disruptive_t_lowercase: LOWERCASE;
action_non_disruptive_t_md5: MD5;
action_non_disruptive_t_none: NONE;
action_non_disruptive_t_normalise_path: NORMALISE_PATH;
action_non_disruptive_t_normalize_path: NORMALIZE_PATH;
action_non_disruptive_t_normalise_pathwin: NORMALISE_PATHWIN;
action_non_disruptive_t_normalize_pathwin: NORMALIZE_PATHWIN;
action_non_disruptive_t_parity_even_7bit: PARITY_EVEN_7BIT;
action_non_disruptive_t_parity_odd_7bit: PARITY_ODD_7BIT;
action_non_disruptive_t_parity_zero_7bit: PARITY_ZERO_7BIT;
action_non_disruptive_t_remove_nulls: REMOVE_NULLS;
action_non_disruptive_t_remove_whitespace: REMOVE_WHITESPACE;
action_non_disruptive_t_replace_comments: REPLACE_COMMENTS;
action_non_disruptive_t_remove_commentschar:
	REMOVE_COMMENTSCHAR;
action_non_disruptive_t_remove_comments: REMOVE_COMMENTS;
action_non_disruptive_t_replace_nulls: REPLACE_NULLS;
action_non_disruptive_t_url_decode: URL_DECODE;
action_non_disruptive_t_uppercase: UPPERCASE;
action_non_disruptive_t_url_decode_uni: URL_DECODE_UNI;
action_non_disruptive_t_url_encode: URL_ENCODE;
action_non_disruptive_t_utf8_to_unicode: UTF8_TO_UNICODE;
action_non_disruptive_t_sha1: SHA1;
action_non_disruptive_t_trim_left: TRIM_LEFT;
action_non_disruptive_t_trim_right: TRIM_RIGHT;
action_non_disruptive_t_trim: TRIM;

action_non_disruptive_ctl:
	Ctl COLON (
		action_non_disruptive_ctl_audit_engine
		| action_non_disruptive_ctl_audit_log_parts
		| action_non_disruptive_ctl_force_request_body_variable
		| action_non_disruptive_ctl_request_body_access
		| action_non_disruptive_ctl_request_body_processor
		| action_non_disruptive_ctl_rule_engine
		| action_non_disruptive_ctl_rule_remove_by_id
		| action_non_disruptive_ctl_rule_remove_by_tag
		| action_non_disruptive_ctl_rule_remove_target_by_id
		| action_non_disruptive_ctl_rule_remove_target_by_tag
	);
action_non_disruptive_ctl_audit_engine:
	CTL_AUDIT_ENGINE ASSIGN AUDIT_ENGINE;
action_non_disruptive_ctl_audit_log_parts:
	CTL_AUDIT_LOG_PARTS ASSIGN (PLUS | MINUS) AUDIT_PARTS;
action_non_disruptive_ctl_force_request_body_variable:
	CTL_FORCE_REQUEST_BODY_VARIABLE ASSIGN OPTION;
action_non_disruptive_ctl_request_body_access:
	CTL_REQUEST_BODY_ACCESS ASSIGN OPTION;
action_non_disruptive_ctl_request_body_processor:
	action_non_disruptive_ctl_request_body_processor_url_encode
	| action_non_disruptive_ctl_request_body_processor_multi_part
	| action_non_disruptive_ctl_request_body_processor_xml
	| action_non_disruptive_ctl_request_body_processor_json;
action_non_disruptive_ctl_request_body_processor_url_encode:
	CTL_REQUEST_BODY_PROCESSOR ASSIGN URLENCODED;
action_non_disruptive_ctl_request_body_processor_multi_part:
	CTL_REQUEST_BODY_PROCESSOR ASSIGN MULTIPART;
action_non_disruptive_ctl_request_body_processor_xml:
	CTL_REQUEST_BODY_PROCESSOR ASSIGN XML;
action_non_disruptive_ctl_request_body_processor_json:
	CTL_REQUEST_BODY_PROCESSOR ASSIGN JSON;
action_non_disruptive_ctl_rule_engine:
	CTL_RULE_ENGINE ASSIGN OPTION;
action_non_disruptive_ctl_rule_remove_by_id:
	CTL_RULE_REMOVE_BY_ID ASSIGN (INT | INT_RANGE);
action_non_disruptive_ctl_rule_remove_by_tag:
	CTL_RULE_REMOVE_BY_TAG ASSIGN STRING;
action_non_disruptive_ctl_rule_remove_target_by_id:
	CTL_RULE_REMOVE_TARGET_BY_ID ASSIGN INT SEMICOLON variables;
action_non_disruptive_ctl_rule_remove_target_by_tag:
	CTL_RULE_REMOVE_TARGET_BY_TAG ASSIGN STRING SEMICOLON variables;

action_non_disruptive_audit_log: Auditlog;
action_non_disruptive_log: Log;
action_non_disruptive_no_audit_log: Noauditlog;
action_non_disruptive_no_log: Nolog;
action_non_disruptive_logdata:
	Logdata COLON SINGLE_QUOTE string_with_macro SINGLE_QUOTE;
action_non_disruptive_capture: Capture;
action_non_disruptive_multi_match: MultiMatch;
action_non_disruptive_initcol:
	Initcol COLON persistent_storage_collection ASSIGN string_with_macro;
persistent_storage_collection:
	INIT_COL_GLOBAL
	| INIT_COL_RESOURCE
	| INIT_COL_IP
	| INIT_COL_SESSION
	| INIT_COL_USER;

action_disruptive:
	action_disruptive_allow
	| action_disruptive_block
	| action_disruptive_deny
	| action_disruptive_drop
	| action_disruptive_pass
	| action_disruptive_redirect;
action_disruptive_allow: Allow | AllowPhase | AllowRequest;
action_disruptive_block: Block;
action_disruptive_deny: Deny;
action_disruptive_drop: Drop;
action_disruptive_pass: Pass;
action_disruptive_redirect: Redirect COLON STRING;

action_data: action_data_status | action_data_xml_ns;
action_data_status: Status COLON INT;
action_data_xml_ns: Xmlns COLON STRING;

action_flow:
	action_flow_chain
	| action_flow_skip
	| action_flow_skip_after;
action_flow_chain: Chain;
action_flow_skip: Skip COLON INT;
action_flow_skip_after: SkipAfter COLON STRING;

audit_log_config:
	sec_audit_engine
	| sec_audit_log
	| sec_audit_log2
	| sec_audit_log_dir_mode
	| sec_audit_log_format
	| sec_audit_log_file_mode
	| sec_audit_log_parts
	| sec_audit_log_relevant_status
	| sec_audit_log_storage_dir
	| sec_audit_log_type
	| sec_component_signature;
sec_audit_engine: SecAuditEngine AUDIT_ENGINE;
sec_audit_log: SecAuditLog ((QUOTE STRING QUOTE) | STRING);
sec_audit_log2: SecAuditLog2 ((QUOTE STRING QUOTE) | STRING);
sec_audit_log_dir_mode: SecAuditLogDirMode OCTAL;
sec_audit_log_format: SecAuditLogFormat AUDIT_FORMAT;
sec_audit_log_file_mode: SecAuditLogFileMode OCTAL;
sec_audit_log_parts: SecAuditLogParts AUDIT_PARTS;
sec_audit_log_relevant_status:
	SecAuditLogRelevantStatus ((QUOTE STRING QUOTE) | STRING);
sec_audit_log_storage_dir:
	SecAuditLogStorageDir ((QUOTE STRING QUOTE) | STRING);
sec_audit_log_type: SecAuditLogType AUDIT_TYPE;
sec_component_signature:
	SecComponentSignature ((QUOTE STRING QUOTE) | STRING);

extension_directive:
	sec_rule_update_operator_by_id
	| sec_rule_update_operator_by_tag;
sec_rule_update_operator_by_id:
	SecRuleUpdateOperatorById (
		INT
		| INT_RANGE
		| ID_AND_CHAIN_INDEX
	) (INT | INT_RANGE | ID_AND_CHAIN_INDEX)* QUOTE operator QUOTE;
sec_rule_update_operator_by_tag:
	SecRuleUpdateOperatorByTag QUOTE STRING QUOTE QUOTE operator QUOTE;