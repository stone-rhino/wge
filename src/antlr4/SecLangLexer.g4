lexer grammar SecLangLexer;

tokens{
	QUOTE,
	COMMA,
	NOT,
	STRING
}

WS: [ \t\r\n]+ -> skip;
COMMENT: ('#' .*? '\r'? ('\n' | EOF))+ -> skip;
QUOTE: '"';
NOT: '!';
INT_RANGE: INT '-' INT;
INT: [0-9]+;

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
SecRequestBodyAccess:
	'SecRequestBodyAccess' -> pushMode(ModeEngineConfig);
SecRequestBodyInMemoryLimit: 'SecRequestBodyInMemoryLimit';
SecRequestBodyJsonDepthLimit: 'SecRequestBodyJsonDepthLimit';
SecRequestBodyLimit: 'SecRequestBodyLimit';
SecRequestBodyNoFilesLimit: 'SecRequestBodyNoFilesLimit';
SecRequestBodyLimitAction: 'SecRequestBodyLimitAction';
SecResponseBodyLimit: 'SecResponseBodyLimit';
SecResponseBodyLimitAction: 'SecResponseBodyLimitAction';
SecResponseBodyMimeType: 'SecResponseBodyMimeType';
SecResponseBodyMimeTypesClear: 'SecResponseBodyMimeTypesClear';
SecResponseBodyAccess:
	'SecResponseBodyAccess' -> pushMode(ModeEngineConfig);
SecRuleEngine: 'SecRuleEngine' -> pushMode(ModeEngineConfig);
SecRuleRemoveById: 'SecRuleRemoveById';
SecRuleRemoveByMsg:
	'SecRuleRemoveByMsg' -> pushMode(ModeRuleRemoveByMsg);
SecRuleRemoveByTag:
	'SecRuleRemoveByTag' -> pushMode(ModeRuleRemoveByTag);
SecRuleScript: 'SecRuleScript';
SecRuleUpdateActionById: 'SecRuleUpdateActionById';
SecRuleUpdateTargetById: 'SecRuleUpdateTargetById';
SecRuleUpdateTargetByMsg: 'SecRuleUpdateTargetByMsg';
SecRuleUpdateTargetByTag: 'SecRuleUpdateTargetByTag';
SecRule: 'SecRule' -> pushMode(ModeSecRuleVariable);
SecTmpDir: 'SecTmpDir';
SecTmpSaveUploadedFiles:
	'SecTmpSaveUploadedFiles' -> pushMode(ModeEngineConfig);
SecUnicodeMapFile: 'SecUnicodeMapFile';
SecUploadDir: 'SecUploadDir';
SecUploadFileLimit: 'SecUploadFileLimit';
SecUploadFileMode: 'SecUploadFileMode';
SecUploadKeepFiles:
	'SecUploadKeepFiles' -> pushMode(ModeEngineConfig);
SecWebAppId: 'SecWebAppId';
SecXmlExternalEntity:
	'SecXmlExternalEntity' -> pushMode(ModeEngineConfig);

mode ModeInclude;
ModeInclude_WS: ' ' -> skip;
ModeInclude_QUOTE: '"' -> type(QUOTE);
IncludeFilePath:
	[a-zA-Z0-9/._~|\\:-]+ -> type(STRING), popMode;

mode ModeEngineConfig;
ModeEngineConfig_WS: ' ' -> skip;
OPTION: ('On' | 'Off') -> popMode;
DERECTION_ONLY: 'DetectionOnly' -> popMode;

