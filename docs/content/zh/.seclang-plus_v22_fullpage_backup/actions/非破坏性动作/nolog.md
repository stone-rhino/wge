+++
title = "nolog"
weight = 2
+++

本章详细介绍 SecLang++ 中的动作。

## 动作


            
动作定义了规则匹配时执行的操作。动作分为几类:元数据动作、非破坏性动作、破坏性动作、数据动作和流程控制动作。


            
### 动作语法


            
多个动作使用逗号分隔:

            
```
"id:1001,phase:2,deny,status:403,msg:'Attack Detected'"
```


            
### 元数据动作


| 名称 | 描述 |
|---|---|
| [`id`](#id) | 规则唯一标识符 (必需) |
| [`phase`](#phase) | 处理阶段 (1-5) |
| [`msg`](#msg) | 日志消息 |
| [`severity`](#severity) | 严重级别 (0-7) |
| [`tag`](#tag) | 规则标签 |
| [`ver`](#ver) | 规则版本 |
| [`rev`](#rev) | 规则修订版本 |
| [`accuracy`](#accuracy) | 准确度级别 (1-9) |
| [`maturity`](#maturity) | 成熟度级别 (1-9) |


#### id

> **描述:** 规则唯一标识符 (必需)

**语法:** `id:number`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny"
```


**参数类型:** `int`


**是否实现:** 暂无


**区分大小写:** 是


#### phase


> **描述:** 处理阶段 (1-5)


**语法:** `phase:1|2|3|4|5|request|response|logging`


phase 动作指定规则在哪个处理阶段执行。WGE 在处理 HTTP 事务时会按顺序执行五个阶段，每个阶段在特定数据可用时触发。选择正确的阶段对规则的有效性至关重要。


- **phase:1 (request)** - 请求头阶段：在接收完整请求头后立即执行，此时请求体尚未读取。用于早期决策，如 IP 黑名单、URL 检测。
- **phase:2 (request)** - 请求体阶段：在请求体完全接收并解析后执行。大多数应用层规则在此阶段运行，可访问 ARGS、REQUEST_BODY 等变量。
- **phase:3 (response)** - 响应头阶段：在接收后端响应头后执行，此时响应体尚未读取。用于检查响应状态码和响应头。
- **phase:4 (response)** - 响应体阶段：在响应体完全接收后执行。用于检测响应内容中的敏感数据泄露。
- **phase:5 (logging)** - 日志阶段：在响应发送完成后执行，无法阻止请求。仅用于日志记录和统计。
**示例:**


```apache
# 在请求头阶段检测 IP 黑名单
SecRule REMOTE_ADDR "@ipMatch 1.2.3.4" "id:1001,phase:1,deny,msg:'IP 黑名单'"

# 在请求体阶段检测 SQL 注入
SecRule ARGS "@detectSQLi" "id:1002,phase:2,deny,msg:'SQL 注入检测'"

# 在响应体阶段检测敏感数据泄露
SecRule RESPONSE_BODY "@rx \d{16}" "id:1003,phase:4,deny,msg:'信用卡号泄露'"
```


**参数类型:** `int 或 string`


**区分大小写:** 是


#### msg

> **描述:** 日志消息

**语法:** `msg:'message'`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,msg:'SQL Injection Detected'"
```


**参数类型:** `string`


**区分大小写:** 是


#### severity

> **描述:** 严重级别 (0-7)

**语法:** `severity:CRITICAL|ERROR|WARNING|NOTICE|INFO|DEBUG`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,severity:CRITICAL"
```


**参数类型:** `int 或 string`


**区分大小写:** 是


#### tag

> **描述:** 规则标签

**语法:** `tag:'tagname'`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,tag:'attack-sqli'"
```


**参数类型:** `string`


**区分大小写:** 是


#### ver

> **描述:** 规则版本

**语法:** `ver:'version'`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,ver:'1.0.0'"
```


**参数类型:** `string`


**区分大小写:** 是


#### rev

> **描述:** 规则修订版本

**语法:** `rev:'revision'`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,rev:'2'"
```


**参数类型:** `string`


**区分大小写:** 是


#### accuracy

> **描述:** 准确度级别 (1-9)

**语法:** `accuracy:level`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,accuracy:8"
```


**参数类型:** `int`


**区分大小写:** 是


#### maturity

> **描述:** 成熟度级别 (1-9)

**语法:** `maturity:level`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,maturity:9"
```


**参数类型:** `int`


**区分大小写:** 是


            
#### 处理阶段说明

            
- **Phase 1:** 请求头处理阶段
- **Phase 2:** 请求体处理阶段
- **Phase 3:** 响应头处理阶段
- **Phase 4:** 响应体处理阶段
- **Phase 5:** 日志记录阶段
#### 严重级别说明

            
- **0 / EMERGENCY:** 紧急
- **1 / ALERT:** 告警
- **2 / CRITICAL:** 严重
- **3 / ERROR:** 错误
- **4 / WARNING:** 警告
- **5 / NOTICE:** 注意
- **6 / INFO:** 信息
- **7 / DEBUG:** 调试
### 破坏性动作


| 名称 | 描述 |
|---|---|
| [`deny`](#deny) | 拒绝请求 |
| [`drop`](#drop) | 丢弃连接 |
| [`allow`](#allow) | 允许请求通过 |
| [`allow:phase`](#allowphase) | 允许请求通过当前阶段 |
| [`allow:request`](#allowrequest) | 允许整个请求通过 |
| [`redirect`](#redirect) | 重定向到指定 URL |
| [`pass`](#pass) | 继续处理后续规则 |
| [`block`](#block) | 使用默认破坏性动作 |


#### deny


> **描述:** 拒绝请求


**语法:** `deny`


deny 是最常用的破坏性动作，用于立即终止当前请求的处理并返回错误响应。当规则匹配时，WGE 将停止处理后续规则，并向客户端返回指定的 HTTP 状态码（默认 403 Forbidden）。


deny 动作通常与 status 动作配合使用，以指定自定义的响应状态码。在异常评分模式中，deny 通常只用于最终的评分阈值检查规则。


**示例:**


```apache
# 基本拒绝 - 返回默认 403 状态码
SecRule ARGS "@detectSQLi" "id:1,phase:2,deny,msg:'SQL 注入检测'"

# 指定自定义状态码
SecRule REQUEST_URI "@contains ../../../" \
    "id:2,phase:1,deny,status:400,msg:'路径遍历攻击'"

# 异常评分模式中的阈值检查
SecRule TX:anomaly_score "@ge 5" \
    "id:3,phase:2,deny,status:403,msg:'异常评分超标: %{TX.anomaly_score}'"

# 返回 503 表示服务暂时不可用
SecRule IP:request_count "@gt 1000" \
    "id:4,phase:1,deny,status:503,msg:'请求频率限制'"
```


**参数类型:** `无`


**区分大小写:** 是

#### drop

> **描述:** 丢弃连接

**信息:** Original Example: `drop`

**语法:** `drop | drop:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,drop,status:403"
```


**参数类型:** `无`


**区分大小写:** 是

#### allow

> **描述:** 允许请求通过

**信息:** Original Example: `allow`

**语法:** `allow | allow:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,allow,status:403"
```


**参数类型:** `无 或 string`


**区分大小写:** 是

#### allow:phase

> **描述:** 允许请求通过当前阶段

**信息:** Original Example: `allow:phase`

**语法:** `allow:phase`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,allow:phase"
```


**参数类型:** `string`


该操作支持更精细的控制。可选参数如下： 若需放行响应，请在 RESPONSE_HEADERS 阶段添加规则并单独使用 allow：

#### allow:request

> **描述:** 允许整个请求通过

**信息:** Original Example: `allow:request`

**语法:** `allow:request`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,allow:request"
```


**参数类型:** `string`


此操作对 SecRule 指令和 SecAction 指令均为必需。目前虽非 SecRuleScript 指令强制要求，但强烈建议使用。

#### redirect

> **描述:** 重定向到指定 URL

**信息:** Original Example: `redirect:https://example.com`

**语法:** `redirect | redirect:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,redirect,status:403"
```


**参数类型:** `string`


**区分大小写:** 是

#### pass

> **描述:** 继续处理后续规则

**信息:** Original Example: `pass`

**语法:** `pass | pass:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,pass,status:403"
```


**参数类型:** `无`


**区分大小写:** 是

#### block

> **描述:** 使用默认破坏性动作

**信息:** Original Example: `block`

**语法:** `block | block:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,block,status:403"
```


**参数类型:** `无`


**区分大小写:** 是


            
### 非破坏性动作


| 名称 | 描述 |
|---|---|
| [`log`](#log) | 记录到错误日志 |
| [`nolog`](#nolog) | 不记录到错误日志 |
| [`auditlog`](#auditlog) | 记录到审计日志 |
| [`noauditlog`](#noauditlog) | 不记录到审计日志 |
| [`logdata`](#logdata) | 记录额外数据到日志 |
| [`capture`](#capture) | 捕获正则表达式匹配的内容 |
| [`multiMatch`](#multimatch) | 对所有匹配执行动作 |


#### log

> **描述:** 记录到错误日志

**信息:** Original Example: `log`

**语法:** `log | log:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,log,status:403"
```


**参数类型:** `无`


**区分大小写:** 是

#### nolog


> **描述:** 不记录到错误日志

**信息:** Original Example: `nolog`

**语法:** `nolog | nolog:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,nolog,status:403"
```


**参数类型:** `无`


**区分大小写:** 是

#### auditlog

> **描述:** 记录到审计日志

**信息:** Original Example: `auditlog`

**语法:** `auditlog | auditlog:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,auditlog,status:403"
```


**参数类型:** `无`


**区分大小写:** 是

#### noauditlog

> **描述:** 不记录到审计日志

**信息:** Original Example: `noauditlog`

**语法:** `noauditlog | noauditlog:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,noauditlog,status:403"
```


**参数类型:** `无`


**区分大小写:** 是

#### logdata

> **描述:** 记录额外数据到日志

**信息:** Original Example: `logdata:'User: %{MATCHED_VAR}'`

**语法:** `logdata | logdata:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,logdata,status:403"
```


**参数类型:** `string`


**区分大小写:** 是

#### capture

> **描述:** 捕获正则表达式匹配的内容

**信息:** Original Example: `capture`

**语法:** `capture | capture:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,capture,status:403"
```


**参数类型:** `无`


**区分大小写:** 是

#### multiMatch

> **描述:** 对所有匹配执行动作

**信息:** Original Example: `multiMatch`

**语法:** `multiMatch | multiMatch:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,multiMatch,status:403"
```


**参数类型:** `无`


**区分大小写:** 是


            
### 变量操作动作


| 名称 | 描述 |
|---|---|
| [`setvar`](#setvar) | 设置变量 |
| [`expirevar`](#expirevar) | 设置变量过期时间 |
| [`setenv`](#setenv) | 设置环境变量 |
| [`setuid`](#setuid) | 设置用户 ID |
| [`setsid`](#setsid) | 设置会话 ID |
| [`setrsc`](#setrsc) | 设置资源 ID |
| [`initcol`](#initcol) | 初始化持久化集合 |


#### setvar


> **描述:** 设置变量


**语法:** `setvar:collection.key=value`


setvar 动作用于创建、修改或删除集合变量。这是实现异常评分、状态跟踪和规则间数据传递的核心动作。支持多种操作：


- **setvar:tx.key=value** - 设置变量为指定值
- **setvar:tx.key=+value** - 将变量增加指定值（数值累加）
- **setvar:tx.key=-value** - 将变量减少指定值
- **setvar:!tx.key** - 删除变量

可在单条规则中使用多个 setvar 动作。支持宏扩展，可使用 %{VARIABLE} 引用其他变量的值。


**示例:**


```apache
# 初始化异常评分
SecAction "id:1,phase:1,pass,nolog,setvar:tx.anomaly_score=0"

# 检测到攻击时增加评分
SecRule ARGS "@detectSQLi" \
    "id:2,phase:2,pass,setvar:tx.anomaly_score=+5,msg:'SQL 注入 +5'"

# 设置标志变量
SecRule REQUEST_URI "@beginsWith /api" \
    "id:3,phase:1,pass,nolog,setvar:tx.is_api=1"

# 使用宏扩展设置变量
SecRule REQUEST_HEADERS:User-Agent "@rx (bot|spider)" \
    "id:4,phase:1,pass,setvar:tx.client_type=%{MATCHED_VAR}"

# 删除变量
SecRule TX:temporary "@eq 1" \
    "id:5,phase:5,pass,nolog,setvar:!tx.temporary"
```


**参数类型:** `string`


**区分大小写:** 是

#### expirevar

> **描述:** 设置变量过期时间

**信息:** Original Example: `expirevar:ip.blocked=600`

**语法:** `expirevar | expirevar:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,expirevar,status:403"
```


**参数类型:** `string`


**区分大小写:** 是

#### setenv

> **描述:** 设置环境变量

**信息:** Original Example: `setenv:attack_detected=1`

**语法:** `setenv | setenv:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,setenv,status:403"
```


**参数类型:** `string`


**区分大小写:** 是

#### setuid

> **描述:** 设置用户 ID

**信息:** Original Example: `setuid:%{REMOTE_ADDR}`

**语法:** `setuid | setuid:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,setuid,status:403"
```


**参数类型:** `string`


**是否实现:** 暂无


**区分大小写:** 是

#### setsid

> **描述:** 设置会话 ID

**信息:** Original Example: `setsid:%{SESSION.id}`

**语法:** `setsid | setsid:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,setsid,status:403"
```


**参数类型:** `string`


**是否实现:** 暂无


**区分大小写:** 是

#### setrsc

> **描述:** 设置资源 ID

**信息:** Original Example: `setrsc:%{REQUEST_FILENAME}`

**语法:** `setrsc | setrsc:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,setrsc,status:403"
```


**参数类型:** `string`


**是否实现:** 暂无


**区分大小写:** 是

#### initcol


> **描述:** 初始化持久化集合


**语法:** `initcol:collection=key`


initcol 动作用于初始化持久化集合（IP、SESSION、USER、RESOURCE、GLOBAL）。持久化集合允许在多个请求之间保存和共享数据，是实现 IP 频率限制、会话跟踪、用户行为分析等功能的基础。


**可用集合:**


- **IP** - 基于客户端 IP 地址的持久化存储
- **SESSION** - 基于会话 ID 的持久化存储
- **USER** - 基于用户 ID 的持久化存储
- **RESOURCE** - 基于资源标识的持久化存储
- **GLOBAL** - 全局持久化存储

集合数据会在规则执行过程中自动从存储加载，并在修改后自动持久化。


**示例:**


```apache
# 初始化 IP 集合用于频率限制
SecAction "id:1,phase:1,pass,nolog,initcol:IP=%{REMOTE_ADDR}"

# IP 频率限制实现
SecRule IP:request_count "@gt 100" \
    "id:2,phase:1,deny,msg:'IP 请求频率超限'"
SecRule REQUEST_URI "@unconditionalMatch" \
    "id:3,phase:1,pass,nolog,setvar:IP.request_count=+1,\
    expirevar:IP.request_count=60"

# 初始化会话集合
SecRule REQUEST_COOKIES:session_id "@rx ^[a-f0-9]{32}$" \
    "id:4,phase:1,pass,nolog,initcol:SESSION=%{REQUEST_COOKIES.session_id}"

# 初始化全局集合
SecAction "id:5,phase:1,pass,nolog,initcol:GLOBAL=global"
```


**参数类型:** `string`


**区分大小写:** 是


            
### 执行动作


| 名称 | 描述 |
|---|---|
| [`exec`](#exec) | 执行外部脚本或命令 |


#### exec

> **描述:** 执行外部脚本或命令

**信息:** Original Example: `exec:/path/to/script.sh`

**语法:** `exec | exec:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,exec,status:403"
```


**参数类型:** `string (文件路径)`


**区分大小写:** 是

<script>
(function(){var a="nolog",done=false;function jump(){if(done)return;var c=document.querySelector('#R-body-inner')||document.documentElement,t=null,els=document.querySelectorAll('[id="'+a+'"]');for(var i=0;i<els.length;i++){if(els[i].tagName==='H3'||els[i].tagName==='H4'){t=els[i];break}}if(t&&c){done=true;c.scrollTop=c.scrollTop+t.getBoundingClientRect().top-c.getBoundingClientRect().top-20}};jump();document.addEventListener('DOMContentLoaded',jump);requestAnimationFrame(jump)})();
</script>
