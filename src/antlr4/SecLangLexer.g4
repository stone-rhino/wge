lexer grammar SecLangLexer;

tokens{
	QUOTE,
	SINGLE_QUOTE,
	COMMA,
	NOT,
	DOT,
	COLON,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	PER_CENT,
	PIPE,
	INT,
	OPTION,
	STRING,
	VAR_COUNT
}

QUOTE: '"';
SINGLE_QUOTE: '\'';
COMMA: ',';
NOT: '!';
DOT: '.';
COLON: ':';
LEFT_BRACKET: '{';
RIGHT_BRACKET: '}';
PER_CENT: '%';
PIPE: '|';

INT_RANGE: INT '-' INT;
INT: [0-9]+;
OPTION: ('On' | 'Off');
COMMENT: '#' ~[\r\n]* '\r'? '\n'? -> skip;
NL: '\\' '\r'? '\n' -> skip;
WS: (([ \t\r\n]+) | NL) -> skip;

Include: 'Include' -> pushMode(ModeInclude);
SecAction: 'SecAction';
SecArgumentSeparator: 'SecArgumentSeparator';
SecArgumentsLimit: 'SecArgumentsLimit';
SecAuditEngine: 'SecAuditEngine' -> pushMode(ModeAuditLog);
SecAuditLog: 'SecAuditLog' -> pushMode(ModeAuditLog);
SecAuditLog2: 'SecAuditLog2' -> pushMode(ModeAuditLog);
SecAuditLogDirMode:
	'SecAuditLogDirMode' -> pushMode(ModeAuditLog);
SecAuditLogFormat:
	'SecAuditLogFormat' -> pushMode(ModeAuditLog);
SecAuditLogFileMode:
	'SecAuditLogFileMode' -> pushMode(ModeAuditLog);
SecAuditLogParts: 'SecAuditLogParts' -> pushMode(ModeAuditLog);
SecAuditLogRelevantStatus:
	'SecAuditLogRelevantStatus' -> pushMode(ModeAuditLog);
SecAuditLogStorageDir:
	'SecAuditLogStorageDir' -> pushMode(ModeAuditLog);
SecAuditLogType: 'SecAuditLogType' -> pushMode(ModeAuditLog);
SecComponentSignature:
	'SecComponentSignature' -> pushMode(ModeAuditLog);
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
	'SecRuleUpdateTargetById' -> pushMode(ModeRuleUpdateTargetById);
SecRuleUpdateTargetByMsg:
	'SecRuleUpdateTargetByMsg' -> pushMode(ModeRuleUpdateTargetByMsg);
SecRuleUpdateTargetByTag:
	'SecRuleUpdateTargetByTag' -> pushMode(ModeRuleUpdateTargetByMsg);
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

mode ModeInclude;
ModeInclude_WS: WS -> skip;
ModeInclude_QUOTE: '"' -> type(QUOTE);
ModeInclude_FilePath:
	[a-zA-Z0-9/._~|\\:-]+ -> type(STRING), popMode;

