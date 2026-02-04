+++
title = "配置命令"
weight = 1
+++

本章详细介绍 SecLang++ 中的配置命令（指令），用于配置 WAF 引擎的全局行为。

## 配置指令




### Include


> **描述:** 包含其他配置文件。


**语法:** `Include "path/to/file.conf"`


**示例:**


```
Include "crs-setup.conf"
Include "rules/*.conf"
```




**区分大小写:** 是





## 引擎配置指令




### SecRuleEngine


> **描述:** 配置规则引擎的运行模式。


**语法:** `SecRuleEngine On|Off|DetectionOnly`


**默认值:** Off


**版本:** 1.0



可选值:


- **On:** 启用规则引擎,执行所有规则并应用破坏性动作
- **Off:** 禁用规则引擎,不执行任何规则
- **DetectionOnly:** 仅检测模式,执行规则但不应用破坏性动作

**区分大小写:** 是



**示例:**


```apache
SecRuleEngine On
```




### SecRequestBodyAccess


> **描述:** 配置是否允许 WAF 访问请求体内容。


**语法:** `SecRequestBodyAccess On|Off`


**默认值:** Off


**版本:** 1.0





**区分大小写:** 是



若需检查请求主体传输的数据（如 POST 参数），则必须使用此指令。为实现可靠阻塞，请求缓冲亦不可或缺。可选值：


- **On:** 缓冲并检查请求主体
- **Off:** 不缓冲请求主体

**示例:**


```
SecRequestBodyAccess On
```




### SecRequestBodyLimit


> **描述:** 配置可接受的最大请求体大小 (字节)。


**语法:** `SecRequestBodyLimit LIMIT`


**默认值:** 13107200 (12.5MB)


**区分大小写:** 是



超过限制的请求将返回状态码 413（请求实体过大）被拒绝。硬性上限为 1 GB。


**示例:**


```
SecRequestBodyLimit 13107200
```




### SecRequestBodyNoFilesLimit


> **描述:** 配置不包含文件上传的请求体最大大小 (字节)。


**语法:** `SecRequestBodyNoFilesLimit LIMIT`


**默认值:** 131072 (128KB)


**区分大小写:** 是



通常而言，默认值不够小。对于大多数应用程序，您应能将其降低至 128 KB 或更低。超过限制的请求将返回状态码 413（请求实体过大）被拒绝。存在 1 GB 的硬性上限。


**示例:**


```
SecRequestBodyNoFilesLimit 131072
```




### SecRequestBodyLimitAction


> **描述:** 配置当请求体超过 SecRequestBodyLimit 限制时的处理方式。


**语法:** `SecRequestBodyLimitAction Reject|ProcessPartial`


**默认值:** Reject


**可选值:**


- **Reject:** 拒绝超限请求
- **ProcessPartial:** 仅检查符合限制的部分请求体，放行其余内容

**区分大小写:** 是



当 WAF 处于 DetectionOnly 模式且需完全被动运行时，使用 ProcessPartial 可避免干扰事务。可通过创建规则检查 INBOUND_DATA_ERROR 变量来检测请求体是否超限。


**示例:**


```
SecRequestBodyLimitAction Reject
```




### SecRequestBodyJsonDepthLimit


> **描述:** 配置 JSON 对象的最大解析深度。


**语法:** `SecRequestBodyJsonDepthLimit DEPTH`


**默认值:** 512


**区分大小写:** 是



解析 JSON 对象时，若嵌套层级超过配置深度限制，解析将终止并设置 REQBODY_ERROR 错误。


**示例:**


```
SecRequestBodyJsonDepthLimit 512
```




### SecResponseBodyAccess


> **描述:** 配置是否允许 WAF 访问响应体内容。


**语法:** `SecResponseBodyAccess On|Off`


**默认值:** Off


**区分大小写:** 是



若需检查 HTML 响应并实现响应阻断，则必须使用此指令。可选值：


- **On:** 缓冲并检查响应体
- **Off:** 不缓冲响应体

**示例:**


```
SecResponseBodyAccess On
```




### SecResponseBodyMimeType


> **描述:** 配置需要检查的响应体 MIME 类型。


**语法:** `SecResponseBodyMimeType MIME_TYPE1 MIME_TYPE2 ...`


**区分大小写:** 是



