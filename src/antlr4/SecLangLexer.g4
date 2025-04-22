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
VAR_ARGS: [aA][rR][gG][sS];
VAR_ARGS_COMBINED_SIZE:
	[aA][rR][gG][sS]'_' [cC][oO][mM][bB][iI][nN][eE][dD]'_' [sS][iI][zZ][eE];
VAR_ARGS_GET: [aA][rR][gG][sS]'_' [gG][eE][tT];
VAR_ARGS_GET_NAMES:
	[aA][rR][gG][sS]'_' [gG][eE][tT]'_' [nN][aA][mM][eE][sS];
VAR_ARGS_NAMES:
	[aA][rR][gG][sS]'_' [nN][aA][mM][eE][sS];
VAR_ARGS_POST: [aA][rR][gG][sS]'_' [pP][oO][sS][tT];
VAR_ARGS_POST_NAMES:
	[aA][rR][gG][sS]'_' [pP][oO][sS][tT]'_' [nN][aA][mM][eE][sS];
VAR_AUTH_TYPE: [aA][uU][tT][hH]'_' [tT][yY][pP][eE];
VAR_DURATION: [dD][uU][rR][aA][tT][iI][oO][nN];
VAR_ENV: [eE][nN][vV];
VAR_FILES: [fF][iI][lL][eE][sS];
VAR_FILES_COMBINED_SIZE:
	[fF][iI][lL][eE][sS]'_' [cC][oO][mM][bB][iI][nN][eE][dD]'_' [sS][iI][zZ][eE];
VAR_FILES_NAMES:
	[fF][iI][lL][eE][sS]'_' [nN][aA][mM][eE][sS];
VAR_FULL_REQUEST:
	[fF][uU][lL][lL]'_' [rR][eE][qQ][uU][eE][sS][tT];
VAR_FULL_REQUEST_LENGTH:
	[fF][uU][lL][lL]'_' [rR][eE][qQ][uU][eE][sS][tT]'_' [lL][eE][nN][gG][tT][hH];
VAR_FILES_SIZES:
	[fF][iI][lL][eE][sS]'_' [sS][iI][zZ][eE][sS];
VAR_FILES_TMPNAMES:
	[fF][iI][lL][eE][sS]'_' [tT][mM][pP][nN][aA][mM][eE][sS];
VAR_FILES_TMP_CONTENT:
	[fF][iI][lL][eE][sS]'_' [tT][mM][pP]'_' [cC][oO][nN][tT][eE][nN][tT];
VAR_GEO: [gG][eE][oO];
VAR_HIGHEST_SEVERITY:
	[hH][iI][gG][hH][eE][sS][tT]'_' [sS][eE][vV][eE][rR][iI][tT][yY];
VAR_INBOUND_DATA_ERROR:
	[iI][nN][bB][oO][uU][nN][dD]'_' [dD][aA][tT][aA]'_' [eE][rR][rR][oO][rR];
VAR_MATCHED_VAR:
	[mM][aA][tT][cC][hH][eE][dD]'_' [vV][aA][rR];
VAR_MATCHED_VARS:
	[mM][aA][tT][cC][hH][eE][dD]'_' [vV][aA][rR][sS];
VAR_MATCHED_VAR_NAME:
	[mM][aA][tT][cC][hH][eE][dD]'_' [vV][aA][rR]'_' [nN][aA][mM][eE];
VAR_MATCHED_VARS_NAMES:
	[mM][aA][tT][cC][hH][eE][dD]'_' [vV][aA][rR][sS]'_' [nN][aA][mM][eE][sS];
VAR_MODSEC_BUILD:
	[mM][oO][dD][sS][eE][cC]'_' [bB][uU][iI][lL][dD];
VAR_MSC_PCRE_LIMITS_EXCEEDED:
	[mM][sS][cC]'_' [pP][cC][rR][eE]'_' [lL][iI][mM][iI][tT][sS]'_' [eE][xX][cC][eE][eE][dD][eE][dD]
		;
