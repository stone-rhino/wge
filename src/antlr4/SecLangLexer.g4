lexer grammar SecLangLexer;

tokens{
	QUOTE,
	COMMA,
	NOT,
	STRING,
	OPTION,
	INT,
	PIPE,
	COLON,
	VAR_COUNT,
	VAR_MAIN_NAME
}

WS: [ \t\r\n]+ -> skip;
COMMENT: '#' ~[\r\n]* '\r'? '\n'? -> skip;
QUOTE: '"';
NOT: '!';
INT_RANGE: INT '-' INT;
INT: [0-9]+;
OPTION: ('On' | 'Off');

Include: 'Include' -> pushMode(ModeInclude);
SecAction: 'SecAction';
SecArgumentSeparator: 'SecArgumentSeparator';
SecArgumentsLimit: 'SecArgumentsLimit';
SecAuditEngine: 'SecAuditEngine';
SecAuditLog: 'SecAuditLog';
SecAuditLog2: 'SecAuditLog2';
SecAuditLogDirMode: 'SecAuditLogDirMode';
SecAuditLogFormat: 'SecAuditLogFormat';
SecAuditLogFileMode: 'SecAuditLogFileMode';
SecAuditLogParts: 'SecAuditLogParts';
SecAuditLogRelevantStatus: 'SecAuditLogRelevantStatus';
SecAuditLogStorageDir: 'SecAuditLogStorageDir';
SecAuditLogType: 'SecAuditLogType';
SecComponentSignature: 'SecComponentSignature';
SecDebugLog: 'SecDebugLog';
SecDebugLogLevel: 'SecDebugLogLevel';
SecDefaultAction: 'SecDefaultAction';
SecGeoLookupDb: 'SecGeoLookupDb';
SecHttpBlKey: 'SecHttpBlKey';
SecMarker: 'SecMarker';
SecPcreMatchLimit: 'SecPcreMatchLimit';
SecRemoteRules: 'SecRemoteRules';
SecRemoteRulesFailAction: 'SecRemoteRulesFailAction';
SecRequestBodyAccess: 'SecRequestBodyAccess';
SecRequestBodyInMemoryLimit: 'SecRequestBodyInMemoryLimit';
SecRequestBodyJsonDepthLimit: 'SecRequestBodyJsonDepthLimit';
SecRequestBodyLimit: 'SecRequestBodyLimit';
SecRequestBodyNoFilesLimit: 'SecRequestBodyNoFilesLimit';
SecRequestBodyLimitAction: 'SecRequestBodyLimitAction';
SecResponseBodyLimit: 'SecResponseBodyLimit';
SecResponseBodyLimitAction: 'SecResponseBodyLimitAction';
SecResponseBodyMimeType: 'SecResponseBodyMimeType';
SecResponseBodyMimeTypesClear: 'SecResponseBodyMimeTypesClear';
SecResponseBodyAccess: 'SecResponseBodyAccess';
SecRuleEngine: 'SecRuleEngine' -> pushMode(ModeRuleEngine);
SecRuleRemoveById: 'SecRuleRemoveById';
SecRuleRemoveByMsg:
	'SecRuleRemoveByMsg' -> pushMode(ModeRuleRemoveByMsg);
SecRuleRemoveByTag:
	'SecRuleRemoveByTag' -> pushMode(ModeRuleRemoveByTag);
SecRuleScript: 'SecRuleScript';
SecRuleUpdateActionById:
	'SecRuleUpdateActionById' -> pushMode(ModeRuleUpdateActionById);
SecRuleUpdateTargetById:
	'SecRuleUpdateTargetById' -> pushMode(ModeRuleUpdateTarget);
SecRuleUpdateTargetByMsg: 'SecRuleUpdateTargetByMsg';
SecRuleUpdateTargetByTag: 'SecRuleUpdateTargetByTag';
SecRule: 'SecRule' -> pushMode(ModeSecRuleVariable);
SecTmpDir: 'SecTmpDir';
SecTmpSaveUploadedFiles: 'SecTmpSaveUploadedFiles';
SecUnicodeMapFile: 'SecUnicodeMapFile';
SecUploadDir: 'SecUploadDir';
SecUploadFileLimit: 'SecUploadFileLimit';
SecUploadFileMode: 'SecUploadFileMode';
SecUploadKeepFiles: 'SecUploadKeepFiles';
SecWebAppId: 'SecWebAppId';
SecXmlExternalEntity: 'SecXmlExternalEntity';

