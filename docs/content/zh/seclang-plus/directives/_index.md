+++
title = "规则指令"
weight = 1
ordersectionsby = "title"
+++

这部分包含了WGE支持的规则指令，其中除了`SecRule`与`SecAction`是运行时的，其它都是配置阶段的，也就是解析立即生效的，同时对于`SecRuleRemoveById`这种修改指定规则状态的指令必须保证其在对应规则后被加载。

其中有的指令WGE并没实现，其中大部分WGE能够正常解析（如`SecArgumentSeparator`），但是其功能并没有实现，这种并不会崩溃或者报错，这种所有子页面会增加说明其并没有实现，使用前需要注意查询。

另外WGE增加了部分Modsecurity并没有的指令（如`SecRuleUpdateOperatorById`），以便更方便的使用。

| 关键字 | 说明 |
|--------|------|
| [`SecRuleEngine`](./secruleengine/) | - |
| [`SecRequestBodyAccess`](./secrequestbodyaccess/) | - |
| [`SecRequestBodyLimit`](./secrequestbodylimit/) | - |
| [`SecRequestBodyNoFilesLimit`](./secrequestbodynofileslimit/) | - |
| [`SecRequestBodyLimitAction`](./secrequestbodylimitaction/) | - |
| [`SecRequestBodyJsonDepthLimit`](./secrequestbodyjsondepthlimit/) | - |
| [`SecResponseBodyAccess`](./secresponsebodyaccess/) | - |
| [`SecResponseBodyMimeType`](./secresponsebodymimetype/) | - |
| [`SecResponseBodyLimit`](./secresponsebodylimit/) | - |
| [`SecArgumentsLimit`](./secargumentslimit/) | - |
| [`SecArgumentSeparator`](./secargumentseparator/) | - |
| [`SecPcreMatchLimit`](./secpcrematchlimit/) | - |
| [`SecPcreMatchLimitRecursion`](./secpcrematchlimitrecursion/) | - |
| [`SecTmpDir`](./sectmpdir/) | - |
| [`SecDataDir`](./secdatadir/) | - |
| [`SecUnicodeMapFile`](./secunicodemapfile/) | - |
| [`SecDebugLog`](./secdebuglog/) | - |
| [`SecDebugLogLevel`](./secdebugloglevel/) | - |
| [`SecStatusEngine`](./secstatusengine/) | - |
| [`SecCookieFormat`](./seccookieformat/) | - |
| [`SecCollectionTimeout`](./seccollectiontimeout/) | - |
| [`SecRequestBodyInMemoryLimit`](./secrequestbodyinmemorylimit/) | - |
| [`SecResponseBodyMimeTypesClear`](./secresponsebodymimetypesclear/) | - |
| [`SecResponseBodyLimitAction`](./secresponsebodylimitaction/) | - |
| [`SecXmlExternalEntity`](./secxmlexternalentity/) | - |
| [`SecGeoLookupDb`](./secgeolookupdb/) | - |
| [`SecHttpBlKey`](./sechttpblkey/) | - |
| [`SecRemoteRules`](./secremoterules/) | - |
| [`SecRemoteRulesFailAction`](./secremoterulesfailaction/) | - |
| [`SecWebAppId`](./secwebappid/) | - |
| [`SecComponentSignature`](./seccomponentsignature/) | - |
| [`SecTmpSaveUploadedFiles`](./sectmpsaveuploadedfiles/) | - |
| [`SecUploadDir`](./secuploaddir/) | - |
| [`SecUploadFileLimit`](./secuploadfilelimit/) | - |
| [`SecUploadFileMode`](./secuploadfilemode/) | - |
| [`SecUploadKeepFiles`](./secuploadkeepfiles/) | - |
| [`SecRule`](./secrule/) | - |
| [`SecAction`](./secaction/) | - |
| [`SecDefaultAction`](./secdefaultaction/) | - |
| [`SecRuleRemoveById`](./secruleremovebyid/) | - |
| [`SecRuleRemoveByMsg`](./secruleremovebymsg/) | - |
| [`SecRuleRemoveByTag`](./secruleremovebytag/) | - |
| [`SecRuleUpdateActionById`](./secruleupdateactionbyid/) | - |
| [`SecRuleUpdateTargetById`](./secruleupdatetargetbyid/) | - |
| [`SecRuleUpdateTargetByMsg`](./secruleupdatetargetbymsg/) | - |
| [`SecRuleUpdateTargetByTag`](./secruleupdatetargetbytag/) | - |
| [`SecRuleScript`](./secrulescript/) | - |
