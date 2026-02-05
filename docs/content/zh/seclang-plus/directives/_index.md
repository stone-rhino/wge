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
| [`Include`](./include/) | 将外部配置文件包含到当前配置中 |
| [`SecAction`](./secaction/) | 无条件执行指定的动作 |
| [`SecArgumentSeparator`](./secargumentseparator/) | 指定参数分隔符字符 |
| [`SecArgumentsLimit`](./secargumentslimit/) | 配置可接受的最大参数数量 |
| [`SecAuditEngine`](./secauditengine/) | 配置审计日志引擎 |
| [`SecAuditLog`](./secauditlog/) | 配置主审计日志文件的路径 |
| [`SecAuditLog2`](./secauditlog2/) | 配置辅助审计日志文件的路径 |
| [`SecAuditLogDirMode`](./secauditlogdirmode/) | 配置并发审计日志目录的权限模式 |
| [`SecAuditLogFileMode`](./secauditlogfilemode/) | 配置审计日志文件的权限模式 |
| [`SecAuditLogFormat`](./secauditlogformat/) | 配置审计日志的输出格式 |
| [`SecAuditLogParts`](./secauditlogparts/) | 配置审计日志中记录事务的哪些部分 |
| [`SecAuditLogRelevantStatus`](./secauditlogrelevantstatus/) | 配置审计日志的相关HTTP状态码 |
| [`SecAuditLogStorageDir`](./secauditlogstoragedir/) | 配置并发模式下审计日志存储目录 |
| [`SecAuditLogType`](./secauditlogtype/) | 配置审计日志的记录机制类型 |
| [`SecCollectionTimeout`](./seccollectiontimeout/) | 配置持久化集合的过期时间 |
| [`SecComponentSignature`](./seccomponentsignature/) | 添加组件签名信息到日志中 |
| [`SecCookieFormat`](./seccookieformat/) | 配置Cookie解析格式版本 |
| [`SecDataDir`](./secdatadir/) | 配置持久化数据存储目录 |
| [`SecDebugLog`](./secdebuglog/) | 配置调试日志文件路径 |
| [`SecDebugLogLevel`](./secdebugloglevel/) | 配置调试日志的详细级别 |
| [`SecDefaultAction`](./secdefaultaction/) | 定义特定阶段的默认动作列表 |
| [`SecGeoLookupDb`](./secgeolookupdb/) | 配置地理位置数据库文件路径 |
| [`SecHttpBlKey`](./sechttpblkey/) | 配置HTTP黑名单API密钥 |
| [`SecMarker`](./secmarker/) | 创建规则标记用作skipAfter跳转目标 |
| [`SecPcreMatchLimit`](./secpcrematchlimit/) | 配置PCRE正则表达式匹配的最大次数 |
| [`SecPcreMatchLimitRecursion`](./secpcrematchlimitrecursion/) | 配置PCRE正则表达式递归深度限制 |
| [`SecRemoteRules`](./secremoterules/) | 从远程HTTPS服务器加载规则配置 |
| [`SecRemoteRulesFailAction`](./secremoterulesfailaction/) | 配置远程规则加载失败时的处理方式 |
| [`SecRequestBodyAccess`](./secrequestbodyaccess/) | 配置是否允许WAF访问请求体内容 |
| [`SecRequestBodyInMemoryLimit`](./secrequestbodyinmemorylimit/) | 配置请求体在内存中缓冲的最大大小 |
| [`SecRequestBodyJsonDepthLimit`](./secrequestbodyjsondepthlimit/) | 配置JSON对象的最大解析深度 |
| [`SecRequestBodyLimit`](./secrequestbodylimit/) | 配置可接受的最大请求体大小 |
| [`SecRequestBodyLimitAction`](./secrequestbodylimitaction/) | 配置请求体超过限制时的处理方式 |
| [`SecRequestBodyNoFilesLimit`](./secrequestbodynofileslimit/) | 配置不包含文件上传的请求体最大大小 |
| [`SecResponseBodyAccess`](./secresponsebodyaccess/) | 配置是否允许WAF访问响应体内容 |
| [`SecResponseBodyLimit`](./secresponsebodylimit/) | 配置响应体缓冲区的最大大小 |
| [`SecResponseBodyLimitAction`](./secresponsebodylimitaction/) | 配置响应体超过限制时的处理方式 |
| [`SecResponseBodyMimeType`](./secresponsebodymimetype/) | 配置需要检查的响应体MIME类型 |
| [`SecResponseBodyMimeTypesClear`](./secresponsebodymimetypesclear/) | 清除所有响应体MIME类型配置 |
| [`SecRule`](./secrule/) | 定义一个安全规则 |
| [`SecRuleEngine`](./secruleengine/) | 配置规则引擎的运行模式 |
| [`SecRuleRemoveById`](./secruleremovebyid/) | 根据ID移除规则 |
| [`SecRuleRemoveByMsg`](./secruleremovebymsg/) | 根据msg内容移除规则 |
| [`SecRuleRemoveByTag`](./secruleremovebytag/) | 根据tag移除规则 |
| [`SecRuleScript`](./secrulescript/) | 使用Lua脚本定义规则逻辑 |
| [`SecRuleUpdateActionById`](./secruleupdateactionbyid/) | 根据ID更新规则的动作 |
| [`SecRuleUpdateOperatorById`](./secruleupdateoperatorbyid/) | 通过规则ID更新规则的操作符 (WGE扩展) |
| [`SecRuleUpdateOperatorByTag`](./secruleupdateoperatorbytag/) | 通过标签更新规则的操作符 (WGE扩展) |
| [`SecRuleUpdateTargetById`](./secruleupdatetargetbyid/) | 根据ID更新规则的参数列表 |
| [`SecRuleUpdateTargetByMsg`](./secruleupdatetargetbymsg/) | 根据消息内容更新规则的参数列表 |
| [`SecRuleUpdateTargetByTag`](./secruleupdatetargetbytag/) | 根据标签更新规则的参数列表 |
| [`SecStatusEngine`](./secstatusengine/) | 配置状态引擎收集运行时统计信息 |
| [`SecTmpDir`](./sectmpdir/) | 配置临时文件存储目录 |
| [`SecTmpSaveUploadedFiles`](./sectmpsaveuploadedfiles/) | 配置是否保存上传文件到临时目录 |
| [`SecUnicodeMapFile`](./secunicodemapfile/) | 配置Unicode映射文件路径和代码页 |
| [`SecUploadDir`](./secuploaddir/) | 配置文件上传的存储目录 |
| [`SecUploadFileLimit`](./secuploadfilelimit/) | 配置单次请求允许上传的最大文件数量 |
| [`SecUploadFileMode`](./secuploadfilemode/) | 配置上传文件的权限模式 |
| [`SecUploadKeepFiles`](./secuploadkeepfiles/) | 配置是否在请求处理后保留上传文件 |
| [`SecWebAppId`](./secwebappid/) | 配置Web应用程序标识符 |
| [`SecXmlExternalEntity`](./secxmlexternalentity/) | 配置是否允许XML外部实体处理 |