VAR_MAIN_NAME:
	'ARGS'
	| 'ARGS_COMBINED_SIZE'
	| 'ARGS_GET'
	| 'ARGS_GET_NAMES'
	| 'ARGS_NAMES'
	| 'ARGS_POST'
	| 'ARGS_POST_NAMES'
	| 'AUTH_TYPE'
	| 'DURATION'
	| 'ENV'
	| 'FILES'
	| 'FILES_COMBINED_SIZE'
	| 'FILES_NAMES'
	| 'FULL_REQUEST'
	| 'FULL_REQUEST_LENGTH'
	| 'FILES_SIZES'
	| 'FILES_TMPNAMES'
	| 'FILES_TMP_CONTENT'
	| 'GEO'
	| 'HIGHEST_SEVERITY'
	| 'INBOUND_DATA_ERROR'
	| 'MATCHED_VAR'
	| 'MATCHED_VARS'
	| 'MATCHED_VAR_NAME'
	| 'MATCHED_VARS_NAMES'
	| 'MODSEC_BUILD'
	| 'MSC_PCRE_LIMITS_EXCEEDED'
	| 'MULTIPART_CRLF_LF_LINES'
	| 'MULTIPART_FILENAME'
	| 'MULTIPART_NAME'
	| 'MULTIPART_PART_HEADERS'
	| 'MULTIPART_STRICT_ERROR'
	| 'MULTIPART_UNMATCHED_BOUNDARY'
	| 'OUTBOUND_DATA_ERROR'
	| 'PATH_INFO'
	| 'QUERY_STRING'
	| 'REMOTE_ADDR'
	| 'REMOTE_HOST'
	| 'REMOTE_USER'
	| 'REQBODY_ERROR'
	| 'REQBODY_ERROR_MSG'
	| 'REQBODY_PROCESSOR'
	| 'REQUEST_BASENAME'
	| 'REQUEST_BODY'
	| 'REQUEST_BODY_LENGTH'
	| 'REQUEST_COOKIES'
	| 'REQUEST_COOKIES_NAMES'
	| 'REQUEST_FILENAME'
	| 'REQUEST_HEADERS'
	| 'REQUEST_HEADERS_NAMES'
	| 'REQUEST_LINE'
	| 'REQUEST_METHOD'
	| 'REQUEST_PROTOCOL'
	| 'REQUEST_URI'
	| 'REQUEST_URI_RAW'
	| 'RESPONSE_BODY'
	| 'RESPONSE_CONTENT_LENGTH'
	| 'RESPONSE_CONTENT_TYPE'
	| 'RESPONSE_HEADERS'
	| 'RESPONSE_HEADERS_NAMES'
	| 'RESPONSE_PROTOCOL'
	| 'RESPONSE_STATUS'
	| 'RULE'
	| 'SERVER_ADDR'
	| 'SERVER_NAME'
	| 'SERVER_PORT'
	| 'SESSION'
	| 'SESSIONID'
	| 'STATUS_LINE'
	| 'TIME'
	| 'TIME_DAY'
	| 'TIME_EPOCH'
	| 'TIME_HOUR'
	| 'TIME_MIN'
	| 'TIME_MON'
	| 'TIME_SEC'
	| 'TIME_WDAY'
	| 'TIME_YEAR'
	| 'TX'
	| 'UNIQUE_ID'
	| 'URLENCODED_ERROR'
	| 'USERID'
	| 'WEBAPPID';

mode ModeInclude;
ModeInclude_WS: ' ' -> skip;
ModeInclude_QUOTE: '"' -> type(QUOTE);
IncludeFilePath:
	[a-zA-Z0-9/._~|\\:-]+ -> type(STRING), popMode;

mode ModeRuleEngine;
ModeEngineConfig_WS: ' ' -> skip;
ModeRuleEngine_OPTION: ('On' | 'Off' | 'DetectionOnly') -> type(OPTION), popMode;

