parser grammar SecLangParser;

options {
	tokenVocab = SecLangLexer;
}

configuration: ( include | engine_config | rule_directive)* EOF;

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

sec_rule:
	SecRule variables QUOTE operator QUOTE QUOTE action (
		COMMA action
	)* QUOTE;

variables: variable (PIPE variable)*;

variable: NOT? VAR_COUNT? var_main_name (COLON STRING)?;

var_main_name:
	ARGS
	| ARGS_COMBINED_SIZE
	| ARGS_GET
	| ARGS_GET_NAMES
	| ARGS_NAMES
	| ARGS_POST
	| ARGS_POST_NAMES
	| AUTH_TYPE
	| DURATION
	| ENV
	| FILES
	| FILES_COMBINED_SIZE
	| FILES_NAMES
	| FULL_REQUEST
	| FULL_REQUEST_LENGTH
	| FILES_SIZES
	| FILES_TMPNAMES
	| FILES_TMP_CONTENT
	| GEO
	| HIGHEST_SEVERITY
	| INBOUND_DATA_ERROR
	| MATCHED_VAR
	| MATCHED_VARS
	| MATCHED_VAR_NAME
	| MATCHED_VARS_NAMES
	| MODSEC_BUILD
	| MSC_PCRE_LIMITS_EXCEEDED
	| MULTIPART_CRLF_LF_LINES
	| MULTIPART_FILENAME
	| MULTIPART_NAME
	| MULTIPART_PART_HEADERS
	| MULTIPART_STRICT_ERROR
	| MULTIPART_UNMATCHED_BOUNDARY
	| OUTBOUND_DATA_ERROR
	| PATH_INFO
	| QUERY_STRING
	| REMOTE_ADDR
	| REMOTE_HOST
	| REMOTE_USER
	| REQBODY_ERROR
	| REQBODY_ERROR_MSG
	| REQBODY_PROCESSOR
	| REQUEST_BASENAME
	| REQUEST_BODY
	| REQUEST_BODY_LENGTH
	| REQUEST_COOKIES
	| REQUEST_COOKIES_NAMES
	| REQUEST_FILENAME
	| REQUEST_HEADERS
	| REQUEST_HEADERS_NAMES
	| REQUEST_LINE
	| REQUEST_METHOD
	| REQUEST_PROTOCOL
	| REQUEST_URI
	| REQUEST_URI_RAW
	| RESPONSE_BODY
	| RESPONSE_CONTENT_LENGTH
	| RESPONSE_CONTENT_TYPE
	| RESPONSE_HEADERS
	| RESPONSE_HEADERS_NAMES
	| RESPONSE_PROTOCOL
	| RESPONSE_STATUS
	| RULE
	| SERVER_ADDR
	| SERVER_NAME
	| SERVER_PORT
	| SESSION
	| SESSIONID
	| STATUS_LINE
	| TIME
	| TIME_DAY
	| TIME_EPOCH
	| TIME_HOUR
	| TIME_MIN
	| TIME_MON
	| TIME_SEC
	| TIME_WDAY
	| TIME_YEAR
	| TX
	| UNIQUE_ID
	| URLENCODED_ERROR
	| USERID
	| WEBAPPID;

operator: (AT operator_name)? operator_value;

operator_name:
	BeginsWith
	| Contains
	| ContainsWord
	| DetectSQLi
	| DetectXSS
	| EndsWith
	| FuzzyHash
	| Eq
	| Ge
	| GeoLookup
	| Gt
	| InspectFile
	| IpMatch
	| IpMatchF
	| IpMatchFromFile
	| Le
	| Lt
	| NoMatch
	| Pm
	| Pmf
	| PmFromFile
	| Rbl
	| Rsub
	| Rx
	| RxGlobal
	| Streq
	| Strmatch
	| UnconditionalMatch
	| ValidateByteRange
	| ValidateDTD
	| ValidateSchema
	| ValidateUrlEncoding
	| ValidateUtf8Encoding
	| VerifyCC
	| VerifyCPF
	| VerifySSN
	| Within;

operator_value: STRING;

action: action_name (COLON action_value)?;

action_name:
	Accuracy
	| Allow
	| Auditlog
	| Block
	| Capture
	| Chain
	| Ctl
	| Deny
	| Drop
	| Exec
	| Expirevar
	| Id
	| Initcol
	| Log
	| Logdata
	| Maturity
	| Msg
	| MultiMatch
	| Noauditlog
	| Nolog
	| Pass
	| Phase
	| Redirect
	| Rev
	| Severity
	| Setuid
	| Setrsc
	| Setsid
	| Setenv
	| Setvar
	| Skip
	| SkipAfter
	| Status
	| T
	| Tag
	| Ver
	| Xmlns;

action_value: STRING;

sec_rule_remove_by_id:
	SecRuleRemoveById (INT | INT_RANGE) (INT | INT_RANGE)*;

sec_rule_remove_by_msg: SecRuleRemoveByMsg QUOTE STRING QUOTE;

sec_rule_remove_by_tag: SecRuleRemoveByTag QUOTE STRING QUOTE;

sec_rule_update_action_by_id: SecRuleUpdateActionById action;

sec_rule_update_target_by_id:
	SecRuleUpdateTargetById (INT | INT_RANGE) (INT | INT_RANGE)*;

sec_rule_update_target_by_msg:
	SecRuleUpdateTargetByMsg (QUOTE STRING QUOTE)
	| STRING;

sec_rule_update_target_by_tag:
	SecRuleUpdateTargetByTag (QUOTE STRING QUOTE)
	| STRING;