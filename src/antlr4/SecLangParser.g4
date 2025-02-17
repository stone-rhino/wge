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
	)* EOF;

include: Include QUOTE? STRING QUOTE?;

engine_config:
	sec_reqeust_body_access
	| sec_response_body_access
	| sec_rule_engine
	| sec_tmp_save_uploaded_files
	| sec_upload_keep_files
	| sec_xml_external_entity;
sec_reqeust_body_access: SecRequestBodyAccess OPTION;
sec_response_body_access: SecResponseBodyAccess OPTION;
sec_rule_engine: SecRuleEngine OPTION;
sec_tmp_save_uploaded_files: SecTmpSaveUploadedFiles OPTION;
sec_upload_keep_files: SecUploadKeepFiles OPTION;
sec_xml_external_entity: SecXmlExternalEntity OPTION;

engine_action: sec_action;
sec_action: SecAction QUOTE action ( COMMA action)* QUOTE;

rule_directive:
	sec_rule
	| sec_rule_remove_by_id
	| sec_rule_remove_by_msg
	| sec_rule_remove_by_tag
	| sec_rule_update_action_by_id
	| sec_rule_update_target_by_id
	| sec_rule_update_target_by_msg
	| sec_rule_update_target_by_tag
	| sec_marker
	| sec_default_action;
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
sec_default_action: SecDefaultAction QUOTE action QUOTE;

sec_rule:
	SecRule variables QUOTE operator QUOTE QUOTE action (
		COMMA? action
	)* QUOTE;

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
	| variable_xml;
variable_args: NOT? VAR_COUNT? VAR_ARGS (COLON STRING)?;
variable_args_combined_size:
	NOT? VAR_COUNT? VAR_ARGS_COMBINED_SIZE (COLON STRING)?;
variable_args_get: NOT? VAR_COUNT? VAR_ARGS_GET (COLON STRING)?;
variable_args_get_names:
	NOT? VAR_COUNT? VAR_ARGS_GET_NAMES (COLON STRING)?;
variable_args_names:
	NOT? VAR_COUNT? VAR_ARGS_NAMES (COLON STRING)?;
variable_args_post:
	NOT? VAR_COUNT? VAR_ARGS_POST (COLON STRING)?;
variable_args_post_names:
	NOT? VAR_COUNT? VAR_ARGS_POST_NAMES (COLON STRING)?;
variable_auth_type:
	NOT? VAR_COUNT? VAR_AUTH_TYPE (COLON STRING)?;
variable_duration: NOT? VAR_COUNT? VAR_DURATION (COLON STRING)?;
variable_env: NOT? VAR_COUNT? VAR_ENV (COLON STRING)?;
variable_files: NOT? VAR_COUNT? VAR_FILES (COLON STRING)?;
variable_files_combined_size:
	NOT? VAR_COUNT? VAR_FILES_COMBINED_SIZE (COLON STRING)?;
variable_files_names:
	NOT? VAR_COUNT? VAR_FILES_NAMES (COLON STRING)?;
variable_full_request:
	NOT? VAR_COUNT? VAR_FULL_REQUEST (COLON STRING)?;
variable_full_request_length:
	NOT? VAR_COUNT? VAR_FULL_REQUEST_LENGTH (COLON STRING)?;
variable_files_sizes:
	NOT? VAR_COUNT? VAR_FILES_SIZES (COLON STRING)?;
variable_files_tmpnames:
	NOT? VAR_COUNT? VAR_FILES_TMPNAMES (COLON STRING)?;
variable_files_tmp_content:
	NOT? VAR_COUNT? VAR_FILES_TMP_CONTENT (COLON STRING)?;
variable_geo: NOT? VAR_COUNT? VAR_GEO (COLON STRING)?;
variable_highest_severity:
	NOT? VAR_COUNT? VAR_HIGHEST_SEVERITY (COLON STRING)?;
variable_inbound_data_error:
	NOT? VAR_COUNT? VAR_INBOUND_DATA_ERROR (COLON STRING)?;