mode ModeRuleRemoveByMsg;
ModeRuleRemoveByMsg_WS: ' ' -> skip;
ModeRuleRemoveByMsg_QUOTE: '"' -> type(QUOTE);
ModeRuleRemoveByMsg_STRING:
	('\\"' | ~["])+ -> type(STRING), popMode;

mode ModeRuleRemoveByTag;
ModeRuleRemoveByTag_WS: ' ' -> skip;
ModeRuleRemoveByTag_QUOTE: '"' -> type(QUOTE);
ModeRuleRemoveByTag_STRING: ('\\"' | ~["])+ -> type(STRING), popMode;

mode ModeRuleUpdateActionById;
ModeRuleUpdateActionById_WS: ' ' -> skip;
ModeRuleUpdateActionById_INT:
	[0-9]+ -> type(INT), pushMode(ModeSecRuleAction);

mode ModeRuleUpdateTarget;
ModeRuleUpdateTarget_WS: ' ' -> skip;
ModeRuleUpdateTarget_PIPE: '|' -> type(PIPE);
ModeRuleUpdateTarget_COLON: ':' -> type(COLON);
ModeRuleUpdateTarget_VAR_COUNT: '&' -> type(VAR_COUNT);
ModeRuleUpdateTarget_VAR_NOT: '!' -> type(NOT);
ModeRuleUpdateTarget_INT: [0-9]+ -> type(INT);
ModeRuleUpdateTarget_VAR_MAIN_NAME:
	VAR_MAIN_NAME -> type(VAR_MAIN_NAME);
ModeRuleUpdateTargetById_VAR_SUB_NAME:
	~[ :!&|",\n]+ -> type(STRING);

mode ModeRuleUpdateTargetByMsg;
ModeRuleUpdateTargetByMsg_WS: ' ' -> skip;
ModeRuleUpdateTargetByMsg_QUOTE:
	'"' -> type(QUOTE), pushMode(ModeSecRuleVariable);

mode ModeRuleUpdateTargetByTag;
ModeRuleUpdateTargetByTag_WS: ' ' -> skip;
ModeRuleUpdateTargetByTag_QUOTE:
	'"' -> type(QUOTE), pushMode(ModeSecRuleVariable);

mode ModeSecRuleVariable;
ModeSecRuleVariable_WS:
	[ \t]+ -> skip, popMode, pushMode(ModeSecRuleVariableName);

mode ModeSecRuleVariableName;
ModeSecRuleVariableName_WS:
	[ \t] -> skip, popMode, pushMode(ModeSecRuleOperator);
ModeSecRuleVariableName_PIPE: '|' -> type(PIPE);
ModeSecRuleVariableName_COLON: ':' -> type(COLON);
ModeSecRuleVariableName_VAR_COUNT: '&' -> type(VAR_COUNT);
ModeSecRuleVariableName_VAR_NOT: '!' -> type(NOT);
ModeSecRuleVariableName_VAR_MAIN_NAME:
	VAR_MAIN_NAME -> type(VAR_MAIN_NAME);
ModeSecRuleVariableName_VAR_SUB_NAME:
	~[ :!&|",\n]+ -> type(STRING);

mode ModeSecRuleOperator;
ModeSecRuleOperator_QUOTE: '"' -> type(QUOTE);
AT: '@';
OPERATOR_NAME:
	'beginsWith'
	| 'contains'
	| 'containsWord'
	| 'detectSQLi'
	| 'detectXSS'
	| 'endsWith'
	| 'fuzzyHash'
	| 'eq'
	| 'ge'
	| 'geoLookup'
	| 'gt'
	| 'inspectFile'
	| 'ipMatch'
	| 'ipMatchF'
	| 'ipMatchFromFile'
	| 'le'
	| 'lt'
	| 'noMatch'
	| 'pm'
	| 'pmf'
	| 'pmFromFile'
	| 'rbl'
	| 'rsub'
	| 'rx'
	| 'rxGlobal'
	| 'streq'
	| 'strmatch'
	| 'unconditionalMatch'
	| 'validateByteRange'
	| 'validateDTD'
	| 'validateSchema'
	| 'validateUrlEncoding'
	| 'validateUtf8Encoding'
	| 'verifyCC'
	| 'verifyCPF'
	| 'verifySSN'
	| 'within';
ModeSecRuleOperator_WS:
	[ \t]+ -> skip, popMode, pushMode(ModeSecRuleOperatorValue);
OPERATOR_VALUE: ('\\"' | ~["])+ -> type(STRING), popMode, pushMode(ModeSecRuleAction);

mode ModeSecRuleOperatorValue;
OPERATOR_VALUE2: ('\\"' | ~["])+ -> type(STRING), popMode, pushMode(ModeSecRuleAction);

mode ModeSecRuleAction;
ModeSecRuleAction_WS: [ \t]+ -> skip;
ModeSecRuleAction_QUOTE: '"' -> type(QUOTE);
ModeSecRuleAction_COLON: ':' -> type(COLON);
ModeSecRuleAction_COMMA: ',' -> type(COMMA);
ModeSecRuleAction_EOF: ('\r'? ('\n' | EOF)) -> skip, popMode;
ACTION_NAME:
	'accuracy'
	| 'allow'
	| 'auditlog'
	| 'block'
	| 'capture'
	| 'chain'
	| 'ctl'
	| 'deny'
	| 'drop'
	| 'exec'
	| 'expirevar'
	| 'id'
	| 'initcol'
	| 'log'
	| 'logdata'
	| 'maturity'
	| 'msg'
	| 'multiMatch'
	| 'noauditlog'
	| 'nolog'
	| 'pass'
	| 'phase'
	| 'redirect'
	| 'rev'
	| 'severity'
	| 'setuid'
	| 'setrsc'
	| 'setsid'
	| 'setenv'
	| 'setvar'
	| 'skip'
	| 'skipAfter'
	| 'status'
	| 't'
	| 'tag'
	| 'ver'
	| 'xmlns';
ACTION_VALUE: ~[ :",\n]+ -> type(STRING);