可使用多个 SecResponseBodyMimeType 指令添加 MIME 类型。使用 SecResponseBodyMimeTypesClear 清除先前配置的 MIME 类型并重新开始。


**示例:**


```
SecResponseBodyMimeType text/plain text/html text/xml
```




### SecResponseBodyLimit


> **描述:** 配置响应体缓冲区的最大大小 (字节)。


**语法:** `SecResponseBodyLimit LIMIT`


**默认值:** 524288 (512KB)


**区分大小写:** 是



超过此限制的响应将被拒绝。此设置不影响未选中缓冲的 MIME 类型的响应。存在 1 GB 的硬性限制。


**示例:**


```
SecResponseBodyLimit 524288
```




### SecArgumentsLimit


> **描述:** 配置可接受的最大参数数量。


**语法:** `SecArgumentsLimit LIMIT`


**区分大小写:** 是



使用此设置时，建议配合规则检测相同整数值，达到上限时拒绝请求。例如：
若未配置匹配规则，攻击者可能通过将攻击有效载荷放置在超出限制的参数中规避检测。


**示例:**


```
SecArgumentsLimit 1000
```




### SecArgumentSeparator


> **描述:** 指定应用于 application/x-www-form-urlencoded 内容的参数分隔符字符。


**语法:** `SecArgumentSeparator CHARACTER`


**默认值:** &


**区分大小写:** 是



当后端 Web 应用程序使用非标准参数分隔符时需要此指令。某些应用程序会采用分号作为分隔符。除非确认所处理的应用程序需要不同分隔符，否则不应更改默认设置。若未为每个 Web 应用程序正确配置此指令，将无法正确解析参数，导致规则匹配效果显著降低。


**示例:**


```
SecArgumentSeparator &
```




### SecPcreMatchLimit


> **描述:** 配置 PCRE 正则表达式匹配的最大次数,防止 ReDoS 攻击。


**语法:** `SecPcreMatchLimit LIMIT`


**默认值:** 3000


**区分大小写:** 是



若配置的限制被超过，将设置变量 MSC_PCRE_LIMITS_EXCEEDED。


**示例:**


```
SecPcreMatchLimit 3000
```




### SecPcreMatchLimitRecursion


> **描述:** 配置 PCRE 正则表达式递归深度限制。


**语法:** `SecPcreMatchLimitRecursion LIMIT`


**默认值:** 1000


**区分大小写:** 是



此指令限制正则表达式匹配时的递归调用深度，用于防止复杂正则表达式导致的栈溢出。当正则表达式包含嵌套的分组或回溯时，递归深度可能快速增加。若达到限制，匹配将终止并设置 MSC_PCRE_LIMITS_EXCEEDED 变量。


**示例:**


```
SecPcreMatchLimitRecursion 1000
```




### SecTmpDir


> **描述:** 配置临时文件存储目录。


**语法:** `SecTmpDir PATH`


**区分大小写:** 是



此目录用于存储请求体处理过程中的临时文件，特别是当请求体大小超过 SecRequestBodyInMemoryLimit 设置时，WAF 将把超出部分写入此目录。该目录需对运行 WAF 进程的用户具有可写权限。


**示例:**


```
SecTmpDir /tmp/
```




### SecDataDir


> **描述:** 配置持久化数据存储目录。


**语法:** `SecDataDir PATH`


**区分大小写:** 是



此目录用于存储持久化数据，如 IP 信誉记录、会话数据、用户跟踪数据等。WAF 使用该目录保存需要跨请求持久化的集合数据（通过 initcol 动作初始化）。该目录需对运行 WAF 进程的用户具有可写权限。


**示例:**


```
SecDataDir /var/lib/wge/
```




### SecUnicodeMapFile


> **描述:** 配置 Unicode 映射文件路径和代码页。


**语法:** `SecUnicodeMapFile PATH CODEPAGE`


**区分大小写:** 是



该指令定义用于 urlDecodeUni 转换函数的 Unicode 映射文件路径。代码页参数指定目标字符编码（如 20127 表示 US-ASCII）。当处理使用非 ASCII 编码的 URL 时，此配置确保正确的字符转换。


**示例:**


```
SecUnicodeMapFile unicode.mapping 20127
```




### SecDebugLog


> **描述:** 配置调试日志文件路径。


**语法:** `SecDebugLog PATH`


**区分大小写:** 是