variable_matched_var:
	NOT? VAR_COUNT? VAR_MATCHED_VAR (COLON STRING)?;
variable_matched_vars:
	NOT? VAR_COUNT? VAR_MATCHED_VARS (COLON STRING)?;
variable_matched_var_name:
	NOT? VAR_COUNT? VAR_MATCHED_VAR_NAME (COLON STRING)?;
variable_matched_vars_names:
	NOT? VAR_COUNT? VAR_MATCHED_VARS_NAMES (COLON STRING)?;
variable_modsec_build:
	NOT? VAR_COUNT? VAR_MODSEC_BUILD (COLON STRING)?;
variable_msc_pcre_limits_exceeded:
	NOT? VAR_COUNT? VAR_MSC_PCRE_LIMITS_EXCEEDED (COLON STRING)?;
variable_multipart_crlf_lf_lines:
	NOT? VAR_COUNT? VAR_MULTIPART_CRLF_LF_LINES (COLON STRING)?;
variable_multipart_filename:
	NOT? VAR_COUNT? VAR_MULTIPART_FILENAME (COLON STRING)?;
variable_multipart_name:
	NOT? VAR_COUNT? VAR_MULTIPART_NAME (COLON STRING)?;
variable_multipart_part_headers:
	NOT? VAR_COUNT? VAR_MULTIPART_PART_HEADERS (COLON STRING)?;
variable_multipart_strict_error:
	NOT? VAR_COUNT? VAR_MULTIPART_STRICT_ERROR (COLON STRING)?;
variable_multipart_unmatched_boundary:
	NOT? VAR_COUNT? VAR_MULTIPART_UNMATCHED_BOUNDARY (
		COLON STRING
	)?;
variable_outbound_data_error:
	NOT? VAR_COUNT? VAR_OUTBOUND_DATA_ERROR (COLON STRING)?;
variable_path_info:
	NOT? VAR_COUNT? VAR_PATH_INFO (COLON STRING)?;
variable_query_string:
	NOT? VAR_COUNT? VAR_QUERY_STRING (COLON STRING)?;
variable_remote_addr:
	NOT? VAR_COUNT? VAR_REMOTE_ADDR (COLON STRING)?;
variable_remote_host:
	NOT? VAR_COUNT? VAR_REMOTE_HOST (COLON STRING)?;
variable_remote_port:
	NOT? VAR_COUNT? VAR_REMOTE_PORT (COLON STRING)?;
variable_remote_user:
	NOT? VAR_COUNT? VAR_REMOTE_USER (COLON STRING)?;
variable_reqbody_error:
	NOT? VAR_COUNT? VAR_REQBODY_ERROR (COLON STRING)?;
variable_reqbody_error_msg:
	NOT? VAR_COUNT? VAR_REQBODY_ERROR_MSG (COLON STRING)?;
variable_reqbody_processor:
	NOT? VAR_COUNT? VAR_REQBODY_PROCESSOR (COLON STRING)?;
variable_request_basename:
	NOT? VAR_COUNT? VAR_REQUEST_BASENAME (COLON STRING)?;
variable_request_body:
	NOT? VAR_COUNT? VAR_REQUEST_BODY (COLON STRING)?;
variable_request_body_length:
	NOT? VAR_COUNT? VAR_REQUEST_BODY_LENGTH (COLON STRING)?;
variable_request_cookies:
	NOT? VAR_COUNT? VAR_REQUEST_COOKIES (COLON STRING)?;
variable_request_cookies_names:
	NOT? VAR_COUNT? VAR_REQUEST_COOKIES_NAMES (COLON STRING)?;
variable_request_filename:
	NOT? VAR_COUNT? VAR_REQUEST_FILENAME (COLON STRING)?;
variable_request_headers:
	NOT? VAR_COUNT? VAR_REQUEST_HEADERS (COLON STRING)?;