VAR_MULTIPART_CRLF_LF_LINES:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [cC][rR][lL][fF]'_' [lL][fF]'_' [lL][iI][nN][eE][sS];
VAR_MULTIPART_FILENAME:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [fF][iI][lL][eE][nN][aA][mM][eE];
VAR_MULTIPART_NAME:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [nN][aA][mM][eE];
VAR_MULTIPART_PART_HEADERS:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [pP][aA][rR][tT]'_' [hH][eE][aA][dD][eE][rR][sS];
VAR_MULTIPART_STRICT_ERROR:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [sS][tT][rR][iI][cC][tT]'_' [eE][rR][rR][oO][rR];
VAR_MULTIPART_UNMATCHED_BOUNDARY:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [uU][nN][mM][aA][tT][cC][hH][eE][dD]'_' [bB][oO][uU][nN]
		[dD][aA][rR][yY];
VAR_OUTBOUND_DATA_ERROR:
	[oO][uU][tT][bB][oO][uU][nN][dD]'_' [dD][aA][tT][aA]'_' [eE][rR][rR][oO][rR];
VAR_PATH_INFO: [pP][aA][tT][hH]'_' [iI][nN][fF][oO];
VAR_QUERY_STRING:
	[qQ][uU][eE][rR][yY]'_' [sS][tT][rR][iI][nN][gG];
VAR_REMOTE_ADDR:
	[rR][eE][mM][oO][tT][eE]'_' [aA][dD][dD][rR];
VAR_REMOTE_HOST:
	[rR][eE][mM][oO][tT][eE]'_' [hH][oO][sS][tT];
VAR_REMOTE_PORT:
	[rR][eE][mM][oO][tT][eE]'_' [pP][oO][rR][tT];
VAR_REMOTE_USER:
	[rR][eE][mM][oO][tT][eE]'_' [uU][sS][eE][rR];
VAR_REQBODY_ERROR:
	[rR][eE][qQ][bB][oO][dD][yY]'_' [eE][rR][rR][oO][rR];
VAR_REQBODY_ERROR_MSG:
	[rR][eE][qQ][bB][oO][dD][yY]'_' [eE][rR][rR][oO][rR]'_' [mM][sS][gG];
VAR_REQBODY_PROCESSOR:
	[rR][eE][qQ][bB][oO][dD][yY]'_' [pP][rR][oO][cC][eE][sS][sS][oO][rR];
VAR_REQUEST_BASENAME:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [bB][aA][sS][eE][nN][aA][mM][eE];
VAR_REQUEST_BODY:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [bB][oO][dD][yY];
VAR_REQUEST_BODY_LENGTH:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [bB][oO][dD][yY]'_' [lL][eE][nN][gG][tT][hH];
VAR_REQUEST_COOKIES:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [cC][oO][oO][kK][iI][eE][sS];
VAR_REQUEST_COOKIES_NAMES:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [cC][oO][oO][kK][iI][eE][sS]'_' [nN][aA][mM][eE][sS];
VAR_REQUEST_FILENAME:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [fF][iI][lL][eE][nN][aA][mM][eE];
VAR_REQUEST_HEADERS:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [hH][eE][aA][dD][eE][rR][sS];
VAR_REQUEST_HEADERS_NAMES:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [hH][eE][aA][dD][eE][rR][sS]'_' [nN][aA][mM][eE][sS];
VAR_REQUEST_LINE:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [lL][iI][nN][eE];
VAR_REQUEST_METHOD:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [mM][eE][tT][hH][oO][dD];
VAR_REQUEST_PROTOCOL:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [pP][rR][oO][tT][oO][cC][oO][lL];
VAR_REQUEST_URI:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [uU][rR][iI];
VAR_REQUEST_URI_RAW:
	[rR][eE][qQ][uU][eE][sS][tT]'_' [uU][rR][iI]'_' [rR][aA][wW];