调试日志用于记录 WAF 规则引擎的详细运行信息，对于规则调试和问题排查非常有用。调试日志的详细程度由 SecDebugLogLevel 控制。建议在生产环境中仅在排查问题时启用调试日志。


**示例:**


```
SecDebugLog /var/log/wge_debug.log
```






### SecDebugLogLevel


> **描述:** 配置调试日志的详细级别。


**语法:** `SecDebugLogLevel LEVEL`


**默认值:** 0


**可选值:**


- **0:** 无日志
- **1:** 仅错误
- **2:** 警告
- **3:** 通知
- **4:** 详细信息
- **5:** 调试信息
- **9:** 最详细

**区分大小写:** 是



在生产环境中始终启用调试日志通常不被建议。即使在排查特定问题时，也需注意使用 4 级或更高级别的日志会显著影响性能。
调试日志级别的取值范围：


**示例:**


```
SecDebugLogLevel 3
```




### SecStatusEngine


> **描述:** 配置状态引擎，用于收集和报告运行时统计信息。


**语法:** `SecStatusEngine On|Off`


**默认值:** Off


**区分大小写:** 是



启用后，状态引擎会收集 WAF 运行时的统计数据，包括处理的请求数、规则匹配次数、阻断次数等。这些信息有助于监控 WAF 性能和安全态势。


**示例:**


```
SecStatusEngine On
```




### SecCookieFormat


> **描述:** 配置 Cookie 解析格式版本。


**语法:** `SecCookieFormat VERSION`


**默认值:** 0


**可选值:**


- **0:** 使用 Netscape Cookie 格式 (版本 0)
- **1:** 使用 RFC 2109 Cookie 格式 (版本 1)

**区分大小写:** 是



**示例:**


```
SecCookieFormat 0
```




### SecCollectionTimeout


> **描述:** 配置持久化集合的过期时间 (秒)。


**语法:** `SecCollectionTimeout SECONDS`


**默认值:** 3600


**区分大小写:** 是



持久化集合（如 IP、SESSION、USER 等）用于跨请求跟踪数据。此指令定义集合记录的存活时间。超过此时间后，集合记录将被自动清理。合理设置此值可平衡安全跟踪需求和存储空间使用。


**示例:**


```
SecCollectionTimeout 3600
```




### SecRequestBodyInMemoryLimit


> **描述:** 配置请求体在内存中缓冲的最大大小，超过此限制将写入临时文件。


**语法:** `SecRequestBodyInMemoryLimit LIMIT`


**默认值:** 131072 (128KB)


**区分大小写:** 是



此指令控制 WAF 将多少请求体数据保留在内存中。当请求体大小超过此限制时，超出部分将写入 SecTmpDir 指定的临时目录。适当设置此值可在内存使用和 I/O 性能之间取得平衡。


**示例:**


```
SecRequestBodyInMemoryLimit 131072
```




### SecResponseBodyMimeTypesClear


> **描述:** 清除所有响应体 MIME 类型配置。


**语法:** `SecResponseBodyMimeTypesClear`


**区分大小写:** 是



此指令清除先前通过 SecResponseBodyMimeType 配置的所有 MIME 类型，允许从零开始重新定义需要缓冲的响应类型。通常用于覆盖继承的配置。


**示例:**


```
SecResponseBodyMimeTypesClear
SecResponseBodyMimeType text/html text/plain
```




### SecResponseBodyLimitAction


> **描述:** 配置当响应体超过 SecResponseBodyLimit 限制时的处理方式。


**语法:** `SecResponseBodyLimitAction Reject|ProcessPartial`


**默认值:** Reject


**可选值:**


- **Reject:** 拒绝超限响应
- **ProcessPartial:** 仅检查符合限制的部分响应体，放行其余内容

**区分大小写:** 是



某些站点会生成超长响应，难以设定合理限制。通过设置为 ProcessPartial，可在不大幅提高内存限制的情况下处理超长响应，但存在部分内容未经检查的风险。


**示例:**


```
SecResponseBodyLimitAction ProcessPartial
```




### SecXmlExternalEntity


> **描述:** 配置是否允许 XML 外部实体处理。


**语法:** `SecXmlExternalEntity On|Off`


**默认值:** Off


**区分大小写:** 是




XXE 攻击可能导致敏感文件泄露、服务器端请求伪造 (SSRF)、拒绝服务等安全问题。