variable_request_headers_names:
	NOT? VAR_COUNT? VAR_REQUEST_HEADERS_NAMES (COLON STRING)?;
variable_request_line:
	NOT? VAR_COUNT? VAR_REQUEST_LINE (COLON STRING)?;
variable_request_method:
	NOT? VAR_COUNT? VAR_REQUEST_METHOD (COLON STRING)?;
variable_request_protocol:
	NOT? VAR_COUNT? VAR_REQUEST_PROTOCOL (COLON STRING)?;
variable_request_uri:
	NOT? VAR_COUNT? VAR_REQUEST_URI (COLON STRING)?;
variable_request_uri_raw:
	NOT? VAR_COUNT? VAR_REQUEST_URI_RAW (COLON STRING)?;
variable_response_body:
	NOT? VAR_COUNT? VAR_RESPONSE_BODY (COLON STRING)?;
variable_response_content_length:
	NOT? VAR_COUNT? VAR_RESPONSE_CONTENT_LENGTH (COLON STRING)?;
variable_response_content_type:
	NOT? VAR_COUNT? VAR_RESPONSE_CONTENT_TYPE (COLON STRING)?;
variable_response_headers:
	NOT? VAR_COUNT? VAR_RESPONSE_HEADERS (COLON STRING)?;
variable_response_headers_names:
	NOT? VAR_COUNT? VAR_RESPONSE_HEADERS_NAMES (COLON STRING)?;
variable_response_protocol:
	NOT? VAR_COUNT? VAR_RESPONSE_PROTOCOL (COLON STRING)?;
variable_response_status:
	NOT? VAR_COUNT? VAR_RESPONSE_STATUS (COLON STRING)?;
variable_rule: NOT? VAR_COUNT? VAR_RULE (COLON STRING)?;
variable_server_addr:
	NOT? VAR_COUNT? VAR_SERVER_ADDR (COLON STRING)?;
variable_server_name:
	NOT? VAR_COUNT? VAR_SERVER_NAME (COLON STRING)?;
variable_server_port:
	NOT? VAR_COUNT? VAR_SERVER_PORT (COLON STRING)?;
variable_session: NOT? VAR_COUNT? VAR_SESSION (COLON STRING)?;
variable_sessionid:
	NOT? VAR_COUNT? VAR_SESSIONID (COLON STRING)?;
variable_status_line:
	NOT? VAR_COUNT? VAR_STATUS_LINE (COLON STRING)?;
variable_time: NOT? VAR_COUNT? VAR_TIME (COLON STRING)?;
variable_time_day: NOT? VAR_COUNT? VAR_TIME_DAY (COLON STRING)?;
variable_time_epoch:
	NOT? VAR_COUNT? VAR_TIME_EPOCH (COLON STRING)?;
variable_time_hour:
	NOT? VAR_COUNT? VAR_TIME_HOUR (COLON STRING)?;
variable_time_min: NOT? VAR_COUNT? VAR_TIME_MIN (COLON STRING)?;
variable_time_mon: NOT? VAR_COUNT? VAR_TIME_MON (COLON STRING)?;
variable_time_sec: NOT? VAR_COUNT? VAR_TIME_SEC (COLON STRING)?;
variable_time_wday:
	NOT? VAR_COUNT? VAR_TIME_WDAY (COLON STRING)?;
variable_time_year:
	NOT? VAR_COUNT? VAR_TIME_YEAR (COLON STRING)?;
variable_tx: NOT? VAR_COUNT? VAR_TX (COLON STRING)?;
variable_unique_id:
	NOT? VAR_COUNT? VAR_UNIQUE_ID (COLON STRING)?;
variable_urlencoded_error:
	NOT? VAR_COUNT? VAR_URLENCODED_ERROR (COLON STRING)?;
variable_userid: NOT? VAR_COUNT? VAR_USERID (COLON STRING)?;
variable_webappid: NOT? VAR_COUNT? VAR_WEBAPPID (COLON STRING)?;
variable_xml: NOT? VAR_COUNT? VAR_XML (COLON STRING)?;

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
	| op_rx_default;
