parser grammar SecLangParser;

options {
	tokenVocab = SecLangLexer;
}

configuration: (
		include
		| engine_config
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

rule_directive:
	sec_rule
	| sec_rule_remove_by_id
	| sec_rule_remove_by_msg
	| sec_rule_remove_by_tag
	| sec_rule_update_action_by_id
	| sec_rule_update_target_by_id
	| sec_rule_update_target_by_msg
	| sec_rule_update_target_by_tag;
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

sec_rule:
	SecRule variables QUOTE operator QUOTE QUOTE action (
		COMMA action
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
	| op_rx2;
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
op_rx2: STRING;

action:
	action_meta_data
	| action_non_disruptive
	| action_disruptive
	| action_data;

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
	| action_non_disruptive_audit_log
	| action_non_disruptive_log
	| action_non_disruptive_no_audit_log
	| action_non_disruptive_no_log
	| action_non_disruptive_capture
	| action_non_disruptive_multi_match;
action_non_disruptive_setvar:
	action_non_disruptive_setvar_create
	| action_non_disruptive_setvar_create_init
	| action_non_disruptive_setvar_remove
	| action_non_disruptive_setvar_increase
	| action_non_disruptive_setvar_decrease;
action_non_disruptive_setvar_create:
	Setvar COLON TX DOT VAR_NAME;
action_non_disruptive_setvar_create_init:
	Setvar COLON TX DOT VAR_NAME ASSIGN (
		VAR_VALUE
		| (
			PER_CENT LEFT_BRACKET action_non_disruptive_setvar_macro RIGHT_BRACKET
		)
	);
action_non_disruptive_setvar_remove:
	Setvar COLON NOT TX DOT VAR_NAME;
action_non_disruptive_setvar_increase:
	Setvar COLON TX DOT VAR_NAME ASSIGN PLUS (
		VAR_VALUE
		| (
			PER_CENT LEFT_BRACKET action_non_disruptive_setvar_macro RIGHT_BRACKET
		)
	);
action_non_disruptive_setvar_decrease:
	Setvar COLON TX DOT VAR_NAME ASSIGN MINUS (
		VAR_VALUE
		| (
			PER_CENT LEFT_BRACKET action_non_disruptive_setvar_macro RIGHT_BRACKET
		)
	);

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
	Setenv COLON VAR_NAME ASSIGN (
		VAR_VALUE
		| (
			PER_CENT LEFT_BRACKET action_non_disruptive_setvar_macro RIGHT_BRACKET
		)
	);

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

action_non_disruptive_audit_log: Auditlog;
action_non_disruptive_log: Log;
action_non_disruptive_no_audit_log: Noauditlog;
action_non_disruptive_no_log: Nolog;
action_non_disruptive_capture: Capture;
action_non_disruptive_multi_match: MultiMatch;

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