mode ModeAuditLog;
ModeAuditLog_WS: WS -> skip;
ModeAuditLog_QUOTE: QUOTE -> type(QUOTE);
AUDIT_ENGINE: ('On' | 'Off' | 'RelevantOnly') -> popMode;
AUDIT_FORMAT: ('JSON' | 'Native') -> popMode;
AUDIT_PARTS: [ABCDEFGHIJKZ]+ -> popMode;
AUDIT_TYPE: ('Serial' | 'Concurrent' | 'HTTPS') -> popMode;
OCTAL: '0' [0-9]+ -> popMode;
ModeAuditLog_STRING: (('\\"') | ~([" ])) (('\\"') | ~('"'))* -> type(STRING), popMode;

mode ModeRuleEngine;
ModeEngineConfig_WS: WS -> skip;
ModeRuleEngine_OPTION: ('On' | 'Off' | 'DetectionOnly') -> type(OPTION), popMode;

mode ModeRuleRemoveByMsg;
ModeRuleRemoveByMsg_WS: WS -> skip;
ModeRuleRemoveByMsg_QUOTE: '"' -> type(QUOTE);
ModeRuleRemoveByMsg_STRING:
	(('\\"') | ~([" ])) (('\\"') | ~('"'))* -> type(STRING), popMode;

mode ModeRuleRemoveByTag;
ModeRuleRemoveByTag_WS: WS -> skip;
ModeRuleRemoveByTag_QUOTE: '"' -> type(QUOTE);
ModeRuleRemoveByTag_STRING: (('\\"') | ~([" ])) (
		('\\"')
		| ~('"')
	)* -> type(STRING), popMode;

mode ModeRuleUpdateActionById;
ModeRuleUpdateActionById_WS: WS -> skip;
ModeRuleUpdateActionById_INT:
	[0-9]+ -> type(INT), pushMode(ModeSecRuleAction);

mode ModeRuleUpdateTargetById;
ModeRuleUpdateTargetById_WS: WS -> skip;
ModeRuleUpdateTargetById_INT:
	[0-9]+ -> type(INT), popMode, pushMode(ModeSecRuleVariable);

mode ModeRuleUpdateTargetByMsg;
ModeRuleUpdateTargetByMsg_WS: WS -> skip;
ModeRuleUpdateTargetByMsg_QUOTE:
	'"' -> type(QUOTE), popMode, pushMode(ModeRuleUpdateTargetByMsgString);

mode ModeRuleUpdateTargetByMsgString;
ModeRuleUpdateTargetByMsgString_QUOTE:
	'"' -> type(QUOTE), popMode, pushMode(ModeSecRuleVariable);
ModeRuleUpdateTargetByMsgString_STRING: (('\\"') | ~([" ])) (
		('\\"')
		| ~('"')
	)* -> type(STRING);

mode ModeSecRuleVariable;
ModeSecRuleVariable_WS:
	WS -> skip, popMode, pushMode(ModeSecRuleVariableName);

mode ModeSecRuleVariableName;
VAR_ARGS: 'ARGS';
VAR_ARGS_COMBINED_SIZE: 'ARGS_COMBINED_SIZE';
VAR_ARGS_GET: 'ARGS_GET';
VAR_ARGS_GET_NAMES: 'ARGS_GET_NAMES';
VAR_ARGS_NAMES: 'ARGS_NAMES';
VAR_ARGS_POST: 'ARGS_POST';
VAR_ARGS_POST_NAMES: 'ARGS_POST_NAMES';
VAR_AUTH_TYPE: 'AUTH_TYPE';
VAR_DURATION: 'DURATION';
VAR_ENV: 'ENV';
VAR_FILES: 'FILES';
VAR_FILES_COMBINED_SIZE: 'FILES_COMBINED_SIZE';
VAR_FILES_NAMES: 'FILES_NAMES';
VAR_FULL_REQUEST: 'FULL_REQUEST';
VAR_FULL_REQUEST_LENGTH: 'FULL_REQUEST_LENGTH';
VAR_FILES_SIZES: 'FILES_SIZES';
VAR_FILES_TMPNAMES: 'FILES_TMPNAMES';
VAR_FILES_TMP_CONTENT: 'FILES_TMP_CONTENT';
VAR_GEO: 'GEO';
VAR_HIGHEST_SEVERITY: 'HIGHEST_SEVERITY';
VAR_INBOUND_DATA_ERROR: 'INBOUND_DATA_ERROR';
VAR_MATCHED_VAR: 'MATCHED_VAR';
VAR_MATCHED_VARS: 'MATCHED_VARS';
VAR_MATCHED_VAR_NAME: 'MATCHED_VAR_NAME';
VAR_MATCHED_VARS_NAMES: 'MATCHED_VARS_NAMES';
VAR_MODSEC_BUILD: 'MODSEC_BUILD';
VAR_MSC_PCRE_LIMITS_EXCEEDED: 'MSC_PCRE_LIMITS_EXCEEDED';
VAR_MULTIPART_CRLF_LF_LINES: 'MULTIPART_CRLF_LF_LINES';
VAR_MULTIPART_FILENAME: 'MULTIPART_FILENAME';
VAR_MULTIPART_NAME: 'MULTIPART_NAME';
VAR_MULTIPART_PART_HEADERS: 'MULTIPART_PART_HEADERS';
VAR_MULTIPART_STRICT_ERROR: 'MULTIPART_STRICT_ERROR';
VAR_MULTIPART_UNMATCHED_BOUNDARY:
	'MULTIPART_UNMATCHED_BOUNDARY';
VAR_OUTBOUND_DATA_ERROR: 'OUTBOUND_DATA_ERROR';
VAR_PATH_INFO: 'PATH_INFO';
VAR_QUERY_STRING: 'QUERY_STRING';
VAR_REMOTE_ADDR: 'REMOTE_ADDR';
VAR_REMOTE_HOST: 'REMOTE_HOST';
VAR_REMOTE_PORT: 'REMOTE_PORT';
VAR_REMOTE_USER: 'REMOTE_USER';
VAR_REQBODY_ERROR: 'REQBODY_ERROR';
VAR_REQBODY_ERROR_MSG: 'REQBODY_ERROR_MSG';
VAR_REQBODY_PROCESSOR: 'REQBODY_PROCESSOR';
VAR_REQUEST_BASENAME: 'REQUEST_BASENAME';
VAR_REQUEST_BODY: 'REQUEST_BODY';
VAR_REQUEST_BODY_LENGTH: 'REQUEST_BODY_LENGTH';
VAR_REQUEST_COOKIES: 'REQUEST_COOKIES';
VAR_REQUEST_COOKIES_NAMES: 'REQUEST_COOKIES_NAMES';
VAR_REQUEST_FILENAME: 'REQUEST_FILENAME';
VAR_REQUEST_HEADERS: 'REQUEST_HEADERS';
VAR_REQUEST_HEADERS_NAMES: 'REQUEST_HEADERS_NAMES';
VAR_REQUEST_LINE: 'REQUEST_LINE';
VAR_REQUEST_METHOD: 'REQUEST_METHOD';
VAR_REQUEST_PROTOCOL: 'REQUEST_PROTOCOL';
VAR_REQUEST_URI: 'REQUEST_URI';
VAR_REQUEST_URI_RAW: 'REQUEST_URI_RAW';
VAR_RESPONSE_BODY: 'RESPONSE_BODY';
VAR_RESPONSE_CONTENT_LENGTH: 'RESPONSE_CONTENT_LENGTH';
VAR_RESPONSE_CONTENT_TYPE: 'RESPONSE_CONTENT_TYPE';
VAR_RESPONSE_HEADERS: 'RESPONSE_HEADERS';
VAR_RESPONSE_HEADERS_NAMES: 'RESPONSE_HEADERS_NAMES';
VAR_RESPONSE_PROTOCOL: 'RESPONSE_PROTOCOL';
VAR_RESPONSE_STATUS: 'RESPONSE_STATUS';
VAR_RULE: 'RULE';
VAR_SERVER_ADDR: 'SERVER_ADDR';
VAR_SERVER_NAME: 'SERVER_NAME';
VAR_SERVER_PORT: 'SERVER_PORT';
VAR_SESSION: 'SESSION';
VAR_SESSIONID: 'SESSIONID';
VAR_STATUS_LINE: 'STATUS_LINE';
VAR_TIME: 'TIME';
VAR_TIME_DAY: 'TIME_DAY';
VAR_TIME_EPOCH: 'TIME_EPOCH';
VAR_TIME_HOUR: 'TIME_HOUR';
VAR_TIME_MIN: 'TIME_MIN';
VAR_TIME_MON: 'TIME_MON';
VAR_TIME_SEC: 'TIME_SEC';
VAR_TIME_WDAY: 'TIME_WDAY';
VAR_TIME_YEAR: 'TIME_YEAR';
VAR_TX: 'TX';
VAR_UNIQUE_ID: 'UNIQUE_ID';
VAR_URLENCODED_ERROR: 'URLENCODED_ERROR';
VAR_USERID: 'USERID';
VAR_WEBAPPID: 'WEBAPPID';
VAR_XML: 'XML';
ModeSecRuleVariableName_WS: WS -> skip;
ModeSecRuleVariableName_PIPE: PIPE -> type(PIPE);
ModeSecRuleVariableName_COLON:
	COLON -> type(COLON), pushMode(ModeSecRuleVariableSubName);
ModeSecRuleVariableName_VAR_COUNT: '&' -> type(VAR_COUNT);
ModeSecRuleVariableName_VAR_NOT: NOT -> type(NOT);
ModeSecRuleVariableName_QUOTE:
	QUOTE -> type(QUOTE), popMode, pushMode(ModeSecRuleOperator);

mode ModeSecRuleVariableSubName;
ModeSecRuleVariableSubName_VAR_SUB_NAME:
	~[ :!&|",\n]+ -> type(STRING), popMode;

mode ModeSecRuleOperator;
ModeSecRuleOperatorName_QUOTE:
	QUOTE -> type(QUOTE), popMode, pushMode( ModeSecRuleAction);
AT: '@';
OP_BEGINS_WITH: 'beginsWith';
OP_CONTAINS: 'contains';
OP_CONTAINS_WORD: 'containsWord';
OP_DETECT_SQLI: 'detectSQLi';
OP_DETECT_XSS: 'detectXSS';
OP_ENDS_WITH: 'endsWith';
OP_FUZZY_HASH: 'fuzzyHash';
OP_EQ: 'eq';
OP_GE: 'ge';
OP_GEO_LOOKUP: 'geoLookup';
OP_GT: 'gt';
OP_INSPECT_FILE: 'inspectFile';
OP_IP_MATCH: 'ipMatch';
OP_IP_MATCH_F: 'ipMatchF';
OP_IP_MATCH_FROM_FILE: 'ipMatchFromFile';
OP_LE: 'le';
OP_LT: 'lt';
OP_NO_MATCH: 'noMatch';
OP_PM: 'pm';
OP_PMF: 'pmf';
OP_PM_FROM_FILE: 'pmFromFile';
OP_RBL: 'rbl';
OP_RSUB: 'rsub';
OP_RX: 'rx';
OP_RX_GLOBAL: 'rxGlobal';
OP_STREQ: 'streq';
OP_STRMATCH: 'strmatch';
OP_UNCONDITIONAL_MATCH: 'unconditionalMatch';
OP_VALIDATE_BYTE_RANGE: 'validateByteRange';
OP_VALIDATE_DTD: 'validateDTD';
OP_VALIDATE_SCHEMA: 'validateSchema';
OP_VALIDATE_URL_ENCODING: 'validateUrlEncoding';
OP_VALIDATE_UTF8_ENCODING: 'validateUtf8Encoding';
OP_VERIFY_CC: 'verifyCC';
OP_VERIFY_CPF: 'verifyCPF';
OP_VERIFY_SSN: 'verifySSN';
OP_WITHIN: 'within';
ModeSecRuleOperatorName_WS:
	WS -> skip, popMode, pushMode(ModeSecRuleOperatorValue);
OPERATOR_VALUE: (('\\"') | ~([" ])) (('\\"') | ~('"'))* -> type(STRING);

mode ModeSecRuleOperatorValue;
ModeSecRuleOperatorValue_QUOTE:
	QUOTE -> type(QUOTE), popMode, pushMode( ModeSecRuleAction);
OPERATOR_VALUE2: (('\\"') | ~([" ])) (('\\"') | ~('"'))* -> type(STRING);

mode ModeSecRuleAction;
ModeSecRuleAction_WS: WS -> skip;
ModeSecRuleAction_QUOTE:
	QUOTE -> type(QUOTE), popMode, pushMode(ModeSecRuleActionName);

mode ModeSecRuleActionName;
ModeSecRuleActionName_WS: WS -> skip;
ModeSecRuleActionName_QUOTE: QUOTE -> type(QUOTE), popMode;
ModeSecRuleActionName_COLON: COLON -> type(COLON);
ModeSecRuleActionName_COMMA: COMMA -> type(COMMA);
ModeSecRuleActionName_SINGLE_QUOTE:
	SINGLE_QUOTE -> type(SINGLE_QUOTE), pushMode(ModeSecRuleActionString);
ModeSecRuleActionName_INT: INT -> type(INT);
LEVEL: [1-9];
Accuracy: 'accuracy';
Allow: 'allow';
Auditlog: 'auditlog';
Block: 'block';
Capture: 'capture';
Chain: 'chain';
Ctl: 'ctl';
Deny: 'deny';
Drop: 'drop';
Exec: 'exec';
Expirevar: 'expirevar';
Id: 'id';
Initcol: 'initcol';
Log: 'log';
Logdata: 'logdata';
Maturity: 'maturity';
Msg: 'msg';
MultiMatch: 'multiMatch';
Noauditlog: 'noauditlog';
Nolog: 'nolog';
Pass: 'pass';
Phase: 'phase';
Redirect: 'redirect' -> pushMode(ModeSecRuleActionRedirect);
Rev: 'rev';
Severity: 'severity' -> pushMode(ModeSecRuleActionSeverity);
Setuid: 'setuid' -> pushMode(ModeSecRuleActionSetUid);
Setrsc: 'setrsc' -> pushMode(ModeSecRuleActionSetUid);
Setsid: 'setsid' -> pushMode(ModeSecRuleActionSetUid);
Setenv:
	'setenv' -> pushMode(ModeSecRuleActionSetVar), pushMode(ModeSecRuleActionSetVarName);
Setvar: 'setvar' -> pushMode(ModeSecRuleActionSetVar);
Skip: 'skip';
SkipAfter: 'skipAfter';
Status: 'status';
T: 't';
Tag: 'tag';
Ver: 'ver';
Xmlns: 'xmlns' -> pushMode(ModeSecRuleActionRedirect);

mode ModeSecRuleActionSetVar;
ModeSecRuleActionSetVar_WS: WS -> skip;
ModeSecRuleActionSetVar_QUOTE: QUOTE -> type(QUOTE), popMode;
ModeSecRuleActionSetVar_COMMA: COMMA -> type(COMMA), popMode;
ModeSecRuleActionSetVar_COLON: COLON -> type(COLON);
ModeSecRuleActionSetVar_SINGLE_QUOTE:
	SINGLE_QUOTE -> type(SINGLE_QUOTE), pushMode(ModeSecRuleActionString);
TX: ('t' | 'T') ('x' | 'X');
ModeSecRuleActionSetVar_DOT:
	DOT -> type(DOT), pushMode(ModeSecRuleActionSetVarName);
ModeSecRuleActionSetVar_NOT: NOT -> type(NOT);
ASSIGN: '=' -> pushMode(ModeSecRuleActionSetVarValue);

mode ModeSecRuleActionSetVarName;
ModeSecRuleActionSetVarName_COLON: COLON -> type(COLON);
VAR_NAME: [0-9a-zA-Z_]+ -> popMode;

mode ModeSecRuleActionSetVarValue;
PLUS: '+';
MINUS: '-';
ModeSecRuleActionSetVarValue_PER_CENT:
	PER_CENT -> type(PER_CENT), popMode, pushMode(ModeSecRuleActionMacroExpansion);
VAR_VALUE: ~[ +\-:",%{}=\n]+ -> popMode;

mode ModeSecRuleActionMacroExpansion;
ModeSecRuleActionSetVar_LEFT_BRACKET:
	LEFT_BRACKET -> type(LEFT_BRACKET);
ModeSecRuleActionSetVar_RIGHT_BRACKET:
	RIGHT_BRACKET -> type(RIGHT_BRACKET), popMode;
TX2: ('t' | 'T') ('x' | 'X');
ModeSecRuleActionMacroExpansion_DOT:
	DOT -> type(DOT), pushMode(ModeSecRuleActionMacroExpansionString);
REMOTE_ADDR: ('REMOTE_ADDR' | 'remote_addr');
USERID: ('USERID' | 'userid');
HIGHEST_SEVERITY: ('HIGHEST_SEVERITY' | 'highest_severity');
MATCHED_VAR: ('MATCHED_VAR' | 'matched_var');
MATCHED_VAR_NAME: ('MATCHED_VAR_NAME' | 'matched_var_name');
MULTIPART_STRICT_ERROR: (
		'MULTIPART_STRICT_ERROR'
		| 'multipart_strict_error'
	);
RULE: ('r' | 'R') ('u' | 'U') ('l' | 'L') ('e' | 'E');
SESSION: ('SESSION' | 'session');

mode ModeSecRuleActionMacroExpansionString;
ModeSecRuleActionMacroExpansionString_STRING:
	[0-9a-zA-Z_]+ -> type(STRING), popMode;

mode ModeSecRuleActionString;
ModeSecRuleActionSetVarString_SINGLE_QUOTE:
	SINGLE_QUOTE -> type(SINGLE_QUOTE), popMode;
ModeSecRuleActionSetVarString_STRING: (('\\\'') | ~([' ])) (
		('\\\'')
		| ~('\'')
	)* -> type(STRING);

mode ModeSecRuleActionSetUid;
ModeSecRuleActionSetUid_PER_CENT:
	PER_CENT -> type(PER_CENT), pushMode(ModeSecRuleActionMacroExpansion);
ModeSecRuleActionSetUid_SINGLE_QUOTE:
	SINGLE_QUOTE -> type(SINGLE_QUOTE), pushMode(ModeSecRuleActionString);
ModeSecRuleActionSetUid_QUOTE: QUOTE -> type(QUOTE), popMode;
ModeSecRuleActionSetUid_COMMA: COMMA -> type(COMMA), popMode;
ModeSecRuleActionSetUid_COLON: COLON -> type(COLON);

mode ModeSecRuleActionRedirect;
ModeSecRuleActionRedirect_COLON:
	COLON -> type(COLON), popMode, pushMode(ModeSecRuleActionRedirectValue);

mode ModeSecRuleActionRedirectValue;
ModeSecRuleActionRedirect_STRING:
	~[,]+ -> type(STRING), popMode;

mode ModeSecRuleActionSeverity;
ModeSecRuleActionSeverity_COLON: COLON -> type(COLON);
ModeSecRuleActionSeverity_SINGLE_QUOTE:
	SINGLE_QUOTE -> type(SINGLE_QUOTE), popMode, pushMode(ModeSecRuleActionSeverityValue);

mode ModeSecRuleActionSeverityValue;
ModeSecRuleActionSeverityValue_SINGLE_QUOTE:
	SINGLE_QUOTE -> type(SINGLE_QUOTE), popMode;
EMERGENCY: 'EMERGENCY';
ALERT: 'ALERT';
CRITICAL: 'CRITICAL';
ERROR: 'ERROR';
WARNING: 'WARNING';
NOTICE: 'NOTICE';
INFO: 'INFO';
DEBUG: 'DEBUG';