VAR_RESPONSE_BODY:
	[rR][eE][sS][pP][oO][nN][sS][eE]'_' [bB][oO][dD][yY];
VAR_RESPONSE_CONTENT_LENGTH:
	[rR][eE][sS][pP][oO][nN][sS][eE]'_' [cC][oO][nN][tT][eE][nN][tT]'_' [lL][eE][nN][gG][tT][hH];
VAR_RESPONSE_CONTENT_TYPE:
	[rR][eE][sS][pP][oO][nN][sS][eE]'_' [cC][oO][nN][tT][eE][nN][tT]'_' [tT][yY][pP][eE];
VAR_RESPONSE_HEADERS:
	[rR][eE][sS][pP][oO][nN][sS][eE]'_' [hH][eE][aA][dD][eE][rR][sS];
VAR_RESPONSE_HEADERS_NAMES:
	[rR][eE][sS][pP][oO][nN][sS][eE]'_' [hH][eE][aA][dD][eE][rR][sS]'_' [nN][aA][mM][eE][sS];
VAR_RESPONSE_PROTOCOL:
	[rR][eE][sS][pP][oO][nN][sS][eE]'_' [pP][rR][oO][tT][oO][cC][oO][lL];
VAR_RESPONSE_STATUS:
	[rR][eE][sS][pP][oO][nN][sS][eE]'_' [sS][tT][aA][tT][uU][sS];
VAR_RULE: [rR][uU][lL][eE];
VAR_SERVER_ADDR:
	[sS][eE][rR][vV][eE][rR]'_' [aA][dD][dD][rR];
VAR_SERVER_NAME:
	[sS][eE][rR][vV][eE][rR]'_' [nN][aA][mM][eE];
VAR_SERVER_PORT:
	[sS][eE][rR][vV][eE][rR]'_' [pP][oO][rR][tT];
VAR_SESSION: [sS][eE][sS][sS][iI][oO][nN];
VAR_SESSIONID: [sS][eE][sS][sS][iI][oO][nN][iI][dD];
VAR_STATUS_LINE:
	[sS][tT][aA][tT][uU][sS]'_' [lL][iI][nN][eE];
VAR_TIME: [tT][iI][mM][eE];
VAR_TIME_DAY: [tT][iI][mM][eE]'_' [dD][aA][yY];
VAR_TIME_EPOCH:
	[tT][iI][mM][eE]'_' [eE][pP][oO][cC][hH];
VAR_TIME_HOUR: [tT][iI][mM][eE]'_' [hH][oO][uU][rR];
VAR_TIME_MIN: [tT][iI][mM][eE]'_' [mM][iI][nN];
VAR_TIME_MON: [tT][iI][mM][eE]'_' [mM][oO][nN];
VAR_TIME_SEC: [tT][iI][mM][eE]'_' [sS][eE][cC];
VAR_TIME_WDAY: [tT][iI][mM][eE]'_' [wW][dD][aA][yY];
VAR_TIME_YEAR: [tT][iI][mM][eE]'_' [yY][eE][aA][rR];
VAR_TX: [tT][xX];
VAR_UNIQUE_ID: [uU][nN][iI][qQ][uU][eE]'_' [iI][dD];
VAR_URLENCODED_ERROR:
	[uU][rR][lL][eE][nN][cC][oO][dD][eE][dD]'_' [eE][rR][rR][oO][rR];
VAR_USERID: [uU][sS][eE][rR][iI][dD];
VAR_WEBAPPID: [wW][eE][bB][aA][pP][pP][iI][dD];
VAR_XML: [xX][mM][lL];
VAR_REQBODY_PROCESSOR_ERROR:
	[rR][eE][qQ][bB][oO][dD][yY]'_' [pP][rR][oO][cC][eE][sS][sS][oO][rR]'_' [eE][rR][rR][oO][rR];