**示例:**


```
SecXmlExternalEntity Off
```




### SecGeoLookupDb


> **描述:** 配置地理位置数据库文件路径，用于 @geoLookup 操作符。


**语法:** `SecGeoLookupDb PATH`


**区分大小写:** 是



WGE 使用 MaxMind 的 GeoIP2 格式数据库（.mmdb 文件）进行 IP 地址地理位置查询。配置此指令后，可在规则中使用 @geoLookup 操作符获取客户端 IP 的地理位置信息，实现基于地理位置的访问控制。


**示例:**


```
SecGeoLookupDb /usr/share/GeoIP/GeoLite2-Country.mmdb
```




### SecHttpBlKey


> **描述:** 配置 HTTP 黑名单 (Project Honeypot) API 密钥。


**语法:** `SecHttpBlKey API_KEY`


若 @rbl 操作符使用 dnsbl.httpbl.org RBL（http://www.projecthoneypot.org/httpbl_api.php），则必须提供 API 密钥。该密钥绑定至个人用户账户，并将包含在 RBL DNS 请求中。


**示例:**


```
SecHttpBlKey abcdefghijkl
```




### SecRemoteRules


> **描述:** 从远程 HTTPS 服务器加载规则配置。


**语法:** `SecRemoteRules KEY URL`


**区分大小写:** 是



此指令允许从远程服务器动态加载规则，便于集中管理和更新安全策略。KEY 参数用于身份验证，目标服务器可据此为不同客户端提供差异化规则集。



**示例:**


```
SecRemoteRules some-key https://example.com/rules.conf
```




### SecRemoteRulesFailAction


> **描述:** 配置远程规则加载失败时的处理方式。


**语法:** `SecRemoteRulesFailAction Abort|Warn`


**默认值:** Abort


**区分大小写:** 是



**可选值:**


- **Abort:** 中止启动，确保 WAF 不会在缺少规则的情况下运行
- **Warn:** 仅记录警告并继续启动，WAF 将使用本地规则继续运行

默认行为是在下载指定 URL 时出现问题即终止操作，这确保了安全策略的完整性。


**示例:**


```
SecRemoteRulesFailAction Warn
```




### SecWebAppId


> **描述:** 配置 Web 应用程序标识符,用于区分不同的应用程序。


**语法:** `SecWebAppId NAME`


**区分大小写:** 是



应用程序命名空间用于避免同一服务器部署多个应用时会话 ID 与用户 ID 发生冲突。若未使用该功能，会话 ID 可能发生冲突。
在上述两个示例配置中，SecWebAppId 与 nginx 服务器块协同使用。配置值将存储于 WEBAPPID 变量中。


**示例:**


```
SecWebAppId "MyWebApp"
```




### SecComponentSignature


> **描述:** 添加组件签名信息到日志中。


**语法:** `SecComponentSignature "SIGNATURE"`


**区分大小写:** 是



此指令用于标识重要规则集的存在。完整签名将记录于事务审计日志中。


**示例:**


```
SecComponentSignature "OWASP_CRS/3.3.2"
```



## 文件上传配置




### SecTmpSaveUploadedFiles


> **描述:** 配置是否保存上传的文件到临时目录。


**语法:** `SecTmpSaveUploadedFiles On|Off`


**默认值:** Off


**区分大小写:** 是



启用后，通过 multipart/form-data 上传的文件将保存到 SecTmpDir 指定的临时目录。这允许规则使用 @inspectFile 操作符对上传文件进行深度检查（如病毒扫描）。


**示例:**


```
SecTmpSaveUploadedFiles On
```




### SecUploadDir


> **描述:** 配置文件上传的存储目录。


**语法:** `SecUploadDir PATH`


**区分大小写:** 是



该目录必须与 SecTmpDir 定义的临时目录位于同一文件系统。此指令需配合 SecUploadKeepFiles 使用。


**示例:**


```
SecUploadDir /var/lib/wge/upload/
```




### SecUploadFileLimit


> **描述:** 配置单次请求允许上传的最大文件数量。


**语法:** `SecUploadFileLimit LIMIT`


**默认值:** 100


**区分大小写:** 是



**示例:**


```
SecUploadFileLimit 10
```




### SecUploadFileMode


> **描述:** 配置上传文件的权限模式 (八进制)。


**语法:** `SecUploadFileMode MODE`


