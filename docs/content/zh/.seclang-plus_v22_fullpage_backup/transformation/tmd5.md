+++
title = "t:md5"
weight = 34
+++

本章详细介绍 SecLang++ 中的转码函数。

### 转换动作


| 名称 | 描述 |
|---|---|
| [`t:none`](#tnone) | 不进行转换 |
| [`t:lowercase`](#tlowercase) | 转换为小写 |
| [`t:uppercase`](#tuppercase) | 转换为大写 |
| [`t:urlDecode`](#turldecode) | URL 解码 |
| [`t:urlDecodeUni`](#turldecodeuni) | Unicode URL 解码 |
| [`t:urlEncode`](#turlencode) | URL 编码 |
| [`t:base64Decode`](#tbase64decode) | Base64 解码 |
| [`t:base64DecodeExt`](#tbase64decodeext) | Base64 扩展解码 |
| [`t:base64Encode`](#tbase64encode) | Base64 编码 |
| [`t:cmdline`](#tcmdline) | 命令行参数规范化 |
| [`t:hexDecode`](#thexdecode) | 十六进制解码 |
| [`t:hexEncode`](#thexencode) | 十六进制编码 |
| [`t:htmlEntityDecode`](#thtmlentitydecode) | HTML 实体解码 |
| [`t:jsDecode`](#tjsdecode) | JavaScript 解码 |
| [`t:cssDecode`](#tcssdecode) | CSS 解码 |
| [`t:escapeSeqDecode`](#tescapeseqdecode) | 转义序列解码 |
| [`t:sqlHexDecode`](#tsqlhexdecode) | SQL 十六进制解码 |
| [`t:removeNulls`](#tremovenulls) | 移除空字符 |
| [`t:removeWhitespace`](#tremovewhitespace) | 移除空白字符 |
| [`t:compressWhitespace`](#tcompresswhitespace) | 压缩空白字符 |
| [`t:removeComments`](#tremovecomments) | 移除注释 |
| [`t:removeCommentsChar`](#tremovecommentschar) | 移除注释字符 |
| [`t:replaceComments`](#treplacecomments) | 替换注释为空格 |
| [`t:replaceNulls`](#treplacenulls) | 替换空字符 |
| [`t:normalizePath`](#tnormalizepath) | 规范化路径 |
| [`t:normalisePath`](#tnormalisepath) | 规范化路径 (英式) |
| [`t:normalizePathWin`](#tnormalizepathwin) | 规范化 Windows 路径 |
| [`t:normalisePathWin`](#tnormalisepathwin) | 规范化 Windows 路径 (英式) |
| [`t:parityEven7bit`](#tparityeven7bit) | 7位偶校验解码 |
| [`t:parityOdd7bit`](#tparityodd7bit) | 7位奇校验解码 |
| [`t:parityZero7bit`](#tparityzero7bit) | 7位零校验解码 |
| [`t:utf8ToUnicode`](#tutf8tounicode) | UTF-8 转 Unicode |
| [`t:length`](#tlength) | 获取字符串长度 |
| [`t:md5`](#tmd5) | 计算 MD5 哈希 |
| [`t:sha1`](#tsha1) | 计算 SHA1 哈希 |
| [`t:trim`](#ttrim) | 去除首尾空白 |
| [`t:trimLeft`](#ttrimleft) | 去除开头空白 |
| [`t:trimRight`](#ttrimright) | 去除结尾空白 |


#### t:none


> **描述:** 清除转换链


**语法:** `t:none`


t:none 用于清除之前所有的转换函数，重置转换链。这在需要覆盖 SecDefaultAction 或 SecRule 继承的默认转换时非常有用。当使用 t:none 后，只有在其之后显式指定的转换函数才会被应用。


**示例:**


```apache
# 假设默认动作包含 t:lowercase,t:urlDecode
SecDefaultAction "phase:2,log,pass,t:lowercase,t:urlDecode"

# 此规则会继承默认转换
SecRule ARGS "@rx admin" "id:1,deny,msg:'检测到 admin'"

# 使用 t:none 清除默认转换，仅使用 t:base64Decode
SecRule ARGS:encoded "@rx admin" \
    "id:2,t:none,t:base64Decode,deny,msg:'Base64 中检测到 admin'"

# 完全不进行任何转换
SecRule REQUEST_BODY "@rx ^\{" \
    "id:3,t:none,pass,setvar:tx.is_json=1"
```


**参数类型:** `无`


**区分大小写:** 是


#### t:lowercase

> **描述:** 转换为小写

**语法:** `t:lowercase`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:lowercase"
```


**参数类型:** `string`


使用当前 C 语言环境设置将所有字符转换为小写。


#### t:uppercase

> **描述:** 转换为大写

**语法:** `t:uppercase`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:uppercase"
```


**参数类型:** `string`


**是否实现:** 暂无


#### t:urlDecode

> **描述:** URL 解码

**语法:** `t:urlDecode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:urlDecode"
```


**参数类型:** `string`


解码 URL 编码的输入字符串。无效编码（即使用非十六进制字符的编码，或位于字符串末尾且缺失一至两个字节的编码）不会被转换，但不会抛出错误。要检测无效编码，请先对输入数据使用 @validateUrlEncoding 操作符。请勿对已进行 URL 解码的变量（如请求参数）使用此转换函数，除非您有意执行两次 URL 解码！


#### t:urlDecodeUni

> **描述:** Unicode URL 解码

**语法:** `t:urlDecodeUni`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:urlDecodeUni"
```


**参数类型:** `string`


与 urlDecode 功能相同，但支持微软专有 %u 编码。若代码值在 FF01-FF5E 范围内（全宽 ASCII 码），则使用高位字节检测并调整低位字节；否则仅使用低位字节，高位字节清零。


#### t:urlEncode

> **描述:** URL 编码

**语法:** `t:urlEncode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:urlEncode"
```


**参数类型:** `string`


**是否实现:** 暂无


#### t:base64Decode


> **描述:** Base64 解码


**语法:** `t:base64Decode`


t:base64Decode 解码 Base64 编码的字符串。攻击者经常使用 Base64 编码来隐藏恶意载荷以绕过安全检测。此转换函数在检测前先解码数据，揭示隐藏的攻击模式。


**示例:**


```apache
# 检测 Base64 编码的攻击载荷
SecRule ARGS:data "@rx (eval|exec|system)" \
    "id:1,phase:2,t:base64Decode,deny,msg:'Base64 编码的代码执行检测'"

# 原始输入: ZXZhbChiYXNlNjRfZGVjb2RlKCR4KSk=
# 解码后: eval(base64_decode($x))

# 结合其他转换检测
SecRule ARGS "@detectSQLi" \
    "id:2,phase:2,t:base64Decode,t:urlDecode,deny,msg:'编码的 SQL 注入'"
```


**参数类型:** `无`


**区分大小写:** 是


#### t:base64DecodeExt

> **描述:** Base64 扩展解码 (支持更多变体)

**语法:** `t:base64DecodeExt`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:base64DecodeExt"
```


**参数类型:** `string`


**是否实现:** 暂无


#### t:base64Encode

> **描述:** Base64 编码

**语法:** `t:base64Encode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:base64Encode"
```


**参数类型:** `string`


**是否实现:** 暂无


#### t:cmdline


> **描述:** 命令行参数规范化


**语法:** `t:cmdline`


t:cmdline 将命令行参数规范化，用于检测命令注入攻击。在 Windows 和 Unix 系统中，命令可能通过不同方式转义（如使用引号、反斜杠、脱字符等）。此转换函数通过以下方式处理输入：


- 删除所有反斜杠 (\)
- 删除所有双引号 (")
- 删除所有单引号 (')
- 删除所有脱字符 (^)
- 删除 /之前的空格
- 删除 -之前的空格
- 压缩多余空白字符
**示例:**


```apache
# 检测命令注入
SecRule ARGS "@rx (cat|ls|wget|curl|nc|bash)" \
    "id:1,phase:2,t:cmdline,t:lowercase,deny,msg:'命令注入检测'"

# 原始输入: c^a"t /e't'c/p\asswd
# 处理后: cat /etc/passwd
```


**参数类型:** `无`


**区分大小写:** 是


#### t:hexDecode

> **描述:** 十六进制解码

**语法:** `t:hexDecode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:hexDecode"
```


**参数类型:** `string`


解码采用与 hexEncode 相同算法编码的字符串（参见后续条目）。


#### t:hexEncode

> **描述:** 十六进制编码

**语法:** `t:hexEncode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:hexEncode"
```


**参数类型:** `string`


将字符串（可能包含二进制字符）编码为每个输入字节对应两个十六进制字符。例如，xyz 将编码为 78797a。


#### t:htmlEntityDecode

> **描述:** HTML 实体解码

**语法:** `t:htmlEntityDecode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:htmlEntityDecode"
```


**参数类型:** `string`


解码以 HTML 实体形式编码的字符。支持以下变体： 该函数始终将单个 HTML 实体转换为一个字节，可能导致信息丢失（若实体对应的字符无法用单字节表示）。因此它有助于揭示本无需编码的字节，但无法对 0xff 以上范围的字符进行有效处理。


#### t:jsDecode

> **描述:** JavaScript 解码

**语法:** `t:jsDecode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:jsDecode"
```


**参数类型:** `string`


解码 JavaScript 转义序列。若 \uHHHH 代码位于 FF01-FF5E 范围（全宽 ASCII 码），则使用高位字节检测并调整低位字节；否则仅使用低位字节，高位字节清零（可能导致信息丢失）。


#### t:cssDecode


> **描述:** CSS 解码


**语法:** `t:cssDecode`


t:cssDecode 解码 CSS 转义序列。CSS 允许使用反斜杠后跟十六进制数字（最多6位）来表示字符，攻击者可能利用此特性绕过检测。此转换函数可以规范化这些 CSS 编码的字符。


**示例:**


```apache
# 检测 CSS 注入攻击
SecRule ARGS "@rx expression|javascript" \
    "id:1,phase:2,t:cssDecode,t:lowercase,deny,msg:'CSS 注入检测'"

# 原始输入: \65\78\70\72\65\73\73\69\6f\6e (CSS 编码的 "expression")
# 解码后: expression
```


**参数类型:** `无`


**区分大小写:** 是


#### t:escapeSeqDecode

> **描述:** 转义序列解码

**语法:** `t:escapeSeqDecode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:escapeSeqDecode"
```


**参数类型:** `string`


解码 ANSI C 转义序列：\a, \b, \f, \n, \r, \t, \v, \\, \?, \', \", \xHH（十六进制），\0OOO（八进制）。无效编码将保留在输出中。


#### t:sqlHexDecode

> **描述:** SQL 十六进制解码

**语法:** `t:sqlHexDecode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:sqlHexDecode"
```


**参数类型:** `string`


**是否实现:** 暂无


#### t:removeNulls

> **描述:** 移除空字符

**语法:** `t:removeNulls`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeNulls"
```


**参数类型:** `string`


移除输入中的所有 NUL 字节。


#### t:removeWhitespace

> **描述:** 移除空白字符

**语法:** `t:removeWhitespace`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeWhitespace"
```


**参数类型:** `string`


成功匹配模式后，最多将复制 100 个捕获项，每个捕获项的名称由 0 至 99 的数字组成。变量 TX.0 始终包含正则表达式匹配到的完整区域。其余所有变量均包含捕获值，其顺序与正则表达式中捕获括号的出现顺序一致。


#### t:compressWhitespace

> **描述:** 压缩空白字符

**语法:** `t:compressWhitespace`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:compressWhitespace"
```


**参数类型:** `string`


将任意空白字符（0x20, \f, \t, \n, \r, \v, 0xa0）转换为空格（ASCII 0x20），并将连续多个空格压缩为单个空格。


#### t:removeComments

> **描述:** 移除注释

**语法:** `t:removeComments`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeComments"
```


**参数类型:** `string`


移除所有注释符号（/* ... */、--、#）。连续出现的多个符号不会被压缩。


#### t:removeCommentsChar

> **描述:** 移除注释字符

**语法:** `t:removeCommentsChar`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeCommentsChar"
```


**参数类型:** `string`


移除常见注释字符（/*, */, --, #）。


#### t:replaceComments

> **描述:** 替换注释为空格

**语法:** `t:replaceComments`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:replaceComments"
```


**参数类型:** `string`


将所有 C 风格注释（/* ... */）替换为单个空格（连续多个空格不会被压缩）。未终止的注释也将替换为空格（ASCII 0x20）。但独立存在的注释终止符（*/）不会被处理。


#### t:replaceNulls

> **描述:** 替换空字符

**语法:** `t:replaceNulls`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:replaceNulls"
```


**参数类型:** `string`


将输入中的 NUL 字节替换为空格字符（ASCII 0x20）。


#### t:normalizePath

> **描述:** 规范化路径

**语法:** `t:normalizePath`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalizePath"
```


**参数类型:** `string`


移除输入字符串中的多重斜杠、目录自引用及目录后向引用（位于输入开头的情况除外）。


#### t:normalisePath

> **描述:** 规范化路径 (英式拼写)

**语法:** `t:normalisePath`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalisePath"
```


**参数类型:** `string`


参见 normalizePath。


#### t:normalizePathWin

> **描述:** 规范化 Windows 路径

**语法:** `t:normalizePathWin`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalizePathWin"
```


**参数类型:** `string`


功能与 normalizePath 相同，但会先将反斜杠转换为正斜杠。


#### t:normalisePathWin

> **描述:** 规范化 Windows 路径 (英式拼写)

**语法:** `t:normalisePathWin`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalisePathWin"
```


**参数类型:** `string`


参见 normalizePathWin。


#### t:parityEven7bit

> **描述:** 7位偶校验解码

**语法:** `t:parityEven7bit`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:parityEven7bit"
```


**参数类型:** `string`


**是否实现:** 暂无


#### t:parityOdd7bit

> **描述:** 7位奇校验解码

**语法:** `t:parityOdd7bit`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:parityOdd7bit"
```


**参数类型:** `string`


**是否实现:** 暂无


#### t:parityZero7bit

> **描述:** 7位零校验解码

**语法:** `t:parityZero7bit`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:parityZero7bit"
```


**参数类型:** `string`


**是否实现:** 暂无


#### t:utf8ToUnicode

> **描述:** UTF-8 转 Unicode

**语法:** `t:utf8ToUnicode`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:utf8ToUnicode"
```


**参数类型:** `string`


将所有 UTF-8 字符序列转换为 Unicode（采用 '%uHHHH' 格式）。此功能可帮助输入规范化，特别适用于非英语语言，最大限度减少误报和漏报。


#### t:length

> **描述:** 获取字符串长度

**语法:** `t:length`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:length"
```


**参数类型:** `string`


查询输入字符串的长度（以字节为单位），并将结果（作为字符串）输出。例如输入 ABCDE 时，该转换函数将返回 5。


#### t:md5

> **描述:** 计算 MD5 哈希

**语法:** `t:md5`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:md5"
```


**参数类型:** `string`


**是否实现:** 暂无


#### t:sha1

> **描述:** 计算 SHA1 哈希

**语法:** `t:sha1`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:sha1"
```


**参数类型:** `string`


计算输入字符串的 SHA1 哈希值。计算结果为原始二进制格式，打印（或记录）时可能需要转码为文本。哈希函数常与十六进制编码组合使用（例如：t:sha1,t:hexEncode）。


#### t:trim

> **描述:** 去除首尾空白

**语法:** `t:trim`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:trim"
```


**参数类型:** `string`


同时清除输入字符串左右两侧的空格。


#### t:trimLeft


> **描述:** 去除开头空白


**语法:** `t:trimLeft`


t:trimLeft 去除输入字符串左侧（开头）的所有空白字符，包括空格、制表符、换行符等。


**示例:**


```apache
# 去除参数开头的空白后检测
SecRule ARGS:username "@rx ^admin" \
    "id:1,phase:2,t:trimLeft,deny,msg:'检测到 admin 用户名'"

# 原始输入: "   admin"
# 处理后: "admin"
```


**参数类型:** `无`


**区分大小写:** 是


#### t:trimRight


> **描述:** 去除结尾空白


**语法:** `t:trimRight`


t:trimRight 去除输入字符串右侧（结尾）的所有空白字符，包括空格、制表符、换行符等。


**示例:**


```apache
# 去除参数结尾的空白后检测
SecRule ARGS:cmd "@rx ;$" \
    "id:1,phase:2,t:trimRight,deny,msg:'检测到命令分隔符'"

# 原始输入: "ls -la;   "
# 处理后: "ls -la;"
```


**参数类型:** `无`


**区分大小写:** 是


            
### 流程控制动作


| 名称 | 描述 |
|---|---|
| [`chain`](#chain) | 链接下一个规则 |
| [`skip`](#skip) | 跳过指定数量的规则 |
| [`skipAfter`](#skipafter) | 跳转到指定标记 |


#### chain


> **描述:** 链接下一个规则


**语法:** `chain`


chain 动作将当前规则与下一条规则链接，形成逻辑 AND 关系。只有当链中所有规则都匹配时，才会执行最终的破坏性动作。这允许创建复杂的多条件检测逻辑。


**规则链特性:**


- 链中的第一条规则必须包含 id 和 phase 动作
- 后续规则不能有独立的 id 和 phase
- 破坏性动作（如 deny）只能在链的最后一条规则中指定
- 非破坏性动作（如 setvar、log）可以在任意链节点使用
- 如果链中任一规则不匹配，整个链终止，不执行破坏性动作
**示例:**


```apache
# 检测 POST 请求 且 包含特定参数 (AND 逻辑)
SecRule REQUEST_METHOD "@streq POST" \
    "id:1,phase:2,chain,deny,msg:'POST 请求中检测到危险参数'"
    SecRule ARGS:action "@streq delete" ""

# 三条件链：POST + 特定路径 + 可疑内容
SecRule REQUEST_METHOD "@streq POST" \
    "id:2,phase:2,chain,log,deny,msg:'可疑的管理操作'"
    SecRule REQUEST_URI "@beginsWith /admin" "chain"
        SecRule ARGS "@rx (drop|truncate|delete)" ""

# 仅在特定条件下记录日志 (非破坏性链)
SecRule REQUEST_HEADERS:User-Agent "@rx curl" \
    "id:3,phase:1,chain,pass,log,msg:'curl 访问 API'"
    SecRule REQUEST_URI "@beginsWith /api" ""
```


**参数类型:** `无`


**区分大小写:** 是

#### skip

> **描述:** 跳过指定数量的规则

**信息:** Original Example: `skip:2`

**语法:** `skip | skip:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,skip,status:403"
```


**参数类型:** `int`


**区分大小写:** 是

#### skipAfter

> **描述:** 跳转到指定标记

**信息:** Original Example: `skipAfter:END_SQLI`

**语法:** `skipAfter | skipAfter:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,skipAfter,status:403"
```


**参数类型:** `string`


**区分大小写:** 是


            
### 数据动作


| 名称 | 描述 |
|---|---|
| [`status`](#status) | 设置 HTTP 响应状态码 |
| [`xmlns`](#xmlns) | 设置 XML 命名空间 |


#### status

> **描述:** 设置 HTTP 响应状态码

**信息:** Original Example: `status:403`

**语法:** `status | status:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,status,status:403"
```


**参数类型:** `int`


**区分大小写:** 是

#### xmlns

> **描述:** 设置 XML 命名空间

**信息:** Original Example: `xmlns:ns=http://example.com`

**语法:** `xmlns | xmlns:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,xmlns,status:403"
```


**参数类型:** `string`


**区分大小写:** 是


            
### 控制动作 (ctl)


            
`ctl` 动作用于在规则执行时动态修改引擎配置:

            


| 名称 | 描述 |
|---|---|
| [`ctl:ruleEngine`](#ctlruleengine) | 修改规则引擎状态 |
| [`ctl:requestBodyAccess`](#ctlrequestbodyaccess) | 修改请求体访问设置 |
| [`ctl:forceRequestBodyVariable`](#ctlforcerequestbodyvariable) | 强制将请求体放入变量 |
| [`ctl:requestBodyProcessor`](#ctlrequestbodyprocessor) | 设置请求体处理器 |
| [`ctl:auditEngine`](#ctlauditengine) | 修改审计引擎状态 |
| [`ctl:auditLogParts`](#ctlauditlogparts) | 修改审计日志部分 |
| [`ctl:ruleRemoveById`](#ctlruleremovebyid) | 移除指定 ID 的规则 |
| [`ctl:ruleRemoveByTag`](#ctlruleremovebytag) | 移除指定标签的规则 |
| [`ctl:ruleRemoveTargetById`](#ctlruleremovetargetbyid) | 移除规则的检查目标 |
| [`ctl:ruleRemoveTargetByTag`](#ctlruleremovetargetbytag) | 按标签移除规则目标 |


#### ctl:ruleEngine


> **描述:** 运行时修改规则引擎状态


**语法:** `ctl:ruleEngine=On|Off|DetectionOnly`


ctl:ruleEngine 动作允许在规则执行过程中动态修改规则引擎的运行状态。这对于实现白名单、条件性绕过检测等场景非常有用。


**可用值:**


- **On** - 启用规则引擎，执行所有规则和破坏性动作
- **Off** - 禁用规则引擎，跳过后续所有规则处理
- **DetectionOnly** - 仅检测模式，执行规则但不执行破坏性动作
**示例:**


```apache
# 为可信 IP 跳过所有检测
SecRule REMOTE_ADDR "@ipMatch 192.168.1.0/24,10.0.0.0/8" \
    "id:1,phase:1,pass,nolog,ctl:ruleEngine=Off"

# 为特定路径禁用检测（如健康检查）
SecRule REQUEST_URI "@streq /health" \
    "id:2,phase:1,pass,nolog,ctl:ruleEngine=Off"

# 对测试环境启用仅检测模式
SecRule REQUEST_HEADERS:X-Test-Mode "@streq true" \
    "id:3,phase:1,pass,nolog,ctl:ruleEngine=DetectionOnly"

# 为 API 客户端禁用某些检测
SecRule REQUEST_HEADERS:X-API-Key "@rx ^[a-z0-9]{32}$" \
    "id:4,phase:1,pass,nolog,ctl:ruleEngine=Off"
```


**参数类型:** `string`


**区分大小写:** 是


#### ctl:requestBodyAccess

> **描述:** 修改请求体访问设置

**信息:** Original Example: `ctl:requestBodyAccess=On`

**语法:** `ctl:requestBodyAccess`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,ctl:requestBodyAccess"
```


**参数类型:** `string`


集合在执行 initcol 操作时按需加载至内存。仅当事务处理过程中对集合进行了修改时，该集合才会被持久化。 更多细节请参阅"持久化存储"章节。

#### ctl:forceRequestBodyVariable

> **描述:** 强制将请求体放入变量

**信息:** Original Example: `ctl:forceRequestBodyVariable=On`

**语法:** `ctl:forceRequestBodyVariable`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,ctl:forceRequestBodyVariable"
```


**参数类型:** `string`


#### ctl:requestBodyProcessor

> **描述:** 设置请求体处理器

**信息:** Original Example: `ctl:requestBodyProcessor=JSON`

**语法:** `ctl:requestBodyProcessor`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,ctl:requestBodyProcessor"
```


**参数类型:** `string`


日志数据信息将显示在错误日志和/或审计日志文件中。系统会执行宏展开，因此可使用 %{TX.0} 或 %{MATCHED_VAR} 等变量名。该信息经过正确转义处理，适用于二进制数据日志记录。

#### ctl:auditEngine

> **描述:** 修改审计引擎状态

**信息:** Original Example: `ctl:auditEngine=Off`

**语法:** `ctl:auditEngine`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,ctl:auditEngine"
```


**参数类型:** `string`


通常，每个规则仅对变量进行一次检查，且仅在所有转换函数完成后执行。启用 multiMatch 时，每次输入数据被转换函数修改前后，都会对变量与操作符进行比对。

#### ctl:auditLogParts

> **描述:** 修改审计日志部分

**信息:** Original Example: `ctl:auditLogParts=+E`

**语法:** `ctl:auditLogParts`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,ctl:auditLogParts"
```


**参数类型:** `string`


若 SecAuditEngine 设置为 On，所有事务均会被记录。若设置为 RelevantOnly，则可通过 noauditlog 操作控制日志记录。 noauditlog 操作仅影响当前规则。若仅在某条规则中禁用审计记录，其他规则匹配仍会触发审计记录。若需完全禁用审计记录（无论是否存在规则匹配），请使用 ctl:auditEngine=Off。

#### ctl:ruleRemoveById

> **描述:** 移除指定 ID 的规则

**信息:** Original Example: `ctl:ruleRemoveById=1001`

**语法:** `ctl:ruleRemoveById`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,ctl:ruleRemoveById"
```


**参数类型:** `string`


虽然 nolog 默认包含 noauditlog，但可通过 nolog,auditlog 覆盖此默认行为。

#### ctl:ruleRemoveByTag

> **描述:** 移除指定标签的规则

**信息:** Original Example: `ctl:ruleRemoveByTag=attack-sqli`

**语法:** `ctl:ruleRemoveByTag`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,ctl:ruleRemoveByTag"
```


**参数类型:** `string`


当在多目标 SecRule 中使用 pass 时，所有变量将被检查，且每次匹配都会触发所有非破坏性操作。在下例中，每次请求参数都会使 TX.test 变量递增一次：

#### ctl:ruleRemoveTargetById

> **描述:** 移除规则的检查目标

**信息:** Original Example: `ctl:ruleRemoveTargetById=1001;ARGS:username`

**语法:** `ctl:ruleRemoveTargetById`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,ctl:ruleRemoveTargetById"
```


**参数类型:** `string`


若同一规则中存在 status 操作且其值可用于重定向（即为以下值之一：301、302、303 或 307），则该值将作为重定向状态码使用。否则将使用状态码 302。

#### ctl:ruleRemoveTargetByTag

> **描述:** 按标签移除规则目标

**信息:** Original Example: `ctl:ruleRemoveTargetByTag=attack-sqli;ARGS:id`

**语法:** `ctl:ruleRemoveTargetByTag`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,ctl:ruleRemoveTargetByTag"
```


**参数类型:** `string`

<script>
(function(){var a="tmd5",done=false;function jump(){if(done)return;var c=document.querySelector('#R-body-inner')||document.documentElement,t=null,els=document.querySelectorAll('[id="'+a+'"]');for(var i=0;i<els.length;i++){if(els[i].tagName==='H3'||els[i].tagName==='H4'){t=els[i];break}}if(t&&c){done=true;c.scrollTop=c.scrollTop+t.getBoundingClientRect().top-c.getBoundingClientRect().top-20}};jump();document.addEventListener('DOMContentLoaded',jump);requestAnimationFrame(jump)})();
</script>
