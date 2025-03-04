lexer grammar SecLangLexer;

tokens{
	QUOTE,
	SINGLE_QUOTE,
	COMMA,
	NOT,
	DOT,
	COLON,
	SEMICOLON,
	ASSIGN,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	PER_CENT,
	PIPE,
	PLUS,
	MINUS,
	INT,
	INT_RANGE,
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
SEMICOLON: ';';
ASSIGN: '=';
LEFT_BRACKET: '{';
RIGHT_BRACKET: '}';
PER_CENT: '%';
PIPE: '|';
PLUS: '+';
MINUS: '-';

INT_RANGE: INT '-' INT;
INT: [0-9]+;
OPTION: ('On' | 'Off');
BODY_LIMIT_ACTION: ('Reject' | 'ProcessPartial');
COMMENT: '#' ~[\r\n]* '\r'? '\n'? -> skip;
NL: '\\' '\r'? '\n' -> skip;
WS: (([ \t\r\n]+) | NL) -> skip;

Include: 'Include' -> pushMode(ModeInclude);
SecAction: 'SecAction' -> pushMode(ModeSecRuleAction);
SecArgumentSeparator:
	'SecArgumentSeparator' -> pushMode(ModeAuditLogString);
SecArgumentsLimit: 'SecArgumentsLimit';
SecAuditEngine: 'SecAuditEngine' -> pushMode(ModeAuditLog);
SecAuditLog: 'SecAuditLog' -> pushMode(ModeAuditLogString);
SecAuditLog2: 'SecAuditLog2' -> pushMode(ModeAuditLogString);
SecAuditLogDirMode:
	'SecAuditLogDirMode' -> pushMode(ModeAuditLog);
SecAuditLogFormat:
	'SecAuditLogFormat' -> pushMode(ModeAuditLog);
SecAuditLogFileMode:
	'SecAuditLogFileMode' -> pushMode(ModeAuditLog);
SecAuditLogParts: 'SecAuditLogParts' -> pushMode(ModeAuditLog);
SecAuditLogRelevantStatus:
	'SecAuditLogRelevantStatus' -> pushMode(ModeAuditLogString);
SecAuditLogStorageDir:
	'SecAuditLogStorageDir' -> pushMode(ModeAuditLogString);
SecAuditLogType: 'SecAuditLogType' -> pushMode(ModeAuditLog);
SecCollectionTimeout: 'SecCollectionTimeout';
SecComponentSignature:
	'SecComponentSignature' -> pushMode(ModeAuditLogString);
SecCookieFormat: 'SecCookieFormat';
SecDataDir: 'SecDataDir' -> pushMode(ModeAuditLogString);
SecDebugLog: 'SecDebugLog';
SecDebugLogLevel: 'SecDebugLogLevel';
SecDefaultAction:
	'SecDefaultAction' -> pushMode(ModeSecRuleAction);
SecGeoLookupDb: 'SecGeoLookupDb';
SecHttpBlKey: 'SecHttpBlKey';
SecMarker: 'SecMarker' -> pushMode(ModeAuditLogString);
SecPcreMatchLimit: 'SecPcreMatchLimit';
SecPcreMatchLimitRecursion: 'SecPcreMatchLimitRecursion';
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
SecResponseBodyMimeType:
	'SecResponseBodyMimeType' -> pushMode(ModeResponseBodyMimeType);
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
SecRule: 'SecRule' -> pushMode(ModeSecRule);
SecStatusEngine: 'SecStatusEngine';
SecTmpDir: 'SecTmpDir' -> pushMode(ModeAuditLogString);
SecTmpSaveUploadedFiles: 'SecTmpSaveUploadedFiles';
SecUnicodeMapFile:
	'SecUnicodeMapFile' -> pushMode(ModeAuditLogString);
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

mode ModeAuditLogString;
ModeAuditLogString_WS: WS -> skip;
ModeAuditLogString_QUOTE: QUOTE -> type(QUOTE);
ModeAuditLogString_STRING: (('\\"') | ~([" ])) (
		('\\"')
		| ('\\' '\r'? '\n')
		| ~["\r\n ]
	)* -> type(STRING), popMode;

mode ModeResponseBodyMimeType;
ModeResponseBodyMimeType_WS: WS -> skip;
MIME_TYPE: [a-zA-Z]+ '/' [a-zA-Z]+ ' '*;
MIME_TYPES: MIME_TYPE+ -> popMode;

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
	INT -> type(INT), popMode, pushMode(ModeSecRuleVariable);

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

mode ModeSecRule;
ModeSecRule_WS: WS -> skip, pushMode(ModeSecRuleVariableName);
ModeSecRule_QUOTE:
	QUOTE -> type(QUOTE), popMode, pushMode(ModeSecRuleOperator);

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
ModeSecRuleVariableName_WS: WS -> skip, popMode;
ModeSecRuleVariableName_COMMA: COMMA -> skip, popMode;
ModeSecRuleVariableName_PIPE: PIPE -> type(PIPE);
ModeSecRuleVariableName_COLON:
	COLON -> type(COLON), pushMode(ModeSecRuleVariableSubName);
ModeSecRuleVariableName_VAR_COUNT: '&' -> type(VAR_COUNT);
ModeSecRuleVariableName_VAR_NOT: NOT -> type(NOT);

mode ModeSecRuleVariableSubName;
ModeSecRuleVariableSubName_VAR_SUB_NAME:
	~[ :!&|",\n]+ -> type(STRING), popMode;

mode ModeSecRuleOperator;
ModeSecRuleOperator_NOT: NOT -> type(NOT);
AT: '@' -> popMode, pushMode(ModeSecRuleOperatorName);
ModeSecRuleOperator_QUOTE:
	QUOTE -> type(QUOTE), popMode, pushMode(ModeSecRuleAction);
RX_DEFUALT: (('\\"') | ~([" @!])) (('\\"') | ~('"'))* -> type(STRING);

mode ModeSecRuleOperatorName;
ModeSecRuleOperator_WS:
	WS -> skip, popMode, pushMode(ModeSecRuleOperatorValue);
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

mode ModeSecRuleOperatorValue;
ModeSecRuleOperatorValue_QUOTE:
	QUOTE -> type(QUOTE), popMode, pushMode(ModeSecRuleAction);
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
Ctl: 'ctl' -> pushMode(ModeSecRuleActionCtl);
Deny: 'deny';
Drop: 'drop';
Exec: 'exec';
Expirevar: 'expirevar';
Id: 'id';
Initcol: 'initcol' -> pushMode(ModeSecRuleActionInitCol);
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
Setenv: 'setenv' -> pushMode(ModeSecRuleActionSetVar);
Setvar: 'setvar' -> pushMode(ModeSecRuleActionSetVar);
Skip: 'skip';
SkipAfter: 'skipAfter' -> pushMode(ModeSecRuleActionRedirect);
Status: 'status';
T: 't' -> pushMode(ModeSecRuleActionT);
Tag: 'tag';
Ver: 'ver';
Xmlns: 'xmlns' -> pushMode(ModeSecRuleActionRedirect);

mode ModeSecRuleActionSetVar;
ModeSecRuleActionSetVar_WS: WS -> skip;
ModeSecRuleActionSetVar_COLON: COLON -> type(COLON);
ModeSecRuleActionSetVar_SINGLE_QUOTE:
	SINGLE_QUOTE -> type(SINGLE_QUOTE);
TX: ('t' | 'T') ('x' | 'X');
ModeSecRuleActionSetVar_DOT: DOT -> type(DOT);
ModeSecRuleActionSetVar_NOT: NOT -> type(NOT);
ModeSecRuleActionSetVar_COMMA: COMMA -> type(COMMA), popMode;
ModeSecRuleActionSetVar_QUOTE:
	QUOTE -> type(QUOTE), popMode, popMode;
ModeSecRuleActionSetVarName_ASSIGN:
	ASSIGN -> type(ASSIGN), popMode, pushMode(ModeSecRuleActionSetVarValue);
ModeSecRuleActionSetVarName_PER_CENT:
	PER_CENT -> type(PER_CENT), pushMode(ModeSecRuleActionMacroExpansion);
VAR_NAME: [0-9a-zA-Z_][0-9a-zA-Z_]*;

mode ModeSecRuleActionSetVarValue;
ModeSecRuleActionSetVarValue_SINGLE_QUOTE:
	SINGLE_QUOTE -> type(SINGLE_QUOTE);
ModeSecRuleActionSetVarValue_PLUS: PLUS -> type(PLUS);
ModeSecRuleActionSetVarValue_MINUS: MINUS -> type(MINUS);
ModeSecRuleActionSetVarValue_COMMA:
	COMMA -> type(COMMA), popMode;
ModeSecRuleActionSetVarValue_QUOTE:
	QUOTE -> type(QUOTE), popMode, popMode;
ModeSecRuleActionSetVarValue_PER_CENT:
	PER_CENT -> type(PER_CENT), pushMode(ModeSecRuleActionMacroExpansion);
VAR_VALUE: ~[+\-%'",]~[{}%'",]*;

mode ModeSecRuleActionMacroExpansion;
ModeSecRuleActionSetVar_LEFT_BRACKET:
	LEFT_BRACKET -> type(LEFT_BRACKET);
ModeSecRuleActionSetVar_RIGHT_BRACKET:
	RIGHT_BRACKET -> type(RIGHT_BRACKET), popMode;
TX2: [tT][xX];
ModeSecRuleActionMacroExpansion_DOT:
	DOT -> type(DOT), pushMode(ModeSecRuleActionMacroExpansionString);
REMOTE_ADDR:
	[rR][eE][mM][oO][tT][eE]'_' [aA][dD][dD][rR];
USERID: [uU][sS][eE][rR][iI][dD];
HIGHEST_SEVERITY:
	[hH][iI][gG][hH][eE][sS][tT]'_' [sS][eE][vV][eE][rR][iI][tT][yY];
MATCHED_VAR:
	[mM][aA][tT][cC][hH][eE][dD]'_' [vV][aA][rR];
MATCHED_VAR_NAME:
	[mM][aA][tT][cC][hH][eE][dD]'_' [vV][aA][rR]'_' [nN][aA][mM][eE];
MULTIPART_STRICT_ERROR:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [sS][tT][rR][iI][cC][tT]'_' [eE][rR][rR][oO][rR];
REQBODY_PROCESSOR_ERROR:
	[rR][eE][qQ][bB][oO][dD][yY]'_' [pP][rR][oO][cC][eE][sS][sS][oO][rR]'_' [eE][rR][rR][oO][rR];
MULTIPART_BOUNDARY_QUOTED:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [bB][oO][uU][nN][dD][aA][rR][yY]'_' [qQ][uU][oO][tT][eE]
		[dD];
MULTIPART_BOUNDARY_WHITESPACE:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [bB][oO][uU][nN][dD][aA][rR][yY]'_' [wW][hH][iI][tT][eE]
		[sS][pP][aA][cC][eE];
MULTIPART_DATA_AFTER:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [dD][aA][tT][aA]'_' [aA][fF][tT][eE][rR];
MULTIPART_DATA_BEFORE:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [dD][aA][tT][aA]'_' [bB][eE][fF][oO][rR][eE];
MULTIPART_HEADER_FOLDING:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [hH][eE][aA][dD][eE][rR]'_' [fF][oO][lL][dD][iI][nN][gG]
		;
MULTIPART_LF_LINE:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [lL][fF]'_' [lL][iI][nN][eE];
MULTIPART_MISSING_SEMICOLON:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [mM][iI][sS][sS][iI][nN][gG]'_' [sS][eE][mM][iI][cC][oO]
		[lL][oO][nN];
MULTIPART_INVALID_QUOTING:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [iI][nN][vV][aA][lL][iI][dD]'_' [qQ][uU][oO][tT][iI][nN]
		[gG];
MULTIPART_INVALID_PART:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [iI][nN][vV][aA][lL][iI][dD]'_' [pP] [aA][rR][tT];
MULTIPART_INVALID_HEADER_FOLDING:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [iI][nN][vV][aA][lL][iI][dD]'_' [hH][eE][aA][dD][eE][rR]
		'_' [fF][oO][lL][dD][iI][nN][gG];
MULTIPART_FILE_LIMIT_EXCEEDED:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [fF][iI][lL][eE]'_' [lL][iI][mM][iI][tT]'_' [eE][xX][cC]
		[eE][eE][dD][eE][dD];
RULE: [rR][uU][lL][eE];
SESSION: [sS][eE][sS][sS][iI][oO][nN];
REQBODY_ERROR_MSG:
	[rR][eE][qQ][bB][oO][dD][yY]'_' [eE][rR][rR][oO][rR]'_' [mM][sS][gG];

mode ModeSecRuleActionMacroExpansionString;
ModeSecRuleActionMacroExpansionString_STRING:
	[0-9a-zA-Z_]+ -> type(STRING), popMode;

mode ModeSecRuleActionString;
ModeSecRuleActionSetVarString_SINGLE_QUOTE:
	SINGLE_QUOTE -> type(SINGLE_QUOTE), popMode;
ModeSecRuleActionSetVarString_STRING: (
		'\\\''
		| ~['%]
		| ('%' ~[{])
	) ('\\\'' | ~['%] | ('%' ~[{]))* -> type(STRING);
ModeSecRuleActionString_PER_CENT:
	PER_CENT -> type(PER_CENT), pushMode(ModeSecRuleActionMacroExpansion);

mode ModeSecRuleActionSetUid;
ModeSecRuleActionSetUid_PER_CENT:
	PER_CENT -> type(PER_CENT), pushMode(ModeSecRuleActionMacroExpansion);
ModeSecRuleActionSetUid_SINGLE_QUOTE:
	SINGLE_QUOTE -> type(SINGLE_QUOTE), pushMode(ModeSecRuleActionString);
ModeSecRuleActionSetUid_QUOTE: QUOTE -> type(QUOTE), popMode;
ModeSecRuleActionSetUid_COMMA: COMMA -> type(COMMA), popMode;
ModeSecRuleActionSetUid_COLON: COLON -> type(COLON);

mode ModeSecRuleActionT;
ModeSecRuleActionT_COLON: COLON -> type(COLON);
BASE64_DECODE: 'base64Decode' -> popMode;
SQL_HEX_DECODE: 'sqlHexDecode' -> popMode;
BASE64_DECODE_EXT: 'base64DecodeExt' -> popMode;
BASE64_ENCODE: 'base64Encode' -> popMode;
CMDLINE: 'cmdLine' -> popMode;
COMPRESS_WHITESPACE: 'compressWhitespace' -> popMode;
CSS_DECODE: 'cssDecode' -> popMode;
ESCAPE_SEQ_DECODE: 'escapeSeqDecode' -> popMode;
HEX_DECODE: 'hexDecode' -> popMode;
HEX_ENCODE: 'hexEncode' -> popMode;
HTML_ENTITY_DECODE: 'htmlEntityDecode' -> popMode;
JS_DECODE: 'jsDecode' -> popMode;
LENGTH: 'length' -> popMode;
LOWERCASE: 'lowercase' -> popMode;
MD5: 'md5' -> popMode;
NONE: 'none' -> popMode;
NORMALISE_PATH: 'normalisePath' -> popMode;
NORMALIZE_PATH: 'normalizePath' -> popMode;
NORMALISE_PATHWIN: 'normalisePathWin' -> popMode;
NORMALIZE_PATHWIN: 'normalizePathWin' -> popMode;
PARITY_EVEN_7BIT: 'parityEven7bit' -> popMode;
PARITY_ODD_7BIT: 'parityOdd7bit' -> popMode;
PARITY_ZERO_7BIT: 'parityZero7bit' -> popMode;
REMOVE_NULLS: 'removeNulls' -> popMode;
REMOVE_WHITESPACE: 'removeWhitespace' -> popMode;
REPLACE_COMMENTS: 'replaceComments' -> popMode;
REMOVE_COMMENTSCHAR: 'removeCommentsChar' -> popMode;
REMOVE_COMMENTS: 'removeComments' -> popMode;
REPLACE_NULLS: 'replaceNulls' -> popMode;
URL_DECODE: 'urlDecode' -> popMode;
UPPERCASE: 'uppercase' -> popMode;
URL_DECODE_UNI: 'urlDecodeUni' -> popMode;
URL_ENCODE: 'urlEncode' -> popMode;
UTF8_TO_UNICODE: 'utf8toUnicode' -> popMode;
SHA1: 'sha1' -> popMode;
TRIM_LEFT: 'trimLeft' -> popMode;
TRIM_RIGHT: 'trimRight' -> popMode;
TRIM: 'trim' -> popMode;

mode ModeSecRuleActionCtl;
ModeSecRuleActionCtl_COLON: COLON -> type(COLON);
CTL_AUDIT_ENGINE:
	'auditEngine' -> popMode, pushMode(ModeSecRuleActionCtlAuditEngine);
CTL_AUDIT_LOG_PARTS:
	'auditLogParts' -> popMode, pushMode(ModeSecRuleActionCtlAuditLogParts);
CTL_FORCE_REQUEST_BODY_VARIABLE:
	'forceRequestBodyVariable' -> popMode, pushMode(ModeSecRuleActionCtlForceRequestBodyVariable);
CTL_REQUEST_BODY_ACCESS:
	'requestBodyAccess' -> popMode, pushMode(ModeSecRuleActionCtlForceRequestBodyVariable);
CTL_REQUEST_BODY_PROCESSOR:
	'requestBodyProcessor' -> popMode, pushMode(ModeSecRuleActionCtlRequestBodyProcessor);
CTL_RULE_ENGINE:
	'ruleEngine' -> popMode, pushMode(ModeSecRuleActionCtlRuleEngine);
CTL_RULE_REMOVE_BY_ID:
	'ruleRemoveById' -> popMode, pushMode(ModeSecRuleActionCtlRuleRemoveById);
CTL_RULE_REMOVE_BY_TAG:
	'ruleRemoveByTag' -> popMode, pushMode(ModeSecRuleActionCtlRuleRemoveByTag);
CTL_RULE_REMOVE_TARGET_BY_ID:
	'ruleRemoveTargetById' -> popMode, pushMode(ModeSecRuleActionCtlRuleRemoveTargetById);
CTL_RULE_REMOVE_TARGET_BY_TAG:
	'ruleRemoveTargetByTag' -> popMode, pushMode(ModeSecRuleActionCtlRuleRemoveTargetByTag);

mode ModeSecRuleActionInitCol;
ModeSecRuleActionInitCol_COLON: COLON -> type(COLON);
ModeSecRuleActionInitCol_ASSIGN:
	ASSIGN -> type(ASSIGN), popMode, pushMode(ModeSecRuleActionInitColValue);
ModeSecRuleActionInitCol_STRING: ~[=:]+ -> type(STRING);

mode ModeSecRuleActionInitColValue;
ModeSecRuleActionInitColValue_STRING:
	~[,"]+ -> type(STRING), popMode;

mode ModeSecRuleActionCtlAuditEngine;
ModeSecRuleActionCtlAuditEngine_ASSIGN:
	ASSIGN -> type(ASSIGN), popMode, pushMode(ModeAuditLog);

mode ModeSecRuleActionCtlAuditLogParts;
ModeSecRuleActionCtlAuditLogParts_ASSIGN:
	ASSIGN -> type(ASSIGN);
ModeSecRuleActionCtlAuditLogParts_PLUS:
	PLUS -> type(PLUS), popMode, pushMode(ModeAuditLog);
ModeSecRuleActionCtlAuditLogParts_MINUS:
	MINUS -> type(MINUS), popMode, pushMode(ModeAuditLog);

mode ModeSecRuleActionCtlForceRequestBodyVariable;
ModeSecRuleActionCtlForceRequestBodyVariable_ASSIGN:
	ASSIGN -> type(ASSIGN);
ModeSecRuleActionCtlForceRequestBodyVariable_OPTION:
	OPTION -> type(OPTION), popMode;

mode ModeSecRuleActionCtlRequestBodyProcessor;
ModeSecRuleActionCtlValueRequestBodyProcessor_ASSIGN:
	ASSIGN -> type(ASSIGN);
URLENCODED: 'URLENCODED' -> popMode;
MULTIPART: 'MULTIPART' -> popMode;
XML: 'XML' -> popMode;
JSON: 'JSON' -> popMode;

mode ModeSecRuleActionCtlRuleEngine;
ModeSecRuleActionCtlRuleEngine_ASSIGN: ASSIGN -> type(ASSIGN);
ModeSecRuleActionCtlRuleEngine_OPTION: (
		'On'
		| 'Off'
		| 'DetectionOnly'
	) -> type(OPTION), popMode;

mode ModeSecRuleActionCtlRuleRemoveById;
ModeSecRuleActionCtlRuleRemoveById_ASSIGN:
	ASSIGN -> type(ASSIGN);
ModeSecRuleActionCtlRuleRemoveById_INT:
	INT -> type(INT), popMode;
ModeSecRuleActionCtlRuleRemoveById_INT_RANGE:
	INT_RANGE -> type(INT_RANGE), popMode;

mode ModeSecRuleActionCtlRuleRemoveByTag;
ModeSecRuleActionCtlRuleRemoveByTag_ASSIGN:
	ASSIGN -> type(ASSIGN);
ModeSecRuleActionCtlRuleRemoveByTag_STRING:
	~[,=]+ -> type(STRING), popMode;

mode ModeSecRuleActionCtlRuleRemoveTargetById;
ModeSecRuleActionCtlRuleRemoveTargetById_ASSIGN:
	ASSIGN -> type(ASSIGN);
ModeSecRuleActionCtlRuleRemoveTargetById_INT: INT -> type(INT);
ModeSecRuleActionCtlRuleRemoveTargetById_SEMICOLON:
	SEMICOLON -> type(SEMICOLON), popMode, pushMode(ModeSecRuleVariableName);

mode ModeSecRuleActionCtlRuleRemoveTargetByTag;
ModeSecRuleActionCtlRuleRemoveTargetByTag_ASSIGN:
	ASSIGN -> type(ASSIGN);
ModeSecRuleActionCtlRuleRemoveTargetByTag_SEMICOLON:
	SEMICOLON -> type(SEMICOLON), popMode, pushMode(ModeSecRuleVariableName);
ModeSecRuleActionCtlRuleRemoveTargetByTag_STRING:
	~[,=;]+ -> type(STRING);

mode ModeSecRuleActionRedirect;
ModeSecRuleActionRedirect_COLON:
	COLON -> type(COLON), popMode, pushMode(ModeSecRuleActionRedirectValue);

mode ModeSecRuleActionRedirectValue;
ModeSecRuleActionRedirect_STRING:
	~[,"]+ -> type(STRING), popMode;

mode ModeSecRuleActionSeverity;
ModeSecRuleActionSeverity_COLON: COLON -> type(COLON);
ModeSecRuleActionSeverity_SINGLE_QUOTE:
	SINGLE_QUOTE -> type(SINGLE_QUOTE), popMode, pushMode(ModeSecRuleActionSeverityValue);
SEVERITY_LEVEL: [0-7] -> popMode;

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