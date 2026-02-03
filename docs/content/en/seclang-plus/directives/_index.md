+++
title = "Directives"
weight = 1
+++

| Keyword | Description |
|---------|-------------|
| [`SecRuleEngine`](./secruleengine/) | Configure rule engine operating mode |
| [`SecRequestBodyAccess`](./secrequestbodyaccess/) | Enable request body inspection |
| [`SecRequestBodyLimit`](./secrequestbodylimit/) | Maximum request body size |
| [`SecRequestBodyNoFilesLimit`](./secrequestbodynofileslimit/) | Maximum request body size without files |
| [`SecRequestBodyLimitAction`](./secrequestbodylimitaction/) | Action when request body exceeds limit |
| [`SecRequestBodyJsonDepthLimit`](./secrequestbodyjsondepthlimit/) | Maximum JSON parsing depth |
| [`SecResponseBodyAccess`](./secresponsebodyaccess/) | Enable response body inspection |
| [`SecResponseBodyMimeType`](./secresponsebodymimetype/) | MIME types for response inspection |
| [`SecResponseBodyLimit`](./secresponsebodylimit/) | Maximum response body buffer size |
| [`SecArgumentsLimit`](./secargumentslimit/) | Maximum number of parameters |
| [`SecArgumentSeparator`](./secargumentseparator/) | Parameter separator character |
| [`SecPcreMatchLimit`](./secpcrematchlimit/) | PCRE match limit |
| [`SecPcreMatchLimitRecursion`](./secpcrematchlimitrecursion/) | PCRE recursion depth limit |
| [`SecTmpDir`](./sectmpdir/) | Temporary file directory |
| [`SecDataDir`](./secdatadir/) | Persistent data directory |
| [`SecUnicodeMapFile`](./secunicodemapfile/) | Unicode mapping file |
| [`SecDebugLog`](./secdebuglog/) | Debug log file path |
| [`SecDebugLogLevel`](./secdebugloglevel/) | Debug log verbosity level |
| [`SecStatusEngine`](./secstatusengine/) | Status engine for statistics |
| [`SecCookieFormat`](./seccookieformat/) | Cookie parsing format |
| [`SecCollectionTimeout`](./seccollectiontimeout/) | Collection expiration time |
| [`SecRequestBodyInMemoryLimit`](./secrequestbodyinmemorylimit/) | Memory limit for request body |
| [`SecResponseBodyMimeTypesClear`](./secresponsebodymimetypesclear/) | Clear MIME type configuration |
| [`SecResponseBodyLimitAction`](./secresponsebodylimitaction/) | Action when response body exceeds limit |
| [`SecXmlExternalEntity`](./secxmlexternalentity/) | XML external entity processing |
| [`SecGeoLookupDb`](./secgeolookupdb/) | Geolocation database path |
| [`SecHttpBlKey`](./sechttpblkey/) | HTTP Blacklist API key |
| [`SecRemoteRules`](./secremoterules/) | Load rules from remote server |
| [`SecRemoteRulesFailAction`](./secremoterulesfailaction/) | Action on remote rule load failure |
| [`SecWebAppId`](./secwebappid/) | Web application identifier |
| [`SecComponentSignature`](./seccomponentsignature/) | Component signature for logs |
| [`SecTmpSaveUploadedFiles`](./sectmpsaveuploadedfiles/) | Save uploaded files to temp |
| [`SecUploadDir`](./secuploaddir/) | Upload storage directory |
| [`SecUploadFileLimit`](./secuploadfilelimit/) | Maximum upload file count |
| [`SecUploadFileMode`](./secuploadfilemode/) | Upload file permissions |
| [`SecUploadKeepFiles`](./secuploadkeepfiles/) | Keep uploaded files after processing |
| [`SecRule`](./secrule/) | Define a security rule |
| [`SecAction`](./secaction/) | Unconditionally execute actions |
| [`SecDefaultAction`](./secdefaultaction/) | Default action list |
| [`SecRuleRemoveById`](./secruleremovebyid/) | Remove rules by ID |
| [`SecRuleRemoveByMsg`](./secruleremovebymsg/) | Remove rules by message |
| [`SecRuleRemoveByTag`](./secruleremovebytag/) | Remove rules by tag |
| [`SecRuleUpdateActionById`](./secruleupdateactionbyid/) | Update rule actions by ID |
| [`SecRuleUpdateTargetById`](./secruleupdatetargetbyid/) | Update rule targets by ID |
| [`SecRuleUpdateTargetByMsg`](./secruleupdatetargetbymsg/) | Update rule targets by message |
| [`SecRuleUpdateTargetByTag`](./secruleupdatetargetbytag/) | Update rule targets by tag |
| [`SecRuleScript`](./secrulescript/) | Lua script rule |