VAR_MULTIPART_BOUNDARY_QUOTED:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [bB][oO][uU][nN][dD][aA][rR][yY]'_' [qQ][uU][oO][tT][eE]
		[dD];
VAR_MULTIPART_BOUNDARY_WHITESPACE:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [bB][oO][uU][nN][dD][aA][rR][yY]'_' [wW][hH][iI][tT][eE]
		[sS][pP][aA][cC][eE];
VAR_MULTIPART_DATA_BEFORE:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [dD][aA][tT][aA]'_' [bB][eE][fF][oO][rR][eE];
VAR_MULTIPART_DATA_AFTER:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [dD][aA][tT][aA]'_' [aA][fF][tT][eE][rR];
VAR_MULTIPART_HEADER_FOLDING:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [hH][eE][aA][dD][eE][rR]'_' [fF][oO][lL][dD][iI][nN][gG]
		;
VAR_MULTIPART_LF_LINE:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [lL][fF]'_' [lL][iI][nN][eE];
VAR_MULTIPART_MISSING_SEMICOLON:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [mM][iI][sS][sS][iI][nN][gG]'_' [sS][eE][mM][iI][cC][oO]
		[lL][oO][nN];
VAR_MULTIPART_INVALID_QUOTING:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [iI][nN][vV][aA][lL][iI][dD]'_' [qQ][uU][oO][tT][iI][nN]
		[gG];
VAR_MULTIPART_INVALID_PART:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [iI][nN][vV][aA][lL][iI][dD]'_' [pP][aA][rR][tT];
VAR_MULTIPART_INVALID_HEADER_FOLDING:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [iI][nN][vV][aA][lL][iI][dD] '_' [hH][eE][aA][dD][eE]
		[rR]'_' [fF][oO][lL][dD][iI][nN][gG];
VAR_MULTIPART_FILE_LIMIT_EXCEEDED:
	[mM][uU][lL][tT][iI][pP][aA][rR][tT]'_' [fF][iI][lL][eE]'_' [lL][iI][mM][iI][tT]'_' [eE][xX][cC]
		[eE][eE][dD][eE][dD];
ModeSecRuleVariableName_WS: WS -> skip, popMode;
ModeSecRuleVariableName_COMMA: COMMA -> skip, popMode;
ModeSecRuleVariableName_PIPE: PIPE -> type(PIPE);
ModeSecRuleVariableName_COLON:
	COLON -> type(COLON), pushMode(ModeSecRuleVariableSubName);
ModeSecRuleVariableName_DOT:
	DOT -> type(DOT), pushMode(ModeSecRuleVariableSubName);
ModeSecRuleVariableName_VAR_COUNT: '&' -> type(VAR_COUNT);
ModeSecRuleVariableName_VAR_NOT: NOT -> type(NOT);
ModeSecRuleVariableName_LEFT_BRACKET:
	LEFT_BRACKET -> type(LEFT_BRACKET);
ModeSecRuleVariableName_RIGHT_BRACKET:
	RIGHT_BRACKET -> type(RIGHT_BRACKET), popMode;