op_begins_with: AT OP_BEGINS_WITH STRING;
op_contains: AT OP_CONTAINS STRING;
op_contains_word: AT OP_CONTAINS_WORD STRING;
op_detect_sqli: AT OP_DETECT_SQLI STRING;
op_detect_xss: AT OP_DETECT_XSS STRING;
op_ends_with: AT OP_ENDS_WITH STRING;
op_fuzzy_hash: AT OP_FUZZY_HASH STRING;
op_eq: AT OP_EQ STRING;
op_ge: AT OP_GE STRING;
op_geo_lookup: AT OP_GEO_LOOKUP STRING;
op_gt: AT OP_GT STRING;
op_inspect_file: AT OP_INSPECT_FILE STRING;
op_ip_match: AT OP_IP_MATCH STRING;
op_ip_match_f: AT OP_IP_MATCH_F STRING;
op_ip_match_from_file: AT OP_IP_MATCH_FROM_FILE STRING;
op_le: AT OP_LE STRING;
op_lt: AT OP_LT STRING;
op_no_match: AT OP_NO_MATCH STRING;
op_pm: AT OP_PM STRING;
op_pmf: AT OP_PMF STRING;
op_pm_from_file: AT OP_PM_FROM_FILE STRING;
op_rbl: AT OP_RBL STRING;
op_rsub: AT OP_RSUB STRING;
op_rx: AT OP_RX STRING;
op_rx_global: AT OP_RX_GLOBAL STRING;
op_streq: AT OP_STREQ STRING;
op_strmatch: AT OP_STRMATCH STRING;
op_unconditional_match: AT OP_UNCONDITIONAL_MATCH STRING;
op_validate_byte_range: AT OP_VALIDATE_BYTE_RANGE STRING;
op_validate_dtd: AT OP_VALIDATE_DTD STRING;
op_validate_schema: AT OP_VALIDATE_SCHEMA STRING;
op_validate_url_encoding: AT OP_VALIDATE_URL_ENCODING STRING;
op_validate_utf8_encoding: AT OP_VALIDATE_UTF8_ENCODING STRING;
op_verify_cc: AT OP_VERIFY_CC STRING;
op_verify_cpf: AT OP_VERIFY_CPF STRING;
op_verify_ssn: AT OP_VERIFY_SSN STRING;
op_within: AT OP_WITHIN STRING;
op_rx_default: STRING;

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
action_meta_data_id: Id COLON INT;
action_meta_data_phase: Phase COLON INT;
action_meta_data_severity:
	Severity COLON SINGLE_QUOTE (
		action_meta_data_severity_emergency
		| action_meta_data_severity_alert
		| action_meta_data_severity_critical
		| action_meta_data_severity_error
		| action_meta_data_severity_waring
		| action_meta_data_severity_notice
		| action_meta_data_severity_info
		| action_meta_data_severity_debug
	) SINGLE_QUOTE;
action_meta_data_msg:
	Msg COLON SINGLE_QUOTE STRING SINGLE_QUOTE;
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
	Setvar COLON SINGLE_QUOTE TX DOT VAR_NAME SINGLE_QUOTE;
action_non_disruptive_setvar_create_init:
	Setvar COLON SINGLE_QUOTE TX DOT VAR_NAME ASSIGN action_non_disruptive_setvar_create_init_value
		SINGLE_QUOTE;
action_non_disruptive_setvar_create_init_value: (
		VAR_VALUE
		| (
			(
				VAR_VALUE? PER_CENT LEFT_BRACKET action_non_disruptive_setvar_macro RIGHT_BRACKET
					VAR_VALUE?
			)+
		)
	);
action_non_disruptive_setvar_remove:
	Setvar COLON SINGLE_QUOTE NOT TX DOT VAR_NAME SINGLE_QUOTE;
