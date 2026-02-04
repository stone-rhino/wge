+++
title = "@rxGlobal"
weight = 12
+++

本章详细介绍 SecLang++ 中的操作符。

## 操作符


            
操作符定义了如何匹配变量的值。WGE 支持多种操作符,可以进行字符串匹配、正则表达式、数值比较等。


            
### 操作符语法

            
操作符使用 `@` 前缀,后跟操作符名称和可选参数:

            
```
"@operatorName parameter"
```

            
操作符可以使用 `!` 前缀进行取反:

            
```
"!@operatorName parameter"
```


            
### 字符串匹配操作符


| 名称 | 描述 |
|---|---|
| [`@rx`](#rx) | 正则表达式匹配 |
| [`@contains`](#contains) | 包含指定字符串 |
| [`@beginsWith`](#beginswith) | 以指定字符串开头 |
| [`@endsWith`](#endswith) | 以指定字符串结尾 |
| [`@within`](#within) | 值在指定字符串中 |
| [`@containsWord`](#containsword) | 包含指定单词 |
| [`@streq`](#streq) | 字符串相等 |
| [`@strmatch`](#strmatch) | 通配符匹配 |
| [`@pm`](#pm) | 并行匹配 (多个关键字) |
| [`@pmf`](#pmf) | 并行匹配 (从文件加载,快速版本) |
| [`@pmFromFile`](#pmfromfile) | 从文件加载并行匹配关键字 |
| [`@rxGlobal`](#rxglobal) | 全局正则表达式匹配 (匹配所有出现) |


#### @rx


> **描述:** 使用 Perl 兼容正则表达式 (PCRE) 对变量值进行匹配


**语法:** `"@rx pattern"`


@rx 是 SecLang 中最强大和最常用的操作符。它使用 PCRE 语法，支持所有标准正则表达式特性。如果未指定操作符，@rx 将作为默认操作符使用。


**示例:**


```apache
# 检测 SQL 注入关键字
SecRule ARGS "@rx (?i:select|union|insert|update|delete|drop)" \
    "id:1001,phase:2,deny,msg:'SQL Injection detected'"

# 检测 XSS 攻击模式
SecRule ARGS "@rx ]*>.*?" \
    "id:1002,phase:2,deny,msg:'XSS Attack detected'"
```


**参数类型:** `string (正则表达式)`


#### @contains


> **描述:** 包含指定字符串


**语法:** `"@contains string"`


@contains 检查变量值是否包含指定的子字符串。匹配区分大小写。相比 @rx，@contains 在进行简单子字符串查找时性能更优，因为不涉及正则表达式引擎开销。


**示例:**


```apache
# 检测 Content-Type 是否包含 multipart
SecRule REQUEST_HEADERS:Content-Type "@contains multipart" \
    "id:1068,phase:1,pass,nolog,ctl:requestBodyProcessor=MULTIPART"

# 检测 URL 中是否包含 admin
SecRule REQUEST_URI "@contains admin" \
    "id:1069,phase:1,log,msg:'访问管理路径'"
```


**参数类型:** `string`


#### @beginsWith


> **描述:** 以指定字符串开头


**语法:** `"@beginsWith string"`


@beginsWith 检查变量值是否以指定的字符串开头。匹配区分大小写。相比使用 @rx "^prefix" 的正则表达式，@beginsWith 在进行简单前缀检查时性能更优，因为避免了正则引擎的开销。常用于 URL 路径检查、协议验证等场景。


**示例:**


```apache
# 检测管理后台访问
SecRule REQUEST_URI "@beginsWith /admin" \
    "id:1070,phase:1,deny,msg:'未授权访问管理路径'"

# 检测 API 路径并标记
SecRule REQUEST_URI "@beginsWith /api/v" \
    "id:1071,phase:1,pass,nolog,setvar:tx.is_api_request=1"

# 检测危险协议 (如 javascript:)
SecRule ARGS "@beginsWith javascript:" \
    "id:1072,phase:2,deny,msg:'检测到 JavaScript 协议注入'"

# 检测 Base64 编码数据
SecRule REQUEST_BODY "@beginsWith data:image" \
    "id:1073,phase:2,pass,nolog,setvar:tx.has_base64_image=1"
```


**参数类型:** `string`


**区分大小写:** 是


#### @endsWith


> **描述:** 以指定字符串结尾


**语法:** `"@endsWith string"`


@endsWith 检查变量值是否以指定的字符串结尾。匹配区分大小写。相比使用 @rx "suffix$" 的正则表达式，@endsWith 在进行简单后缀检查时性能更优。常用于文件扩展名检查、路径验证等场景。


**示例:**


```apache
# 检测危险文件扩展名访问
SecRule REQUEST_FILENAME "@endsWith .bak" \
    "id:1074,phase:1,deny,msg:'禁止访问备份文件'"

# 检测配置文件访问
SecRule REQUEST_URI "@endsWith .conf" \
    "id:1075,phase:1,deny,msg:'禁止访问配置文件'"

# 检测 PHP 文件上传
SecRule FILES_NAMES "@endsWith .php" \
    "id:1076,phase:2,deny,msg:'禁止上传 PHP 文件'"

# 检测隐藏文件 (Unix 风格)
SecRule REQUEST_FILENAME "@endsWith /.htaccess" \
    "id:1077,phase:1,deny,msg:'禁止访问 .htaccess 文件'"
```


**参数类型:** `string`


**区分大小写:** 是


#### @within


> **描述:** 值在指定字符串中


**语法:** `"@within string"`


@within 是 @contains 的反向操作：检查变量值是否作为子字符串存在于指定的参数字符串中。换句话说，它检查参数字符串是否包含变量值。匹配区分大小写。这在创建白名单检查时特别有用，可以验证值是否在允许的列表中。


**示例:**


```apache
# 验证 HTTP 方法是否在允许列表中
SecRule REQUEST_METHOD "!@within GET POST HEAD OPTIONS" \
    "id:1078,phase:1,deny,msg:'不允许的 HTTP 方法'"

# 验证文件扩展名是否在白名单中
SecRule FILES_COMBINED_SIZE "@gt 0" \
    "id:1079,phase:2,chain"
    SecRule FILES_NAMES "!@within .jpg .png .gif .pdf" \
        "deny,msg:'不允许的文件类型'"

# 验证 Content-Type 是否合法
SecRule REQUEST_HEADERS:Content-Type "!@within application/json application/xml text/plain" \
    "id:1080,phase:1,deny,msg:'不支持的 Content-Type'"
```


**参数类型:** `string`


**区分大小写:** 是


#### @containsWord


> **描述:** 包含指定单词


**语法:** `"@containsWord parameter"`


**示例:**


```apache
SecRule ARGS "@containsWord parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string`


**是否实现:** 暂无


#### @streq


> **描述:** 字符串相等


**语法:** `"@streq string"`


@streq 检查变量值是否与指定的字符串完全相等。匹配区分大小写。相比 @rx "^exact$" 的正则表达式，@streq 在进行精确字符串比较时性能更优。如需不区分大小写的比较，可以先应用 lowercase 转换函数。常用于精确值验证、标志检查等场景。


**示例:**


```apache
# 精确匹配 HTTP 方法
SecRule REQUEST_METHOD "@streq POST" \
    "id:1081,phase:1,pass,nolog,setvar:tx.is_post=1"

# 验证特定 Header 值
SecRule REQUEST_HEADERS:X-Requested-With "@streq XMLHttpRequest" \
    "id:1082,phase:1,pass,nolog,setvar:tx.is_ajax=1"

# 不区分大小写的比较（使用 lowercase 转换）
SecRule REQUEST_HEADERS:Accept "!@streq application/json" \
    "id:1083,phase:1,t:lowercase,deny,msg:'仅接受 JSON 请求'"

# 检测特定用户代理
SecRule REQUEST_HEADERS:User-Agent "@streq curl/7.68.0" \
    "id:1084,phase:1,log,pass,msg:'检测到 curl 客户端'"
```


**参数类型:** `string`


**区分大小写:** 是


#### @strmatch


> **描述:** 通配符匹配


**语法:** `"@strmatch parameter"`


**示例:**


```apache
SecRule ARGS "@strmatch parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (通配符)`


**是否实现:** 暂无


#### @pm


> **描述:** 使用 Aho-Corasick 算法进行高效的多模式并行匹配


**语法:** `"@pm keyword1 keyword2 keyword3 ..."`


@pm 操作符可以同时搜索多个关键字，使用空格分隔。相比多个 @rx 规则，@pm 在匹配大量关键字时性能更优。匹配不区分大小写。


**示例:**


```apache
# 检测常见 SQL 注入关键字
SecRule ARGS "@pm select union insert update delete" \
    "id:1001,phase:2,deny,msg:'SQL keyword detected'"

# 检测敏感文件访问
SecRule REQUEST_URI "@pm /etc/passwd /etc/shadow .htaccess" \
    "id:1002,phase:1,deny,msg:'Sensitive file access'"
```


**参数类型:** `string (空格分隔的关键字列表)`


#### @pmf


> **描述:** 从文件加载关键字进行并行匹配（@pmFromFile 的别名）


**语法:** `"@pmf /path/to/keywords.txt"`


与 @pm 功能相同，但从外部文件加载关键字列表。文件中每行一个关键字，以 # 开头的行将作为注释忽略。适用于管理大量关键字的场景。


**示例:**


```apache
# 从文件加载 SQL 注入关键字
SecRule ARGS "@pmf /etc/wge/sql-keywords.txt" \
    "id:1001,phase:2,deny,msg:'SQL keyword detected'"
```


**参数类型:** `string (文件路径)`


#### @pmFromFile


> **描述:** 从文件加载关键字进行并行匹配


**语法:** `"@pmFromFile /path/to/keywords.txt"`


与 @pmf 功能完全相同。文件中每行一个关键字，以 # 开头的行将作为注释忽略。支持相对路径（相对于配置文件目录）和绝对路径。


**示例:**


```apache
# 从文件加载恶意 User-Agent 列表
SecRule REQUEST_HEADERS:User-Agent "@pmFromFile /etc/wge/bad-ua.txt" \
    "id:1001,phase:1,deny,msg:'Malicious User-Agent'"
```


**参数类型:** `string (文件路径)`


#### @rxGlobal


> **描述:** 全局正则表达式匹配 (匹配所有出现)


**语法:** `"@rxGlobal parameter"`


**示例:**


```apache
SecRule ARGS "@rxGlobal ^admin" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (正则)`


**是否实现:** 暂无


### 数值比较操作符


| 名称 | 描述 |
|---|---|
| [`@eq`](#eq) | 数值相等 |
| [`@gt`](#gt) | 大于 |
| [`@lt`](#lt) | 小于 |
| [`@ge`](#ge) | 大于等于 |
| [`@le`](#le) | 小于等于 |


#### @eq


> **描述:** 数值相等


**语法:** `"@eq number"`


@eq 对变量值进行数值相等比较。变量值会先被转换为整数再进行比较，非数值字符串会被转换为 0。常用于检查状态码、计数值等精确数值匹配场景。支持使用宏扩展动态指定比较值。


**示例:**


```apache
# 检测 404 响应状态
SecRule RESPONSE_STATUS "@eq 404" \
    "id:1085,phase:3,pass,log,msg:'页面未找到'"

# 检测空请求体
SecRule REQUEST_HEADERS:Content-Length "@eq 0" \
    "id:1086,phase:1,pass,nolog,setvar:tx.empty_body=1"

# 检测异常评分是否达到阈值
SecRule TX:anomaly_score "@eq %{TX.threshold}" \
    "id:1087,phase:2,deny,msg:'异常评分达到阈值'"
```


**参数类型:** `int`


**区分大小写:** 是


#### @gt


> **描述:** 大于


**语法:** `"@gt number"`


@gt 检查变量值（转换为整数后）是否大于指定的数值。常用于检测超限情况，如请求大小超限、频率超限、评分超标等场景。是实现限流和阈值检测的核心操作符之一。


**示例:**


```apache
# 检测请求体过大 (超过 10MB)
SecRule REQUEST_HEADERS:Content-Length "@gt 10485760" \
    "id:1088,phase:1,deny,msg:'请求体超过大小限制'"

# 检测异常评分超标
SecRule TX:anomaly_score "@gt 10" \
    "id:1089,phase:2,deny,msg:'异常评分超过阈值: %{TX.anomaly_score}'"

# 检测 IP 请求频率超限
SecRule IP:request_count "@gt 100" \
    "id:1090,phase:1,deny,msg:'IP 请求频率超限'"

# 检测参数数量异常
SecRule &ARGS "@gt 100" \
    "id:1091,phase:2,deny,msg:'参数数量异常'"
```


**参数类型:** `int`


**区分大小写:** 是


#### @lt


> **描述:** 小于


**语法:** `"@lt number"`


@lt 检查变量值（转换为整数后）是否小于指定的数值。常用于检测数值不足的情况，如响应状态码范围检测、最小长度验证等场景。与 @gt 配合使用可实现范围检测。


**示例:**


```apache
# 检测成功响应 (状态码 


#### @ge


> **描述:** 大于等于


**语法:** `"@ge number"`


@ge 检查变量值（转换为整数后）是否大于等于指定的数值。常用于阈值检测场景，特别是在异常评分模式中判断累计评分是否达到阻断阈值。与 @le 配合可实现闭区间范围检测。


**示例:**


```apache
# 异常评分达到阻断阈值
SecRule TX:anomaly_score "@ge 5" \
    "id:1095,phase:2,deny,msg:'异常评分达到阈值: %{TX.anomaly_score}'"

# 检测大文件上传
SecRule FILES_COMBINED_SIZE "@ge 5242880" \
    "id:1096,phase:2,deny,msg:'上传文件总大小超过 5MB'"

# 检测暴力破解 (失败次数 >= 5)
SecRule IP:failed_login "@ge 5" \
    "id:1097,phase:2,deny,msg:'疑似暴力破解攻击'"
```


**参数类型:** `int`


**区分大小写:** 是


#### @le


> **描述:** 小于等于


**语法:** `"@le number"`


@le 检查变量值（转换为整数后）是否小于等于指定的数值。常用于上限检测，如响应状态码范围检测、时间窗口内的请求计数限制等。与 @ge 配合可实现闭区间范围检测。


**示例:**


```apache
# 检测正常响应 (状态码 


### IP 地址操作符


| 名称 | 描述 |
|---|---|
| [`@ipMatch`](#ipmatch) | IP 地址/网段匹配 |
| [`@ipMatchF`](#ipmatchf) | IP 地址匹配 (快速版本) |
| [`@ipMatchFromFile`](#ipmatchfromfile) | 从文件加载 IP 地址列表匹配 |


#### @ipMatch


> **描述:** IP 地址/网段匹配


**语法:** `"@ipMatch ip1,ip2,cidr1,cidr2..."`


@ipMatch 检查变量值是否匹配指定的 IP 地址或 CIDR 网段。支持 IPv4 和 IPv6 地址。多个地址/网段用逗号分隔。常用于实现 IP 白名单、黑名单、地域限制等访问控制场景。匹配效率高于使用 @rx 进行 IP 匹配。


**示例:**


```apache
# IP 白名单 - 允许特定 IP 跳过检测
SecRule REMOTE_ADDR "@ipMatch 192.168.1.0/24,10.0.0.0/8" \
    "id:1103,phase:1,pass,nolog,ctl:ruleEngine=Off"

# IP 黑名单 - 阻止已知恶意 IP
SecRule REMOTE_ADDR "@ipMatch 1.2.3.4,5.6.7.8" \
    "id:1104,phase:1,deny,msg:'已列入黑名单的 IP 地址'"

# 限制管理后台访问来源
SecRule REQUEST_URI "@beginsWith /admin" \
    "id:1105,phase:1,chain"
    SecRule REMOTE_ADDR "!@ipMatch 192.168.1.0/24" \
        "deny,msg:'禁止从外部网络访问管理后台'"

# 检测内网 IP（可能的 SSRF 攻击）
SecRule ARGS "@ipMatch 127.0.0.0/8,10.0.0.0/8,172.16.0.0/12,192.168.0.0/16" \
    "id:1106,phase:2,deny,msg:'检测到内网 IP 访问，疑似 SSRF'"
```


**参数类型:** `string`


**区分大小写:** 是


#### @ipMatchF


> **描述:** 从文件加载 IP 地址列表进行匹配 (@ipMatchFromFile 的别名)


**语法:** `"@ipMatchF /path/to/ip-list.txt"`


@ipMatchF 是 @ipMatchFromFile 的简写形式，从外部文件加载 IP 地址/网段列表进行匹配。文件中每行一个 IP 地址或 CIDR 网段，以 # 开头的行作为注释忽略。适用于管理大量 IP 地址的场景，如动态更新的黑名单、CDN 节点列表等。


**示例:**


```apache
# 从文件加载 IP 黑名单
SecRule REMOTE_ADDR "@ipMatchF /etc/wge/ip-blacklist.txt" \
    "id:1107,phase:1,deny,msg:'IP 在黑名单中'"

# 从文件加载可信代理 IP 列表
SecRule REMOTE_ADDR "@ipMatchF /etc/wge/trusted-proxies.txt" \
    "id:1108,phase:1,pass,nolog,setvar:tx.is_trusted_proxy=1"
```


**文件格式示例 (/etc/wge/ip-blacklist.txt):**


```
# 恶意 IP 黑名单
# 更新日期: 2024-01-01
1.2.3.4
5.6.7.8
10.0.0.0/8
192.168.100.0/24
```


**参数类型:** `string (文件路径)`


**区分大小写:** 是


#### @ipMatchFromFile


> **描述:** 从文件加载 IP 地址列表匹配


**语法:** `"@ipMatchFromFile parameter"`


**示例:**


```apache
SecRule ARGS "@ipMatchFromFile 192.168.1.1" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (文件路径)`


**是否实现:** 暂无


### 验证操作符


| 名称 | 描述 |
|---|---|
| [`@validateByteRange`](#validatebyterange) | 验证字节范围 |
| [`@validateUrlEncoding`](#validateurlencoding) | 验证 URL 编码格式 |
| [`@validateUtf8Encoding`](#validateutf8encoding) | 验证 UTF-8 编码格式 |
| [`@validateDtd`](#validatedtd) | 根据 DTD 验证 XML |
| [`@validateSchema`](#validateschema) | 根据 Schema 验证 XML |
| [`@verifyCc`](#verifycc) | 验证信用卡号 |
| [`@verifyCpf`](#verifycpf) | 验证巴西 CPF 号码 |
| [`@verifySsn`](#verifyssn) | 验证美国社会安全号 |


#### @validateByteRange


> **描述:** 验证字节范围


**语法:** `"@validateByteRange range1,range2-range3,..."`


@validateByteRange 检查变量值中的每个字节是否在指定的范围内。支持单个值（如 10）和范围（如 32-126）的组合，用逗号分隔。如果发现范围外的字节则匹配成功。此操作符最常用于检测 NUL 字节（0x00）的存在——这类字节虽无正当用途，却常被用作规避检测的技术。也可用于检测非打印字符和二进制数据。


**示例:**


```apache
# 检测 NUL 字节 (常见的规避技术)
SecRule ARGS "@validateByteRange 1-255" \
    "id:1109,phase:2,deny,msg:'检测到 NUL 字节注入'"

# 仅允许可打印 ASCII 字符 (32-126) 和常见控制字符
SecRule ARGS "@validateByteRange 9,10,13,32-126" \
    "id:1110,phase:2,deny,msg:'检测到非法字符'"

# 检测参数中的二进制数据
SecRule ARGS:data "!@validateByteRange 32-126" \
    "id:1111,phase:2,pass,log,msg:'参数包含非打印字符'"

# 在 URL 中检测非法字符
SecRule REQUEST_URI "@validateByteRange 1-255" \
    "id:1112,phase:1,deny,msg:'URL 中检测到 NUL 字节'"
```


**参数类型:** `string`


**区分大小写:** 是


#### @validateUrlEncoding


> **描述:** 验证 URL 编码格式


**语法:** `"@validateUrlEncoding"`


@validateUrlEncoding 检查变量值中的 URL 编码（百分号编码）是否有效。如果发现无效的 URL 编码则匹配成功。此操作符用于检测利用无效 URL 编码进行规避检测的攻击，例如 %ZZ 或 %1 这样的格式错误编码。不需要参数。


**示例:**


```apache
# 检测无效的 URL 编码
SecRule ARGS "@validateUrlEncoding" \
    "id:1113,phase:2,deny,msg:'检测到无效的 URL 编码'"

# 检测 URI 中的无效编码
SecRule REQUEST_URI "@validateUrlEncoding" \
    "id:1114,phase:1,deny,msg:'URI 中存在无效的 URL 编码'"

# 检测查询字符串中的编码问题
SecRule QUERY_STRING "@validateUrlEncoding" \
    "id:1115,phase:1,deny,msg:'查询字符串中存在无效编码'"
```


**参数类型:** `无`


**区分大小写:** 是


#### @validateUtf8Encoding


> **描述:** 验证 UTF-8 编码格式


**语法:** `"@validateUtf8Encoding parameter"`


**示例:**


```apache
SecRule ARGS "@validateUtf8Encoding parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `无`


**是否实现:** 暂无


#### @validateDtd


> **描述:** 根据 DTD 验证 XML


**语法:** `"@validateDtd parameter"`


**示例:**


```apache
SecRule ARGS "@validateDtd parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string`


**是否实现:** 暂无


#### @validateSchema


> **描述:** 根据 Schema 验证 XML


**语法:** `"@validateSchema parameter"`


**示例:**


```apache
SecRule ARGS "@validateSchema parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string`


**是否实现:** 暂无


#### @verifyCc


> **描述:** 验证信用卡号


**语法:** `"@verifyCc parameter"`


**示例:**


```apache
SecRule ARGS "@verifyCc parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (正则)`


**是否实现:** 暂无


#### @verifyCpf


> **描述:** 验证巴西 CPF 号码


**语法:** `"@verifyCpf parameter"`


**示例:**


```apache
SecRule ARGS "@verifyCpf parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (正则)`


**是否实现:** 暂无


#### @verifySsn


> **描述:** 验证美国社会安全号


**语法:** `"@verifySsn parameter"`


**示例:**


```apache
SecRule ARGS "@verifySsn parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (正则)`


**是否实现:** 暂无


### 检测操作符


| 名称 | 描述 |
|---|---|
| [`@detectSQLi`](#detectsqli) | SQL 注入检测 |
| [`@detectXSS`](#detectxss) | XSS 攻击检测 |


#### @detectSQLi


> **描述:** 使用 libinjection 库检测 SQL 注入攻击


**语法:** `"@detectSQLi"`


@detectSQLi 使用 libinjection 的 SQL 注入检测引擎，通过词法分析和指纹匹配来识别 SQL 注入模式。相比基于正则表达式的检测，libinjection 具有更低的误报率和更高的检测准确性。此操作符不需要参数。


**示例:**


```apache
# 检测所有参数中的 SQL 注入
SecRule ARGS "@detectSQLi" \
    "id:1001,phase:2,deny,msg:'SQL Injection Attack Detected'"

# 结合变量计数检测
SecRule ARGS|REQUEST_HEADERS "@detectSQLi" \
    "id:1002,phase:2,deny,severity:CRITICAL,\
    msg:'SQL Injection detected in %{MATCHED_VAR_NAME}'"
```


**参数类型:** `无`


#### @detectXSS


> **描述:** 使用 libinjection 库检测跨站脚本 (XSS) 攻击


**语法:** `"@detectXSS"`


@detectXSS 使用 libinjection 的 XSS 检测引擎，通过识别 HTML 和 JavaScript 注入模式来检测 XSS 攻击。它能够检测各种类型的 XSS，包括反射型、存储型和 DOM 型 XSS。此操作符不需要参数。


**示例:**


```apache
# 检测参数中的 XSS 攻击
SecRule ARGS "@detectXSS" \
    "id:1001,phase:2,deny,msg:'XSS Attack Detected'"

# 检测请求体中的 XSS
SecRule REQUEST_BODY "@detectXSS" \
    "id:1002,phase:2,deny,severity:CRITICAL,\
    msg:'XSS Attack in request body'"
```


**参数类型:** `无`


### 特殊操作符


| 名称 | 描述 |
|---|---|
| [`@unconditionalMatch`](#unconditionalmatch) | 无条件匹配 (总是返回 true) |
| [`@noMatch`](#nomatch) | 不匹配 (总是返回 false) |
| [`@inspectFile`](#inspectfile) | 调用外部脚本检查文件 |
| [`@fuzzyHash`](#fuzzyhash) | 模糊哈希匹配 |
| [`@geoLookup`](#geolookup) | 地理位置查询 |
| [`@rbl`](#rbl) | 实时黑名单查询 |
| [`@rsub`](#rsub) | 正则表达式替换 |


#### @unconditionalMatch


> **描述:** 无条件匹配 (总是返回 true)


**语法:** `"@unconditionalMatch"`


@unconditionalMatch 无条件返回匹配成功（true）。不检查任何变量值，始终触发规则动作。常用于创建无条件执行的规则，如初始化变量、设置默认值、记录所有请求等场景。通常与 SecAction 配合使用效果相同，但在需要指定变量的规则链中很有用。


**示例:**


```apache
# 初始化事务变量
SecRule REQUEST_URI "@unconditionalMatch" \
    "id:1116,phase:1,pass,nolog,setvar:tx.anomaly_score=0"

# 记录所有请求
SecRule REMOTE_ADDR "@unconditionalMatch" \
    "id:1117,phase:1,pass,log,msg:'请求来自: %{REMOTE_ADDR}'"

# 在规则链中使用
SecRule REQUEST_METHOD "@streq POST" \
    "id:1118,phase:1,chain,pass"
    SecRule REQUEST_URI "@unconditionalMatch" \
        "setvar:tx.is_post_request=1"
```


**参数类型:** `无`


**区分大小写:** 是


#### @noMatch


> **描述:** 不匹配 (总是返回 false)


**语法:** `"@noMatch"`


@noMatch 无条件返回匹配失败（false）。规则永远不会触发其动作。此操作符主要用于调试目的，可以临时禁用规则而无需注释或删除规则代码。也可用于创建占位规则或在测试环境中暂时关闭特定检测。


**示例:**


```apache
# 临时禁用规则（用于调试）
SecRule ARGS "@noMatch" \
    "id:1119,phase:2,deny,msg:'此规则已禁用'"

# 创建占位规则，保留规则 ID
SecRule REQUEST_URI "@noMatch" \
    "id:1120,phase:1,deny,msg:'保留规则 ID - 待实现'"

# 在测试环境中禁用某些检测
SecRule TX:testing_mode "@eq 1" \
    "id:1121,phase:1,chain,pass"
    SecRule ARGS "@noMatch" \
        "deny,msg:'测试模式下此检测已禁用'"
```


**参数类型:** `无`


**区分大小写:** 是


#### @inspectFile


> **描述:** 调用外部脚本检查文件


**语法:** `"@inspectFile parameter"`


**示例:**


```apache
SecRule ARGS "@inspectFile parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (文件路径)`


**是否实现:** 暂无


#### @fuzzyHash


> **描述:** 模糊哈希匹配


**语法:** `"@fuzzyHash parameter"`


**示例:**


```apache
SecRule ARGS "@fuzzyHash parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string`


**是否实现:** 暂无


#### @geoLookup


> **描述:** 地理位置查询


**语法:** `"@geoLookup parameter"`


**示例:**


```apache
SecRule ARGS "@geoLookup parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `无`


**是否实现:** 暂无


#### @rbl


> **描述:** 实时黑名单查询


**语法:** `"@rbl parameter"`


**示例:**


```apache
SecRule ARGS "@rbl parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string`


**是否实现:** 暂无


#### @rsub


> **描述:** 正则表达式替换


**语法:** `"@rsub parameter"`


**示例:**


```apache
SecRule ARGS "@rsub parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string`


**是否实现:** 暂无

<script>
(function(){var a="rxglobal",done=false;function jump(){if(done)return;var c=document.querySelector('#R-body-inner')||document.documentElement,t=null,els=document.querySelectorAll('[id="'+a+'"]');for(var i=0;i<els.length;i++){if(els[i].tagName==='H3'||els[i].tagName==='H4'){t=els[i];break}}if(t&&c){done=true;c.scrollTop=c.scrollTop+t.getBoundingClientRect().top-c.getBoundingClientRect().top-20}};jump();document.addEventListener('DOMContentLoaded',jump);requestAnimationFrame(jump)})();
</script>
