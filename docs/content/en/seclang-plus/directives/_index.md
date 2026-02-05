+++
title = "Directives"
weight = 1
ordersectionsby = "title"
+++

This section contains the rule directives supported by WGE. Among these, only `SecRule` and `SecAction` are runtime directives; all others are configuration-phase directives that take effect immediately upon parsing. For directives like `SecRuleRemoveById` that modify the state of specific rules, they must be loaded after the corresponding rules.

Some directives are not implemented in WGE. Most of these can be parsed normally by WGE (such as `SecArgumentSeparator`), but their functionality is not implemented - this will not cause crashes or errors. All subpages will indicate whether a directive is implemented; please check before use.

Additionally, WGE has added some directives not present in ModSecurity (such as `SecRuleUpdateOperatorById`) for more convenient usage.

| Keyword | Description |
|---------|-------------|
| [`Include`](./include/) | Include an external configuration file into the current configuration |
| [`SecAction`](./secaction/) | Unconditionally execute the specified actions |
| [`SecArgumentSeparator`](./secargumentseparator/) | Specify the parameter separator character |
| [`SecArgumentsLimit`](./secargumentslimit/) | Configure the maximum number of acceptable parameters |
| [`SecAuditEngine`](./secauditengine/) | Configure the audit logging engine |
| [`SecAuditLog`](./secauditlog/) | Configure the path to the primary audit log file |
| [`SecAuditLog2`](./secauditlog2/) | Configure the path to the secondary audit log file |
| [`SecAuditLogDirMode`](./secauditlogdirmode/) | Configure the directory permission mode for audit log directories |
| [`SecAuditLogFileMode`](./secauditlogfilemode/) | Configure the file permission mode for audit log files |
| [`SecAuditLogFormat`](./secauditlogformat/) | Configure the format of the audit log output |
| [`SecAuditLogParts`](./secauditlogparts/) | Configure which parts of a transaction should be recorded |
| [`SecAuditLogRelevantStatus`](./secauditlogrelevantstatus/) | Configure relevant HTTP status codes for audit logging |
| [`SecAuditLogStorageDir`](./secauditlogstoragedir/) | Configure the directory for storing audit log files in concurrent mode |
| [`SecAuditLogType`](./secauditlogtype/) | Configure the type of audit logging mechanism |
| [`SecCollectionTimeout`](./seccollectiontimeout/) | Configure the expiration time for persistent collections |
| [`SecComponentSignature`](./seccomponentsignature/) | Add component signature information to the log |
| [`SecCookieFormat`](./seccookieformat/) | Configure the cookie parsing format version |
| [`SecDataDir`](./secdatadir/) | Configure the persistent data storage directory |
| [`SecDebugLog`](./secdebuglog/) | Configure the debug log file path |
| [`SecDebugLogLevel`](./secdebugloglevel/) | Configure the verbosity level of the debug log |
| [`SecDefaultAction`](./secdefaultaction/) | Define the default action list for a specific phase |
| [`SecGeoLookupDb`](./secgeolookupdb/) | Configure the geolocation database file path |
| [`SecHttpBlKey`](./sechttpblkey/) | Configure the HTTP Blacklist API key for @rbl usage |
| [`SecMarker`](./secmarker/) | Create a rule marker for the skipAfter action |
| [`SecPcreMatchLimit`](./secpcrematchlimit/) | Configure the maximum number of PCRE regex matches |
| [`SecPcreMatchLimitRecursion`](./secpcrematchlimitrecursion/) | Configure the PCRE regex recursion depth limit |
| [`SecRemoteRules`](./secremoterules/) | Load rule configuration from a remote HTTPS server |
| [`SecRemoteRulesFailAction`](./secremoterulesfailaction/) | Configure how to handle remote rule loading failures |
| [`SecRequestBodyAccess`](./secrequestbodyaccess/) | Configure whether the WAF can access request body content |
| [`SecRequestBodyInMemoryLimit`](./secrequestbodyinmemorylimit/) | Configure the maximum size of request body buffered in memory |
| [`SecRequestBodyJsonDepthLimit`](./secrequestbodyjsondepthlimit/) | Configure the maximum parsing depth for JSON objects |
| [`SecRequestBodyLimit`](./secrequestbodylimit/) | Configure the maximum acceptable request body size |
| [`SecRequestBodyLimitAction`](./secrequestbodylimitaction/) | Configure action when request body exceeds limit |
| [`SecRequestBodyNoFilesLimit`](./secrequestbodynofileslimit/) | Configure the maximum request body size without file uploads |
| [`SecResponseBodyAccess`](./secresponsebodyaccess/) | Configure whether the WAF can access response body content |
| [`SecResponseBodyLimit`](./secresponsebodylimit/) | Configure the maximum response body buffer size |
| [`SecResponseBodyLimitAction`](./secresponsebodylimitaction/) | Configure action when response body exceeds limit |
| [`SecResponseBodyMimeType`](./secresponsebodymimetype/) | Configure which response body MIME types should be inspected |
| [`SecResponseBodyMimeTypesClear`](./secresponsebodymimetypesclear/) | Clear all response body MIME type configurations |
| [`SecRule`](./secrule/) | Define a security rule |
| [`SecRuleEngine`](./secruleengine/) | Configure the operating mode of the rule engine |
| [`SecRuleRemoveById`](./secruleremovebyid/) | Remove rules by ID |
| [`SecRuleRemoveByMsg`](./secruleremovebymsg/) | Remove rules by msg content |
| [`SecRuleRemoveByTag`](./secruleremovebytag/) | Remove rules by tag |
| [`SecRuleScript`](./secrulescript/) | Define rule logic using Lua scripts |
| [`SecRuleUpdateActionById`](./secruleupdateactionbyid/) | Update a rule's actions by ID |
| [`SecRuleUpdateOperatorById`](./secruleupdateoperatorbyid/) | Update the operator of a rule by ID (WGE extension) |
| [`SecRuleUpdateOperatorByTag`](./secruleupdateoperatorbytag/) | Update the operator of rules by tag (WGE extension) |
| [`SecRuleUpdateTargetById`](./secruleupdatetargetbyid/) | Update a rule's variable list by ID |
| [`SecRuleUpdateTargetByMsg`](./secruleupdatetargetbymsg/) | Update a rule's variable list by message content |
| [`SecRuleUpdateTargetByTag`](./secruleupdatetargetbytag/) | Update a rule's variable list by tag |
| [`SecStatusEngine`](./secstatusengine/) | Configure the status engine for runtime statistics |
| [`SecTmpDir`](./sectmpdir/) | Configure the temporary file storage directory |
| [`SecTmpSaveUploadedFiles`](./sectmpsaveuploadedfiles/) | Configure whether to save uploaded files to temp directory |
| [`SecUnicodeMapFile`](./secunicodemapfile/) | Configure the Unicode mapping file path and code page |
| [`SecUploadDir`](./secuploaddir/) | Configure the storage directory for file uploads |
| [`SecUploadFileLimit`](./secuploadfilelimit/) | Configure the maximum number of files per upload request |
| [`SecUploadFileMode`](./secuploadfilemode/) | Configure the permission mode for uploaded files |
| [`SecUploadKeepFiles`](./secuploadkeepfiles/) | Configure whether to keep uploaded files after processing |
| [`SecWebAppId`](./secwebappid/) | Configure the web application identifier |
| [`SecXmlExternalEntity`](./secxmlexternalentity/) | Configure whether XML external entity processing is allowed |