action_non_disruptive_setvar_increase:
	Setvar COLON SINGLE_QUOTE TX DOT VAR_NAME ASSIGN PLUS (
		VAR_VALUE
		| (
			PER_CENT LEFT_BRACKET action_non_disruptive_setvar_macro RIGHT_BRACKET
		)
	) SINGLE_QUOTE;
action_non_disruptive_setvar_decrease:
	Setvar COLON SINGLE_QUOTE TX DOT VAR_NAME ASSIGN MINUS (
		VAR_VALUE
		| (
			PER_CENT LEFT_BRACKET action_non_disruptive_setvar_macro RIGHT_BRACKET
		)
	) SINGLE_QUOTE;

action_non_disruptive_setvar_macro:
	action_non_disruptive_setvar_macro_tx
	| action_non_disruptive_setvar_macro_remote_addr
	| action_non_disruptive_setvar_macro_user_id
	| action_non_disruptive_setvar_macro_highest_severity
	| action_non_disruptive_setvar_macro_matched_var
	| action_non_disruptive_setvar_macro_matched_var_name
	| action_non_disruptive_setvar_macro_multipart_strict_error
	| action_non_disruptive_setvar_macro_rule
	| action_non_disruptive_setvar_macro_session;
action_non_disruptive_setvar_macro_tx: TX2 DOT STRING;
action_non_disruptive_setvar_macro_remote_addr: REMOTE_ADDR;
action_non_disruptive_setvar_macro_user_id: USERID;
action_non_disruptive_setvar_macro_highest_severity:
	HIGHEST_SEVERITY;
action_non_disruptive_setvar_macro_matched_var: MATCHED_VAR;
action_non_disruptive_setvar_macro_matched_var_name:
	MATCHED_VAR_NAME;
action_non_disruptive_setvar_macro_multipart_strict_error:
	MULTIPART_STRICT_ERROR;
action_non_disruptive_setvar_macro_rule: RULE DOT STRING;
action_non_disruptive_setvar_macro_session: SESSION;

action_non_disruptive_setenv:
	Setenv COLON SINGLE_QUOTE VAR_NAME ASSIGN (
		VAR_VALUE
		| (
			PER_CENT LEFT_BRACKET action_non_disruptive_setvar_macro RIGHT_BRACKET
		)
	) SINGLE_QUOTE;

action_non_disruptive_setuid:
	Setuid COLON (
		(SINGLE_QUOTE STRING SINGLE_QUOTE)
		| (
			PER_CENT LEFT_BRACKET action_non_disruptive_setvar_macro RIGHT_BRACKET
		)
	);
action_non_disruptive_setrsc:
	Setrsc COLON (
		(SINGLE_QUOTE STRING SINGLE_QUOTE)
		| (
			PER_CENT LEFT_BRACKET action_non_disruptive_setvar_macro RIGHT_BRACKET
		)
	);
action_non_disruptive_setsid:
	Setsid COLON (
		(SINGLE_QUOTE STRING SINGLE_QUOTE)
		| (
			PER_CENT LEFT_BRACKET action_non_disruptive_setvar_macro RIGHT_BRACKET
		)
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
action_non_disruptive_capture: Capture;
action_non_disruptive_multi_match: MultiMatch;
action_non_disruptive_initcol:
	Initcol COLON STRING ASSIGN STRING;

action_disruptive:
	action_disruptive_allow
	| action_disruptive_block
	| action_disruptive_deny
	| action_disruptive_drop
	| action_disruptive_pass
	| action_disruptive_redirect;
action_disruptive_allow: Allow;
action_disruptive_block: Block;
action_disruptive_deny: Deny;
action_disruptive_drop: Drop;
action_disruptive_pass: Pass;
action_disruptive_redirect: Redirect COLON STRING;

action_data: action_data_status | action_data_xml_ns;
action_data_status: Status COLON INT;
action_data_xml_ns: Xmlns COLON STRING;

action_flow: action_flow_chain | action_flow_skip_after;
action_flow_chain: Chain;
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