**默认值:** 0600


**区分大小写:** 是



此功能在不支持八进制文件模式的操作系统上不可用。默认模式（0600）仅授予写入文件的账户读写权限。若需其他账户访问，则可能需要此指令。但请谨慎使用，避免将潜在敏感数据暴露给未经授权的用户。


**示例:**


```
SecUploadFileMode 0640
```




### SecUploadKeepFiles


> **描述:** 配置是否在请求处理完成后保留上传的文件。


**语法:** `SecUploadKeepFiles On|Off|RelevantOnly`


**默认值:** Off


**可选值:**


- **On:** 保留所有上传文件
- **Off:** 不保留上传文件
- **RelevantOnly:** 仅保留触发规则的请求中的上传文件

**区分大小写:** 是



此指令要求存储目录已通过 SecUploadDir 定义。


**示例:**


```
SecUploadKeepFiles RelevantOnly
```



## 规则指令




### SecRule


> **描述:** 定义一个安全规则。


**语法:** `SecRule VARIABLES "OPERATOR" "ACTIONS"`


**版本:** 1.0



SecRule 是 SecLang 中最重要的指令,用于定义检测和响应安全威胁的规则。一个规则由三部分组成:


                - **变量 (VARIABLES):** 指定要检查的数据源
- **操作符 (OPERATOR):** 定义匹配条件
- **动作 (ACTIONS):** 定义规则匹配后的行为



**基本示例:**


```apache
SecRule ARGS "@contains " \
    "id:1001,phase:2,deny,status:403,msg:'XSS Attack Detected'"
```



**多个变量示例:**


```apache
SecRule ARGS|REQUEST_HEADERS "@rx (?i:eval\s*\()" \
    "id:1002,phase:2,deny,status:403,msg:'Code Injection Detected'"
```



**使用转换示例:**


**区分大小写:** 是


```apache
SecRule ARGS "@rx select.*from" \
    "id:1003,phase:2,t:lowercase,t:removeWhitespace,deny,msg:'SQL Injection'"
```







### SecAction


> **描述:** 无条件执行指定的动作。


**语法:** `SecAction "ACTIONS"`


**版本:** 1.0



SecAction 用于无条件执行一组动作,常用于设置变量、初始化集合等。



**区分大小写:** 是



SecAction 无条件处理指定的动作列表，其参数语法与 SecRule 的第三参数完全一致。该指令常用于设置变量及通过 initcol 操作初始化持久集合。由于没有变量和操作符参数，SecAction 的动作将在每次处理时无条件执行。


**示例:**


```apache
SecAction "id:9001,phase:1,nolog,pass,setvar:tx.blocking_paranoia_level=2"
```




### SecDefaultAction


> **描述:** 定义特定阶段的默认动作列表。


**语法:** `SecDefaultAction "ACTIONS"`


**默认值:** phase:2,log,auditlog,pass


**版本:** 1.0



**区分大小写:** 是



在同一配置上下文中，所有位于先前 SecDefaultAction 指令之后的规则将继承其设置，除非规则自身指定了更具体的动作。每个 SecDefaultAction 指令必须指定破坏性动作和处理阶段，且不能包含元数据动作。需注意 SecDefaultAction 不会跨配置上下文继承。


**示例:**


```apache
SecDefaultAction "phase:1,log,auditlog,pass"
SecDefaultAction "phase:2,log,auditlog,deny,status:403"
```




### SecRuleRemoveById


> **描述:** 根据 ID 移除规则。


**语法:** `SecRuleRemoveById ID [ID_RANGE] ...`


**区分大小写:** 是



该指令支持多个参数，每个参数可为规则 ID 或范围。


**示例:**


```apache
# 移除单个规则
SecRuleRemoveById 1001

# 移除多个规则
SecRuleRemoveById 1001 1002 1003

# 移除范围内的规则
SecRuleRemoveById 1001-1100
```




### SecRuleRemoveByMsg


> **描述:** 根据消息内容移除规则。


**语法:** `SecRuleRemoveByMsg "MESSAGE"`


**区分大小写:** 是



通常使用 SecRuleRemoveById 删除规则，但本指令支持通过匹配规则的 msg 操作进行删除。匹配采用区分大小写的字符串精确匹配。


**示例:**


```apache
SecRuleRemoveByMsg "SQL Injection Attack"
```




