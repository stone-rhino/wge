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

variable: NOT? VAR_COUNT? VAR_MAIN_NAME (COLON STRING)?;

operator: (AT OPERATOR_NAME)? operator_value;

operator_value: STRING;

action: action_meta_data | action_non_disruptive;

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
action_meta_data_severity: Severity COLON SeverityEnum;
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

action_non_disruptive:
	action_non_disruptive_setvar
	| action_non_disruptive_setenv;
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
action_non_disruptive_setvar_macro_tx: TX DOT VAR_NAME;
action_non_disruptive_setvar_macro_remote_addr: REMOTE_ADDR;
action_non_disruptive_setvar_macro_user_id: USERID;
action_non_disruptive_setvar_macro_highest_severity:
	HIGHEST_SEVERITY;
action_non_disruptive_setvar_macro_matched_var: MATCHED_VAR;
action_non_disruptive_setvar_macro_matched_var_name:
	MATCHED_VAR_NAME;
action_non_disruptive_setvar_macro_multipart_strict_error:
	MULTIPART_STRICT_ERROR;
action_non_disruptive_setvar_macro_rule: RULE DOT VAR_NAME;
action_non_disruptive_setvar_macro_session: SESSION;

action_non_disruptive_setenv:
	Setenv COLON VAR_NAME ASSIGN (
		VAR_VALUE
		| (
			PER_CENT LEFT_BRACKET action_non_disruptive_setvar_macro RIGHT_BRACKET
		)
	);

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