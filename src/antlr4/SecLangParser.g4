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

sec_rule:
	SecRule variables QUOTE operator QUOTE QUOTE action (
		COMMA action
	)* QUOTE;

variables: variable (PIPE variable)*;

variable: NOT? VAR_COUNT? VAR_MAIN_NAME (COLON STRING)?;

operator: (AT OPERATOR_NAME)? operator_value;

operator_value: STRING;

action: ACTION_NAME (COLON action_value)?;

action_value: STRING;

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
	| sec_audit_log_type;

sec_audit_engine: SecAuditEngine AUDIT_ENGINE;
sec_audit_log: SecAuditLog STRING;
sec_audit_log2: SecAuditLog2 STRING;
sec_audit_log_dir_mode: SecAuditLogDirMode INT;
sec_audit_log_format: SecAuditLogFormat AUDIT_FORMAT;
sec_audit_log_file_mode: SecAuditLogFileMode INT;
sec_audit_log_parts: SecAuditLogParts AUDIT_PARTS;
sec_audit_log_relevant_status: SecAuditLogRelevantStatus STRING;
sec_audit_log_storage_dir: SecAuditLogStorageDir STRING;
sec_audit_log_type: SecAuditLogType AUDIT_TYPE;