mode ModeRuleRemoveByMsg;
ModeRuleRemoveByMsg_WS: ' ' -> skip;
ModeRuleRemoveByMsg_QUOTE: '"' -> type(QUOTE);
ModeRuleRemoveByMsg_STRING:
	('\\"' | ~["])+ -> type(STRING), popMode;

mode ModeRuleRemoveByTag;
ModeRuleRemoveByTag_WS: ' ' -> skip;
ModeRuleRemoveByTag_QUOTE: '"' -> type(QUOTE);
ModeRuleRemoveByTag_STRING: ('\\"' | ~["])+ -> type(STRING), popMode;

mode ModeSecRuleVariable;
ModeSecRuleVariable_WS:
	[ \t]+ -> skip, popMode, pushMode(ModeSecRuleVariableName);

mode ModeSecRuleVariableName;
ModeSecRuleVariableName_WS:
	[ \t] -> skip, popMode, pushMode(ModeSecRuleOperator);
PIPE: '|';
COLON: ':';
VAR_COUNT: '&';
VAR_NOT: '!' -> type(NOT);
ARGS: 'ARGS';
ARGS_COMBINED_SIZE: 'ARGS_COMBINED_SIZE';
ARGS_GET: 'ARGS_GET';
ARGS_GET_NAMES: 'ARGS_GET_NAMES';
ARGS_NAMES: 'ARGS_NAMES';
ARGS_POST: 'ARGS_POST';
ARGS_POST_NAMES: 'ARGS_POST_NAMES';
AUTH_TYPE: 'AUTH_TYPE';
DURATION: 'DURATION';
ENV: 'ENV';
FILES: 'FILES';
FILES_COMBINED_SIZE: 'FILES_COMBINED_SIZE';
FILES_NAMES: 'FILES_NAMES';
FULL_REQUEST: 'FULL_REQUEST';
FULL_REQUEST_LENGTH: 'FULL_REQUEST_LENGTH';
FILES_SIZES: 'FILES_SIZES';
FILES_TMPNAMES: 'FILES_TMPNAMES';
FILES_TMP_CONTENT: 'FILES_TMP_CONTENT';
GEO: 'GEO';
HIGHEST_SEVERITY: 'HIGHEST_SEVERITY';
INBOUND_DATA_ERROR: 'INBOUND_DATA_ERROR';
MATCHED_VAR: 'MATCHED_VAR';
MATCHED_VARS: 'MATCHED_VARS';
MATCHED_VAR_NAME: 'MATCHED_VAR_NAME';
MATCHED_VARS_NAMES: 'MATCHED_VARS_NAMES';
MODSEC_BUILD: 'MODSEC_BUILD';
MSC_PCRE_LIMITS_EXCEEDED: 'MSC_PCRE_LIMITS_EXCEEDED';
MULTIPART_CRLF_LF_LINES: 'MULTIPART_CRLF_LF_LINES';
MULTIPART_FILENAME: 'MULTIPART_FILENAME';
MULTIPART_NAME: 'MULTIPART_NAME';
MULTIPART_PART_HEADERS: 'MULTIPART_PART_HEADERS';
MULTIPART_STRICT_ERROR: 'MULTIPART_STRICT_ERROR';
MULTIPART_UNMATCHED_BOUNDARY: 'MULTIPART_UNMATCHED_BOUNDARY';
OUTBOUND_DATA_ERROR: 'OUTBOUND_DATA_ERROR';
PATH_INFO: 'PATH_INFO';
QUERY_STRING: 'QUERY_STRING';
REMOTE_ADDR: 'REMOTE_ADDR';
REMOTE_HOST: 'REMOTE_HOST';
REMOTE_USER: 'REMOTE_USER';
REQBODY_ERROR: 'REQBODY_ERROR';
REQBODY_ERROR_MSG: 'REQBODY_ERROR_MSG';
REQBODY_PROCESSOR: 'REQBODY_PROCESSOR';
REQUEST_BASENAME: 'REQUEST_BASENAME';
REQUEST_BODY: 'REQUEST_BODY';
REQUEST_BODY_LENGTH: 'REQUEST_BODY_LENGTH';
REQUEST_COOKIES: 'REQUEST_COOKIES';
REQUEST_COOKIES_NAMES: 'REQUEST_COOKIES_NAMES';
REQUEST_FILENAME: 'REQUEST_FILENAME';
REQUEST_HEADERS: 'REQUEST_HEADERS';
REQUEST_HEADERS_NAMES: 'REQUEST_HEADERS_NAMES';
REQUEST_LINE: 'REQUEST_LINE';
REQUEST_METHOD: 'REQUEST_METHOD';
REQUEST_PROTOCOL: 'REQUEST_PROTOCOL';
REQUEST_URI: 'REQUEST_URI';
REQUEST_URI_RAW: 'REQUEST_URI_RAW';
RESPONSE_BODY: 'RESPONSE_BODY';
RESPONSE_CONTENT_LENGTH: 'RESPONSE_CONTENT_LENGTH';
RESPONSE_CONTENT_TYPE: 'RESPONSE_CONTENT_TYPE';
RESPONSE_HEADERS: 'RESPONSE_HEADERS';
RESPONSE_HEADERS_NAMES: 'RESPONSE_HEADERS_NAMES';
RESPONSE_PROTOCOL: 'RESPONSE_PROTOCOL';
RESPONSE_STATUS: 'RESPONSE_STATUS';
RULE: 'RULE';
SERVER_ADDR: 'SERVER_ADDR';
SERVER_NAME: 'SERVER_NAME';
SERVER_PORT: 'SERVER_PORT';
SESSION: 'SESSION';
SESSIONID: 'SESSIONID';
STATUS_LINE: 'STATUS_LINE';
TIME: 'TIME';
TIME_DAY: 'TIME_DAY';
TIME_EPOCH: 'TIME_EPOCH';
TIME_HOUR: 'TIME_HOUR';
TIME_MIN: 'TIME_MIN';
TIME_MON: 'TIME_MON';
TIME_SEC: 'TIME_SEC';
TIME_WDAY: 'TIME_WDAY';
TIME_YEAR: 'TIME_YEAR';
TX: 'TX';
UNIQUE_ID: 'UNIQUE_ID';
URLENCODED_ERROR: 'URLENCODED_ERROR';
USERID: 'USERID';
WEBAPPID: 'WEBAPPID';
VAR_SUB_NAME: ~[ :|",\n]+ -> type(STRING);

mode ModeSecRuleOperator;
ModeSecRuleOperator_QUOTE: '"' -> type(QUOTE);
AT: '@';
BeginsWith: 'beginsWith';
Contains: 'contains';
ContainsWord: 'containsWord';
DetectSQLi: 'detectSQLi';
DetectXSS: 'detectXSS';
EndsWith: 'endsWith';
FuzzyHash: 'fuzzyHash';
Eq: 'eq';
Ge: 'ge';
GeoLookup: 'geoLookup';
Gt: 'gt';
InspectFile: 'inspectFile';
IpMatch: 'ipMatch';
IpMatchF: 'ipMatchF';
IpMatchFromFile: 'ipMatchFromFile';
Le: 'le';
Lt: 'lt';
NoMatch: 'noMatch';
Pm: 'pm';
Pmf: 'pmf';
PmFromFile: 'pmFromFile';
Rbl: 'rbl';
Rsub: 'rsub';
Rx: 'rx';
RxGlobal: 'rxGlobal';
Streq: 'streq';
Strmatch: 'strmatch';
UnconditionalMatch: 'unconditionalMatch';
ValidateByteRange: 'validateByteRange';
ValidateDTD: 'validateDTD';
ValidateSchema: 'validateSchema';
ValidateUrlEncoding: 'validateUrlEncoding';
ValidateUtf8Encoding: 'validateUtf8Encoding';
VerifyCC: 'verifyCC';
VerifyCPF: 'verifyCPF';
VerifySSN: 'verifySSN';
Within: 'within';
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
Redirect: 'redirect';
Rev: 'rev';
Severity: 'severity';
Setuid: 'setuid';
Setrsc: 'setrsc';
Setsid: 'setsid';
Setenv: 'setenv';
Setvar: 'setvar';
Skip: 'skip';
SkipAfter: 'skipAfter';
Status: 'status';
T: 't';
Tag: 'tag';
Ver: 'ver';
Xmlns: 'xmlns';
ACTION_VALUE: ~[ :",\n]+ -> type(STRING);