### SecRuleRemoveByTag


> **描述:** 根据标签移除规则。


**语法:** `SecRuleRemoveByTag "TAG"`


**区分大小写:** 是



通常使用 SecRuleRemoveById 删除规则，但有时通过 SecRuleRemoveByTag 禁用整组规则更为便捷。匹配采用区分大小写的字符串相等性。


**示例:**


```apache
SecRuleRemoveByTag "attack-sqli"
```




### SecRuleUpdateActionById


> **描述:** 根据 ID 更新规则的动作。


**语法:** `SecRuleUpdateActionById ID "ACTIONS"`


**区分大小写:** 是



该指令将用第二个参数提供的动作覆盖指定规则的动作列表。存在两项限制：无法用于修改规则 ID 或阶段；仅覆盖只能出现一次的动作。允许在列表中多次出现的动作将追加至列表末尾。
前例中最终生效的规则如下：
添加 t:none 将覆盖先前指定的任何转换函数（如示例中的 t:lowercase）。


**示例:**


```apache
SecRuleUpdateActionById 1001 "pass,log"
```




### SecRuleUpdateTargetById


> **描述:** 根据 ID 更新规则的检查目标。


**语法:** `SecRuleUpdateTargetById ID VARIABLES`


**区分大小写:** 是



该指令将使用第二个参数提供的目标，向指定规则的当前目标列表追加（或替换）变量。
此功能适用于实现例外情况，即需要外部更新目标列表以排除对特定变量的检查。
前例中生成的有效规则将目标附加至变量列表末尾如下：
请注意，在目标规范中也可使用正则表达式：
请注意，若要在正则表达式中使用分组，必须使用单引号将其括起来。
您也可将目标替换为更适合环境的选项。例如，若需检查 REQUEST_URI 而非 REQUEST_FILENAME，可采用以下方式：
前例中生成的有效规则将变量列表开头的目标替换为：


**示例:**


```apache
SecRuleUpdateTargetById 1001 !ARGS:username
```




### SecRuleUpdateTargetByMsg


> **描述:** 根据消息内容更新规则的检查目标。


**语法:** `SecRuleUpdateTargetByMsg "MESSAGE" VARIABLES`


**区分大小写:** 是



该指令将使用第二个参数提供的目标，向指定规则的当前目标列表追加（或替换）变量。
此方法适用于实现例外情况，即需要外部更新目标列表以排除对特定变量的检测。
前例中生成的有效规则将目标附加至变量列表末尾如下：
您也可以完全替换目标列表，使其更适合您的环境。例如，若需检查 REQUEST_URI 而非 REQUEST_FILENAME，可采用以下方式：
前例中生成的有效规则将按如下方式将目标附加到变量列表末尾：


**示例:**


```apache
SecRuleUpdateTargetByMsg "SQL Injection" !ARGS:id
```




### SecRuleUpdateTargetByTag


> **描述:** 根据标签更新规则的检查目标。


**语法:** `SecRuleUpdateTargetByTag "TAG" VARIABLES`


**区分大小写:** 是



该指令将使用第二个参数提供的目标，向指定规则的当前目标列表追加（或替换）变量。
此方法适用于实现例外情况，即需要外部更新目标列表以排除对特定变量的检测。
前例中生成的有效规则将按如下方式将目标附加到变量列表末尾：
您也可以完全替换目标列表，使其更适合您的环境。例如，若您希望检查 REQUEST_URI 而非 REQUEST_FILENAME，可采用以下方式：
前例中生成的有效规则将按如下方式将目标附加至变量列表末尾：


**示例:**


```apache
SecRuleUpdateTargetByTag "attack-sqli" !ARGS:search
```




### SecRuleScript


> **描述:** 使用 Lua 脚本定义规则逻辑。


**语法:** `SecRuleScript PATH "ACTIONS"`


**区分大小写:** 是



此指令创建特殊规则，通过执行 Lua 脚本决定是否匹配。与 SecRule 的主要区别在于不存在目标和运算符。脚本可获取 WAF 上下文中的任意变量，并使用 Lua 运算符进行测试。所有 Lua 脚本在配置加载时编译并缓存至内存，若需重新加载脚本，必须重新加载整个配置。


**示例:**


```apache
SecRuleScript /etc/wge/scripts/custom_check.lua "id:5001,phase:2,deny,msg:'Script check failed'"
```