mode ModeSecRuleVariableSubName;
ModeSecRuleVariableSubName_VAR_SUB_NAME:
	~[ :!&|",%{}\n]+ -> type(STRING), popMode;

mode ModeSecRuleOperator;
ModeSecRuleOperator_NOT: NOT -> type(NOT);
AT: '@' -> popMode, pushMode(ModeSecRuleOperatorName);
ModeSecRuleOperator_QUOTE:
	QUOTE -> type(QUOTE), popMode, pushMode(ModeSecRuleAction);
RX_DEFUALT: ('\\"' | ~[" @!%] | ('%' ~[{])) (
		'\\"'
		| ~["%]
		| ('%' ~[{])
	)* -> type(STRING);
ModeSecRuleOperator_PER_CENT:
	PER_CENT -> type(PER_CENT), pushMode(ModeSecRuleVariableName);

mode ModeSecRuleOperatorName;
ModeSecRuleOperator_WS:
	WS -> skip, popMode, pushMode(ModeSecRuleOperatorValue);
OP_BEGINS_WITH: 'beginsWith';
OP_CONTAINS: 'contains';
OP_CONTAINS_WORD: 'containsWord';
OP_DETECT_SQLI:
	'detectSQLi' -> popMode, pushMode(ModeSecRuleOperatorValue);
OP_DETECT_XSS:
	'detectXSS' -> popMode, pushMode(ModeSecRuleOperatorValue);
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
OP_UNCONDITIONAL_MATCH:
	'unconditionalMatch' -> popMode, pushMode(ModeSecRuleOperatorValue);
OP_VALIDATE_BYTE_RANGE: 'validateByteRange';
OP_VALIDATE_DTD: 'validateDTD';
OP_VALIDATE_SCHEMA: 'validateSchema';
OP_VALIDATE_URL_ENCODING:
	'validateUrlEncoding' -> popMode, pushMode(ModeSecRuleOperatorValue);
OP_VALIDATE_UTF8_ENCODING:
	'validateUtf8Encoding' -> popMode, pushMode(ModeSecRuleOperatorValue);
OP_VERIFY_CC: 'verifyCC';
OP_VERIFY_CPF: 'verifyCPF';
OP_VERIFY_SSN: 'verifySSN';
OP_WITHIN: 'within';

mode ModeSecRuleOperatorValue;
ModeSecRuleOperatorValue_QUOTE:
	QUOTE -> type(QUOTE), popMode, pushMode(ModeSecRuleAction);
ModeSecRuleOperatorValue_STRING: ('\\"' | ~["%] | ('%' ~[{])) (
		'\\"'
		| ~["%]
		| ('%' ~[{])
	)* -> type(STRING);
ModeSecRuleOperatorValue_PER_CENT:
	PER_CENT -> type(PER_CENT), pushMode(ModeSecRuleVariableName);

mode ModeSecRuleAction;
ModeSecRuleAction_WS: (([ \t]+) | NL) -> skip;
ModeSecRuleAction_END: '\r'? '\n' -> skip, popMode;
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
TX: [tT][xX];
ModeSecRuleActionSetVar_DOT: DOT -> type(DOT);
ModeSecRuleActionSetVar_NOT: NOT -> type(NOT);
ModeSecRuleActionSetVar_COMMA: COMMA -> type(COMMA), popMode;
ModeSecRuleActionSetVar_QUOTE:
	QUOTE -> type(QUOTE), popMode, popMode;
ModeSecRuleActionSetVarName_ASSIGN:
	ASSIGN -> type(ASSIGN), popMode, pushMode(ModeSecRuleActionSetVarValue);
ModeSecRuleActionSetVarName_PER_CENT:
	PER_CENT -> type(PER_CENT), pushMode(ModeSecRuleVariableName);
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
	PER_CENT -> type(PER_CENT), pushMode(ModeSecRuleVariableName);
VAR_VALUE: ~[+\-'",](~[{}%'",] | ('%' ~[{]))*;

mode ModeSecRuleActionString;
ModeSecRuleActionSetVarString_SINGLE_QUOTE:
	SINGLE_QUOTE -> type(SINGLE_QUOTE), popMode;
ModeSecRuleActionSetVarString_STRING: (
		'\\\''
		| ~['%]
		| ('%' ~[{])
	) ('\\\'' | ~['%] | ('%' ~[{]))* -> type(STRING);
ModeSecRuleActionString_PER_CENT:
	PER_CENT -> type(PER_CENT), pushMode(ModeSecRuleVariableName);

mode ModeSecRuleActionSetUid;
ModeSecRuleActionSetUid_PER_CENT:
	PER_CENT -> type(PER_CENT), pushMode(ModeSecRuleVariableName);
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