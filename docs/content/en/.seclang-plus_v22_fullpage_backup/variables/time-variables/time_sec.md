+++
title = "TIME_SEC"
weight = 8
+++

This chapter details the variables available in SecLang++.

## 变量


            
变量是 SecLang 规则中用于指定检查数据源的重要组成部分。WGE 支持丰富的变量类型,涵盖请求和响应的各个方面。


            
### 变量语法

            
变量可以使用以下方式:

            
- **单个变量:** `ARGS`
- **特定参数:** `ARGS:username` 或 `ARGS.username`
- **排除变量:** `!ARGS:token`
- **变量计数:** `&ARGS`
- **多个变量:** `ARGS|REQUEST_HEADERS`
### 请求变量


| Name | Description |
|---|---|
| [`ARGS`](#args) | 所有请求参数 (GET 和 POST) |
| [`ARGS_GET`](#args_get) | GET 请求参数 |
| [`ARGS_POST`](#args_post) | POST 请求参数 |
| [`ARGS_NAMES`](#args_names) | 所有参数名称 |
| [`ARGS_GET_NAMES`](#args_get_names) | GET 参数名称 |
| [`ARGS_POST_NAMES`](#args_post_names) | POST 参数名称 |
| [`ARGS_COMBINED_SIZE`](#args_combined_size) | 所有参数的总大小 |
| [`REQUEST_HEADERS`](#request_headers) | 请求头 |
| [`REQUEST_HEADERS_NAMES`](#request_headers_names) | 请求头名称 |
| [`REQUEST_COOKIES`](#request_cookies) | 请求 Cookie |
| [`REQUEST_COOKIES_NAMES`](#request_cookies_names) | Cookie 名称 |
| [`REQUEST_BODY`](#request_body) | 请求体内容 |
| [`REQUEST_BODY_LENGTH`](#request_body_length) | 请求体长度 |
| [`REQUEST_METHOD`](#request_method) | 请求方法 (GET, POST 等) |
| [`REQUEST_URI`](#request_uri) | 请求 URI (已解码) |
| [`REQUEST_URI_RAW`](#request_uri_raw) | 原始请求 URI (未解码) |
| [`REQUEST_FILENAME`](#request_filename) | 请求的文件路径 |
| [`REQUEST_BASENAME`](#request_basename) | 请求文件的基础名称 |
| [`REQUEST_PROTOCOL`](#request_protocol) | 请求协议 (HTTP/1.1 等) |
| [`REQUEST_LINE`](#request_line) | 完整的请求行 |
| [`QUERY_STRING`](#query_string) | 查询字符串 |
| [`FULL_REQUEST`](#full_request) | 完整的请求数据 |
| [`FULL_REQUEST_LENGTH`](#full_request_length) | 完整请求的长度 |


#### ARGS


> **Description:** 所有请求参数 (GET 和 POST)


**Syntax:** `ARGS | ARGS:Key`


**Example:**


```apache
SecRule ARGS:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```


RESTful API 解析示例

1. JSON 请求体解析 (application/json)

**请求:**


```
POST /api/users HTTP/1.1
Content-Type: application/json

{
  "username": "admin",
  "password": "secret123",
  "profile": {
    "email": "admin@example.com",
    "role": "administrator"
  },
  "tags": ["vip", "test"]
}
```


**ARGS 解析结果:**


```
ARGS:username = "admin"
ARGS:password = "secret123"
ARGS:profile.email = "admin@example.com"
ARGS:profile.role = "administrator"
ARGS:tags.0 = "vip"
ARGS:tags.1 = "test"
```


**规则示例:**


```apache
# 检测 JSON 中的敏感字段
SecRule ARGS:password "@rx (?i)(password|secret)" \
    "id:1001,phase:2,deny,status:403,msg:'Sensitive data in password field'"

# 检测嵌套 JSON 字段
SecRule ARGS:profile.role "@streq administrator" \
    "id:1002,phase:2,log,msg:'Admin role detected'"
```


2. XML 请求体解析 (application/xml)

**请求:**


```
POST /api/order HTTP/1.1
Content-Type: application/xml


  
    John Doe
    john@example.com
  
  
    
      Product A
      2
    
  

```


**XML 变量访问 (需配合 XPath):**


```apache
# 先声明命名空间（如需要）
SecRule REQUEST_HEADERS:Content-Type "@contains xml" \
    "id:2000,phase:1,pass,nolog,ctl:requestBodyProcessor=XML"

# 使用 XPath 访问 XML 节点
SecRule XML:/order/customer/name/text() "@rx admin" \
    "id:2001,phase:2,deny,msg:'Blocked admin in customer name'"

SecRule XML:/order/customer/@id "@eq 0" \
    "id:2002,phase:2,deny,msg:'Invalid customer ID'"

SecRule XML://item/@sku "@rx ^[A-Z]{3}[0-9]{3}$" \
    "id:2003,phase:2,pass,msg:'Valid SKU format'"
```


3. URL-encoded 表单 (application/x-www-form-urlencoded)

**请求:**


```
POST /login HTTP/1.1
Content-Type: application/x-www-form-urlencoded

username=admin&password=secret%26123&remember=true
```


**ARGS 解析结果:**


```
ARGS:username = "admin"
ARGS:password = "secret&123"  # URL 解码后
ARGS:remember = "true"
```


**规则示例:**


```apache
SecRule ARGS:username "@rx ^[a-zA-Z0-9_]{3,20}$" \
    "id:3001,phase:2,pass,nolog"

SecRule ARGS:username "!@rx ^[a-zA-Z0-9_]{3,20}$" \
    "id:3002,phase:2,deny,status:400,msg:'Invalid username format'"
```


4. Multipart 表单 (multipart/form-data)

**请求:**


```
POST /upload HTTP/1.1
Content-Type: multipart/form-data; boundary=----WebKitFormBoundary

------WebKitFormBoundary
Content-Disposition: form-data; name="title"

My Document
------WebKitFormBoundary
Content-Disposition: form-data; name="file"; filename="report.pdf"
Content-Type: application/pdf

[binary content]
------WebKitFormBoundary--
```


**ARGS 与 FILES 解析结果:**


```
ARGS:title = "My Document"
FILES:file = "report.pdf"
FILES_NAMES:file = "report.pdf"
FILES_SIZES:file = 102400
FILES_TMPNAMES:file = "/tmp/upload_abc123"
```


**规则示例:**


```apache
# 检查文件扩展名
SecRule FILES_NAMES "@rx \.(php|exe|sh)$" \
    "id:4001,phase:2,deny,status:403,msg:'Dangerous file extension'"

# 限制文件大小 (5MB)
SecRule FILES_SIZES "@gt 5242880" \
    "id:4002,phase:2,deny,status:413,msg:'File too large'"
```


#### ARGS_GET


> **Description:** GET 请求参数


**Syntax:** `ARGS_GET | ARGS_GET:Key`


ARGS_GET 仅包含 URL 查询字符串中传递的参数（即 GET 参数）。与 ARGS 不同，它不包含 POST 请求体中的参数，因此可用于仅检查 URL 传递的数据，避免误检 POST 数据。可以通过 ARGS_GET:paramname 访问特定参数。


**Example:**


```apache
# 检查 URL 查询参数中的 SQL 注入
SecRule ARGS_GET "@detectSQLi" "id:1001,phase:1,deny,status:403,msg:'GET 参数检测到 SQL 注入'"

# 检查特定的 GET 参数
SecRule ARGS_GET:id "@rx [^0-9]" "id:1002,phase:1,deny,msg:'ID 参数必须为数字'"
```


#### ARGS_POST


> **Description:** POST 请求参数


**Syntax:** `ARGS_POST | ARGS_POST:Key`


ARGS_POST 仅包含请求体中传递的参数（即 POST 数据）。WGE 支持解析 application/x-www-form-urlencoded、multipart/form-data、application/json 和 application/xml 格式的请求体。使用此变量可以仅检查表单提交的数据，而不检查 URL 参数。需要启用 SecRequestBodyAccess 才能访问此变量。


**Example:**


```apache
# 检查 POST 表单参数中的 XSS
SecRule ARGS_POST "@detectXSS" "id:1003,phase:2,deny,status:403,msg:'POST 参数检测到 XSS'"

# 验证登录表单的用户名格式
SecRule ARGS_POST:username "!@rx ^[a-zA-Z0-9_]{3,20}$" \
    "id:1004,phase:2,deny,msg:'用户名格式无效'"
```


#### ARGS_NAMES


> **Description:** 所有参数名称


**Syntax:** `ARGS_NAMES`


ARGS_NAMES 包含所有请求参数的名称（键名），包括 GET 和 POST 参数。此变量用于检测参数名称本身是否包含攻击载荷，这是一种常见的绕过技术，攻击者可能在参数名中注入恶意代码。


**Example:**


```apache
# 检测参数名中的 SQL 注入关键字
SecRule ARGS_NAMES "@pm select union insert delete drop" \
    "id:1005,phase:2,deny,msg:'参数名包含 SQL 关键字'"

# 限制参数名只能包含字母数字和下划线
SecRule ARGS_NAMES "!@rx ^[a-zA-Z0-9_\[\]\.]+$" \
    "id:1006,phase:2,deny,msg:'参数名包含非法字符'"
```


#### ARGS_GET_NAMES


> **Description:** GET 参数名称


**Syntax:** `ARGS_GET_NAMES`


ARGS_GET_NAMES 仅包含 URL 查询字符串中参数的名称。用于检测 GET 参数名是否包含可疑内容，而不检查 POST 参数名。


**Example:**


```apache
SecRule ARGS_GET_NAMES "@detectSQLi" "id:1007,phase:1,deny,msg:'GET 参数名检测到 SQL 注入'"
```


#### ARGS_POST_NAMES


> **Description:** POST 参数名称


**Syntax:** `ARGS_POST_NAMES`


ARGS_POST_NAMES 仅包含请求体中参数的名称。用于检测 POST 参数名是否包含可疑内容，需要启用 SecRequestBodyAccess 才能访问。


**Example:**


```apache
SecRule ARGS_POST_NAMES "@detectXSS" "id:1008,phase:2,deny,msg:'POST 参数名检测到 XSS'"
```


#### ARGS_COMBINED_SIZE


> **Description:** 所有参数的总大小


**Syntax:** `ARGS_COMBINED_SIZE | ARGS_COMBINED_SIZE:Key`


**Example:**


```apache
SecRule ARGS_COMBINED_SIZE:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### REQUEST_HEADERS


> **Description:** 请求头


**Syntax:** `REQUEST_HEADERS | REQUEST_HEADERS:HeaderName`


REQUEST_HEADERS 包含所有 HTTP 请求头。可以使用 REQUEST_HEADERS:HeaderName 访问特定头部，如 User-Agent、Host、Content-Type 等。请求头是攻击者常用的注入点，应对关键头部进行检测。


**Example:**


```apache
# 检查 User-Agent 中的恶意模式
SecRule REQUEST_HEADERS:User-Agent "@pm nikto sqlmap nmap" \
    "id:1009,phase:1,deny,msg:'检测到扫描工具 User-Agent'"

# 验证 Host 头格式
SecRule REQUEST_HEADERS:Host "!@rx ^[a-zA-Z0-9\.\-]+$" \
    "id:1010,phase:1,deny,msg:'无效的 Host 头'"

# 检查 Referer 头中的 XSS
SecRule REQUEST_HEADERS:Referer "@detectXSS" \
    "id:1011,phase:1,deny,msg:'Referer 头检测到 XSS'"
```


#### REQUEST_HEADERS_NAMES


> **Description:** 请求头名称


**Syntax:** `REQUEST_HEADERS_NAMES`


REQUEST_HEADERS_NAMES 包含所有请求头的名称（不包括值）。可用于检测是否存在异常的自定义头部，或检测头部名称本身是否包含攻击载荷。


**Example:**


```apache
# 检测头部名称中的异常字符
SecRule REQUEST_HEADERS_NAMES "@rx [<>'\"]" \
    "id:1012,phase:1,deny,msg:'请求头名称包含非法字符'"
```


#### REQUEST_COOKIES


> **Description:** 请求 Cookie


**Syntax:** `REQUEST_COOKIES | REQUEST_COOKIES:CookieName`


REQUEST_COOKIES 包含请求中所有 Cookie 的值。可以使用 REQUEST_COOKIES:name 访问特定 Cookie。Cookie 是常见的攻击载体，攻击者可能在 Cookie 中注入恶意代码或尝试会话劫持。


**Example:**


```apache
# 检查所有 Cookie 中的 SQL 注入
SecRule REQUEST_COOKIES "@detectSQLi" \
    "id:1013,phase:1,deny,msg:'Cookie 检测到 SQL 注入'"

# 验证会话 Cookie 格式
SecRule REQUEST_COOKIES:SESSIONID "!@rx ^[a-zA-Z0-9]{32}$" \
    "id:1014,phase:1,deny,msg:'无效的会话 Cookie 格式'"
```


#### REQUEST_COOKIES_NAMES


> **Description:** Cookie 名称


**Syntax:** `REQUEST_COOKIES_NAMES`


REQUEST_COOKIES_NAMES 包含所有 Cookie 的名称。可用于检测 Cookie 名称是否包含异常字符或攻击载荷。


**Example:**


```apache
# 检测 Cookie 名称中的异常字符
SecRule REQUEST_COOKIES_NAMES "@rx [<>'\";()]" \
    "id:1015,phase:1,deny,msg:'Cookie 名称包含非法字符'"
```


#### REQUEST_BODY


> **Description:** 请求体内容


**Syntax:** `REQUEST_BODY`


REQUEST_BODY 包含原始的请求体数据。与 ARGS_POST 不同，REQUEST_BODY 是未解析的原始数据。当请求体处理器无法解析内容时（如未知的 Content-Type），可以使用此变量检查原始数据。需要启用 SecRequestBodyAccess 才能访问此变量。


**Example:**


```apache
# 检查原始请求体中的恶意模式
SecRule REQUEST_BODY "@rx 


#### REQUEST_BODY_LENGTH


> **Description:** 请求体长度


**Syntax:** `REQUEST_BODY_LENGTH`


**Example:**


```apache
SecRule REQUEST_BODY_LENGTH "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### REQUEST_METHOD


> **Description:** 请求方法 (GET, POST 等)


**Syntax:** `REQUEST_METHOD`


REQUEST_METHOD 包含 HTTP 请求方法，如 GET、POST、PUT、DELETE、HEAD、OPTIONS 等。可用于限制特定端点允许的请求方法，或检测异常的 HTTP 方法。


**Example:**


```apache
# 仅允许 GET 和 POST 方法
SecRule REQUEST_METHOD "!@pm GET POST HEAD" \
    "id:1018,phase:1,deny,msg:'不允许的 HTTP 方法'"

# 阻止 TRACE 方法（防止 XST 攻击）
SecRule REQUEST_METHOD "@streq TRACE" \
    "id:1019,phase:1,deny,msg:'TRACE 方法被禁止'"
```


#### REQUEST_URI


> **Description:** 请求 URI (已解码)


**Syntax:** `REQUEST_URI`


REQUEST_URI 包含完整的请求 URI，包括查询字符串部分，且已经过 URL 解码。这是检测 URL 中攻击载荷的主要变量。与 REQUEST_URI_RAW 不同，此变量的内容已解码，便于直接匹配攻击模式。


**Example:**


```apache
# 检测 URL 中的目录遍历攻击
SecRule REQUEST_URI "@rx \.\." \
    "id:1020,phase:1,deny,msg:'检测到目录遍历攻击'"

# 阻止访问敏感文件
SecRule REQUEST_URI "@rx \.(conf|ini|log|bak)$" \
    "id:1021,phase:1,deny,msg:'尝试访问敏感文件'"
```


#### REQUEST_URI_RAW


> **Description:** 原始请求 URI (未解码)


**Syntax:** `REQUEST_URI_RAW`


REQUEST_URI_RAW 包含原始的、未经 URL 解码的请求 URI。用于检测编码绕过攻击，攻击者可能使用多重编码或异常编码来绕过 WAF 检测。建议同时检查 REQUEST_URI 和 REQUEST_URI_RAW 以获得更全面的保护。


**Example:**


```apache
# 检测 URL 中的双重编码攻击
SecRule REQUEST_URI_RAW "@rx %25" \
    "id:1022,phase:1,deny,msg:'检测到双重编码'"

# 检测 NULL 字节注入
SecRule REQUEST_URI_RAW "@rx %00" \
    "id:1023,phase:1,deny,msg:'检测到 NULL 字节注入'"
```


#### REQUEST_FILENAME


> **Description:** 请求的文件路径


**Syntax:** `REQUEST_FILENAME`


REQUEST_FILENAME 包含请求 URI 中的文件路径部分，不包括查询字符串。这是从 URI 中提取的规范化路径，可用于检测对特定文件或目录的访问。


**Example:**


```apache
# 阻止访问管理目录
SecRule REQUEST_FILENAME "@beginsWith /admin" \
    "id:1024,phase:1,deny,msg:'禁止访问管理目录'"

# 阻止访问 PHP 配置文件
SecRule REQUEST_FILENAME "@endsWith .htaccess" \
    "id:1025,phase:1,deny,msg:'禁止访问 .htaccess'"
```


#### REQUEST_BASENAME


> **Description:** 请求文件的基础名称


**Syntax:** `REQUEST_BASENAME`


REQUEST_BASENAME 仅包含请求路径中的文件名部分，不包括目录路径。例如，对于 /path/to/file.php，此变量的值为 file.php。用于检测特定文件名或文件扩展名。


**Example:**


```apache
# 阻止访问备份文件
SecRule REQUEST_BASENAME "@rx \.(bak|backup|old|orig)$" \
    "id:1026,phase:1,deny,msg:'禁止访问备份文件'"

# 阻止执行 shell 脚本
SecRule REQUEST_BASENAME "@endsWith .sh" \
    "id:1027,phase:1,deny,msg:'禁止执行 shell 脚本'"
```


#### REQUEST_PROTOCOL


> **Description:** 请求协议 (HTTP/1.1 等)


**Syntax:** `REQUEST_PROTOCOL`


REQUEST_PROTOCOL 包含请求使用的 HTTP 协议版本，如 HTTP/1.0、HTTP/1.1 或 HTTP/2.0。可用于检测协议违规或强制使用特定协议版本。


**Example:**


```apache
# 拒绝 HTTP/1.0 请求
SecRule REQUEST_PROTOCOL "@streq HTTP/1.0" \
    "id:1028,phase:1,deny,msg:'不支持 HTTP/1.0'"

# 检测异常协议
SecRule REQUEST_PROTOCOL "!@rx ^HTTP/[12]\.[0-9]$" \
    "id:1029,phase:1,deny,msg:'无效的 HTTP 协议'"
```


#### REQUEST_LINE


> **Description:** 完整的请求行


**Syntax:** `REQUEST_LINE`


REQUEST_LINE 包含完整的 HTTP 请求行，格式为 "METHOD URI PROTOCOL"，例如 "GET /index.html HTTP/1.1"。可用于全面检查请求行中的异常内容。


**Example:**


```apache
# 检测请求行中的控制字符
SecRule REQUEST_LINE "@rx [\x00-\x08\x0b\x0c\x0e-\x1f]" \
    "id:1030,phase:1,deny,msg:'请求行包含控制字符'"

# 检测超长请求行
SecRule REQUEST_LINE "@gt 8192" "t:length,id:1031,phase:1,deny,msg:'请求行过长'"
```


#### QUERY_STRING


> **Description:** 查询字符串


**Syntax:** `QUERY_STRING`


QUERY_STRING 包含 URL 中问号 (?) 后面的查询字符串部分，不包括问号本身。这是原始的查询字符串，未经解析为单独的参数。可用于检测整体查询字符串的异常模式。


**Example:**


```apache
# 检测查询字符串中的 SQL 注入
SecRule QUERY_STRING "@detectSQLi" \
    "id:1032,phase:1,deny,msg:'查询字符串检测到 SQL 注入'"

# 限制查询字符串长度
SecRule QUERY_STRING "@gt 2048" "t:length,id:1033,phase:1,deny,msg:'查询字符串过长'"
```


#### FULL_REQUEST


> **Description:** 完整的请求数据


**Syntax:** `FULL_REQUEST`


**Example:**


```apache
SecRule FULL_REQUEST "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### FULL_REQUEST_LENGTH


> **Description:** 完整请求的长度


**Syntax:** `FULL_REQUEST_LENGTH`


**Example:**


```apache
SecRule FULL_REQUEST_LENGTH "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


### 响应变量


| Name | Description |
|---|---|
| [`RESPONSE_BODY`](#response_body) | 响应体内容 |
| [`RESPONSE_HEADERS`](#response_headers) | 响应头 |
| [`RESPONSE_HEADERS_NAMES`](#response_headers_names) | 响应头名称 |
| [`RESPONSE_STATUS`](#response_status) | 响应状态码 |
| [`RESPONSE_PROTOCOL`](#response_protocol) | 响应协议 |
| [`RESPONSE_CONTENT_LENGTH`](#response_content_length) | 响应内容长度 |
| [`RESPONSE_CONTENT_TYPE`](#response_content_type) | 响应内容类型 |


#### RESPONSE_BODY


> **Description:** 响应体内容


**Syntax:** `RESPONSE_BODY`


RESPONSE_BODY 包含服务器返回的响应体内容。需要启用 SecResponseBodyAccess 才能访问此变量。可用于检测响应中的敏感信息泄露、恶意内容或异常模式。在 phase:4 (响应体阶段) 中使用。


**Example:**


```apache
# 检测响应中的信用卡号泄露
SecRule RESPONSE_BODY "@rx \b(?:\d{4}[-\s]?){3}\d{4}\b" \
    "id:1034,phase:4,deny,msg:'检测到信用卡号泄露'"

# 检测 SQL 错误信息泄露
SecRule RESPONSE_BODY "@pm mysql_error ora-00 sql syntax error" \
    "id:1035,phase:4,deny,msg:'检测到数据库错误信息泄露'"
```


#### RESPONSE_HEADERS


> **Description:** 响应头


**Syntax:** `RESPONSE_HEADERS | RESPONSE_HEADERS:Key`


**Example:**


```apache
SecRule RESPONSE_HEADERS:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### RESPONSE_HEADERS_NAMES


> **Description:** 响应头名称


**Syntax:** `RESPONSE_HEADERS_NAMES | RESPONSE_HEADERS_NAMES:Key`


**Example:**


```apache
SecRule RESPONSE_HEADERS_NAMES:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### RESPONSE_STATUS


> **Description:** 响应状态码


**Syntax:** `RESPONSE_STATUS`


RESPONSE_STATUS 包含服务器返回的 HTTP 状态码，如 200、404、500 等。可用于检测异常响应或基于状态码的安全策略。在 phase:3 (响应头阶段) 或之后可用。


**Example:**


```apache
# 记录所有 5xx 错误
SecRule RESPONSE_STATUS "@rx ^5" \
    "id:1036,phase:3,pass,log,msg:'服务器错误: %{RESPONSE_STATUS}'"

# 检测信息泄露（某些应用返回 500 可能包含调试信息）
SecRule RESPONSE_STATUS "@eq 500" \
    "id:1037,phase:3,pass,log,msg:'检测到服务器内部错误'"
```


#### RESPONSE_PROTOCOL


> **Description:** 响应协议


**Syntax:** `RESPONSE_PROTOCOL`


**Example:**


```apache
SecRule RESPONSE_PROTOCOL "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### RESPONSE_CONTENT_LENGTH


> **Description:** 响应内容长度


**Syntax:** `RESPONSE_CONTENT_LENGTH`


**Example:**


```apache
SecRule RESPONSE_CONTENT_LENGTH "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### RESPONSE_CONTENT_TYPE


> **Description:** 响应内容类型


**Syntax:** `RESPONSE_CONTENT_TYPE`


**Example:**


```apache
SecRule RESPONSE_CONTENT_TYPE "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


### 服务器变量


| Name | Description |
|---|---|
| [`REMOTE_ADDR`](#remote_addr) | 客户端 IP 地址 |
| [`REMOTE_HOST`](#remote_host) | 客户端主机名 |
| [`REMOTE_PORT`](#remote_port) | 客户端端口 |
| [`REMOTE_USER`](#remote_user) | 认证用户名 |
| [`SERVER_ADDR`](#server_addr) | 服务器 IP 地址 |
| [`SERVER_NAME`](#server_name) | 服务器名称 |
| [`SERVER_PORT`](#server_port) | 服务器端口 |
| [`AUTH_TYPE`](#auth_type) | 认证类型 |
| [`PATH_INFO`](#path_info) | 路径信息 |


#### REMOTE_ADDR


> **Description:** 客户端 IP 地址


**Syntax:** `REMOTE_ADDR`


REMOTE_ADDR 包含发起请求的客户端 IP 地址。这是最常用的变量之一，用于实现 IP 黑白名单、访问频率控制等安全策略。常与 @ipMatch 操作符配合使用来匹配 IP 地址或 CIDR 网段。


**Example:**


```apache
# IP 白名单 - 跳过内网 IP 的检查
SecRule REMOTE_ADDR "@ipMatch 10.0.0.0/8,172.16.0.0/12,192.168.0.0/16" \
    "id:1038,phase:1,pass,nolog,allow"

# IP 黑名单 - 阻止特定 IP
SecRule REMOTE_ADDR "@ipMatch 1.2.3.4,5.6.7.8" \
    "id:1039,phase:1,deny,status:403,msg:'IP 已被封禁'"
```


#### REMOTE_HOST


> **Description:** 客户端主机名


**Syntax:** `REMOTE_HOST`


**Example:**


```apache
SecRule REMOTE_HOST "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### REMOTE_PORT


> **Description:** 客户端端口


**Syntax:** `REMOTE_PORT`


**Example:**


```apache
SecRule REMOTE_PORT "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### REMOTE_USER


> **Description:** 认证用户名


**Syntax:** `REMOTE_USER`


**Example:**


```apache
SecRule REMOTE_USER "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### SERVER_ADDR


> **Description:** 服务器 IP 地址


**Syntax:** `SERVER_ADDR`


**Example:**


```apache
SecRule SERVER_ADDR "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### SERVER_NAME


> **Description:** 服务器名称


**Syntax:** `SERVER_NAME`


**Example:**


```apache
SecRule SERVER_NAME "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### SERVER_PORT


> **Description:** 服务器端口


**Syntax:** `SERVER_PORT`


**Example:**


```apache
SecRule SERVER_PORT "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### AUTH_TYPE


> **Description:** 认证类型


**Syntax:** `AUTH_TYPE`


**Example:**


```apache
SecRule AUTH_TYPE "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### PATH_INFO


> **Description:** 路径信息


**Syntax:** `PATH_INFO`


PATH_INFO 包含 URL 中文件路径之后、查询字符串之前的附加路径信息。例如，对于 /cgi-bin/script.py/extra/path，PATH_INFO 的值为 /extra/path。在 RESTful API 中，此变量可能包含路由参数。


**Example:**


```apache
# 检测 PATH_INFO 中的目录遍历
SecRule PATH_INFO "@contains .." \
    "id:1040,phase:1,deny,msg:'PATH_INFO 检测到目录遍历'"
```


### Multipart 变量


| Name | Description |
|---|---|
| [`MULTIPART_FILENAME`](#multipart_filename) | Multipart 文件名 |
| [`MULTIPART_NAME`](#multipart_name) | Multipart 字段名 |
| [`MULTIPART_PART_HEADERS`](#multipart_part_headers) | Multipart 部分头 |
| [`MULTIPART_CRLF_LF_LINES`](#multipart_crlf_lf_lines) | Multipart CRLF/LF 行数 |
| [`MULTIPART_STRICT_ERROR`](#multipart_strict_error) | Multipart 严格错误标志 |
| [`MULTIPART_UNMATCHED_BOUNDARY`](#multipart_unmatched_boundary) | Multipart 不匹配的边界 |
| [`MULTIPART_BOUNDARY_QUOTED`](#multipart_boundary_quoted) | Multipart 边界引号错误 |
| [`MULTIPART_BOUNDARY_WHITESPACE`](#multipart_boundary_whitespace) | Multipart 边界空白错误 |
| [`MULTIPART_DATA_BEFORE`](#multipart_data_before) | Multipart 边界前的数据 |
| [`MULTIPART_DATA_AFTER`](#multipart_data_after) | Multipart 边界后的数据 |
| [`MULTIPART_HEADER_FOLDING`](#multipart_header_folding) | Multipart 头折叠 |
| [`MULTIPART_LF_LINE`](#multipart_lf_line) | Multipart LF 行 |
| [`MULTIPART_MISSING_SEMICOLON`](#multipart_missing_semicolon) | Multipart 缺少分号 |
| [`MULTIPART_INVALID_QUOTING`](#multipart_invalid_quoting) | Multipart 无效引号 |
| [`MULTIPART_INVALID_PART`](#multipart_invalid_part) | Multipart 无效部分 |
| [`MULTIPART_INVALID_HEADER_FOLDING`](#multipart_invalid_header_folding) | Multipart 无效头折叠 |
| [`MULTIPART_FILE_LIMIT_EXCEEDED`](#multipart_file_limit_exceeded) | Multipart 文件数量超限 |


#### MULTIPART_FILENAME


> **Description:** Multipart 文件名


**Syntax:** `MULTIPART_FILENAME`


**Example:**


```apache
SecRule MULTIPART_FILENAME "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_NAME


> **Description:** Multipart 字段名


**Syntax:** `MULTIPART_NAME`


**Example:**


```apache
SecRule MULTIPART_NAME "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_PART_HEADERS


> **Description:** Multipart 部分头


**Syntax:** `MULTIPART_PART_HEADERS | MULTIPART_PART_HEADERS:HeaderName`


MULTIPART_PART_HEADERS 包含 multipart/form-data 请求中各部分的头信息。可用于检测上传文件部分的 Content-Type、Content-Disposition 等头信息中的异常内容。


**Example:**


```apache
# 检测 multipart 头中的异常 Content-Type
SecRule MULTIPART_PART_HEADERS "@rx application/(php|x-php|x-httpd-php)" \
    "id:1041,phase:2,deny,msg:'上传文件 Content-Type 异常'"
```


#### MULTIPART_CRLF_LF_LINES


> **Description:** Multipart CRLF/LF 行数


**Syntax:** `MULTIPART_CRLF_LF_LINES`


**Example:**


```apache
SecRule MULTIPART_CRLF_LF_LINES "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_STRICT_ERROR


> **Description:** Multipart 严格错误标志


**Syntax:** `MULTIPART_STRICT_ERROR`


MULTIPART_STRICT_ERROR 是一个复合标志变量，当 multipart 请求体解析过程中检测到任何严格模式违规时设置为 1。它综合了多个 multipart 错误标志（如 MULTIPART_UNMATCHED_BOUNDARY 等），用于快速判断 multipart 请求是否存在异常。这是防范文件上传绕过攻击的重要检查点。


**Example:**


```apache
# 拒绝存在 multipart 解析错误的请求
SecRule MULTIPART_STRICT_ERROR "!@eq 0" \
    "id:1042,phase:2,deny,status:403,msg:'Multipart 请求解析异常'"
```


#### MULTIPART_UNMATCHED_BOUNDARY


> **Description:** Multipart 不匹配的边界


**Syntax:** `MULTIPART_UNMATCHED_BOUNDARY`


MULTIPART_UNMATCHED_BOUNDARY 当 multipart 请求体中出现未匹配的边界标记时设置为 1。不匹配的边界通常表明请求被篡改或构造异常，可能是攻击者尝试绕过上传文件检测的手段。


**Example:**


```apache
# 拒绝包含不匹配边界的 multipart 请求
SecRule MULTIPART_UNMATCHED_BOUNDARY "!@eq 0" \
    "id:1043,phase:2,deny,status:403,msg:'Multipart 边界不匹配'"
```


#### MULTIPART_BOUNDARY_QUOTED


> **Description:** Multipart 边界引号错误


**Syntax:** `MULTIPART_BOUNDARY_QUOTED`


**Example:**


```apache
SecRule MULTIPART_BOUNDARY_QUOTED "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_BOUNDARY_WHITESPACE


> **Description:** Multipart 边界空白错误


**Syntax:** `MULTIPART_BOUNDARY_WHITESPACE`


**Example:**


```apache
SecRule MULTIPART_BOUNDARY_WHITESPACE "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_DATA_BEFORE


> **Description:** Multipart 边界前的数据


**Syntax:** `MULTIPART_DATA_BEFORE`


**Example:**


```apache
SecRule MULTIPART_DATA_BEFORE "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_DATA_AFTER


> **Description:** Multipart 边界后的数据


**Syntax:** `MULTIPART_DATA_AFTER`


**Example:**


```apache
SecRule MULTIPART_DATA_AFTER "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_HEADER_FOLDING


> **Description:** Multipart 头折叠


**Syntax:** `MULTIPART_HEADER_FOLDING`


**Example:**


```apache
SecRule MULTIPART_HEADER_FOLDING "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_LF_LINE


> **Description:** Multipart LF 行


**Syntax:** `MULTIPART_LF_LINE`


**Example:**


```apache
SecRule MULTIPART_LF_LINE "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_MISSING_SEMICOLON


> **Description:** Multipart 缺少分号


**Syntax:** `MULTIPART_MISSING_SEMICOLON`


**Example:**


```apache
SecRule MULTIPART_MISSING_SEMICOLON "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_INVALID_QUOTING


> **Description:** Multipart 无效引号


**Syntax:** `MULTIPART_INVALID_QUOTING`


**Example:**


```apache
SecRule MULTIPART_INVALID_QUOTING "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_INVALID_PART


> **Description:** Multipart 无效部分


**Syntax:** `MULTIPART_INVALID_PART`


**Example:**


```apache
SecRule MULTIPART_INVALID_PART "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_INVALID_HEADER_FOLDING


> **Description:** Multipart 无效头折叠


**Syntax:** `MULTIPART_INVALID_HEADER_FOLDING`


**Example:**


```apache
SecRule MULTIPART_INVALID_HEADER_FOLDING "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MULTIPART_FILE_LIMIT_EXCEEDED


> **Description:** Multipart 文件数量超限


**Syntax:** `MULTIPART_FILE_LIMIT_EXCEEDED`


**Example:**


```apache
SecRule MULTIPART_FILE_LIMIT_EXCEEDED "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


### 错误和状态变量


| Name | Description |
|---|---|
| [`REQBODY_ERROR`](#reqbody_error) | 请求体错误标志 |
| [`REQBODY_ERROR_MSG`](#reqbody_error_msg) | 请求体错误消息 |
| [`REQBODY_PROCESSOR`](#reqbody_processor) | 请求体处理器类型 |
| [`REQBODY_PROCESSOR_ERROR`](#reqbody_processor_error) | 请求体处理器错误 |
| [`INBOUND_DATA_ERROR`](#inbound_data_error) | 入站数据错误 |
| [`OUTBOUND_DATA_ERROR`](#outbound_data_error) | 出站数据错误 |
| [`URLENCODED_ERROR`](#urlencoded_error) | URL 编码错误 |
| [`STATUS_LINE`](#status_line) | HTTP 状态行 |
| [`HIGHEST_SEVERITY`](#highest_severity) | 最高严重级别 |
| [`MSC_PCRE_LIMITS_EXCEEDED`](#msc_pcre_limits_exceeded) | PCRE 限制超出标志 |


#### REQBODY_ERROR


> **Description:** 请求体错误标志


**Syntax:** `REQBODY_ERROR`


REQBODY_ERROR 当请求体解析过程中出现错误时设置为 1。例如，当 JSON 格式无效、XML 格式错误或 multipart 数据异常时，此变量会被设置。可用于在 phase:2 中检测并阻止包含无效请求体的请求。


**Example:**


```apache
# 拒绝请求体解析失败的请求
SecRule REQBODY_ERROR "!@eq 0" \
    "id:1044,phase:2,deny,status:400,msg:'请求体解析失败: %{REQBODY_ERROR_MSG}'"
```


#### REQBODY_ERROR_MSG


> **Description:** 请求体错误消息


**Syntax:** `REQBODY_ERROR_MSG`


REQBODY_ERROR_MSG 包含请求体解析失败时的错误描述信息。通常与 REQBODY_ERROR 配合使用，在日志中记录具体的错误原因，便于调试和分析。可通过 %{REQBODY_ERROR_MSG} 在 msg 动作中引用。


**Example:**


```apache
# 记录请求体解析错误的详细信息
SecRule REQBODY_ERROR "!@eq 0" \
    "id:1045,phase:2,pass,log,msg:'请求体错误: %{REQBODY_ERROR_MSG}'"
```


#### REQBODY_PROCESSOR


> **Description:** 请求体处理器类型


**Syntax:** `REQBODY_PROCESSOR`


REQBODY_PROCESSOR 包含当前请求使用的请求体处理器名称，如 URLENCODED、MULTIPART、JSON 或 XML。此变量由 WGE 根据 Content-Type 自动设置，也可通过 ctl:requestBodyProcessor 动作手动指定。


**Example:**


```apache
# 根据处理器类型执行不同检查
SecRule REQBODY_PROCESSOR "@streq JSON" \
    "id:1046,phase:2,pass,nolog,setvar:tx.json_request=1"
```


#### REQBODY_PROCESSOR_ERROR


> **Description:** 请求体处理器错误


**Syntax:** `REQBODY_PROCESSOR_ERROR`


**Example:**


```apache
SecRule REQBODY_PROCESSOR_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### INBOUND_DATA_ERROR


> **Description:** 入站数据错误


**Syntax:** `INBOUND_DATA_ERROR`


**Example:**


```apache
SecRule INBOUND_DATA_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### OUTBOUND_DATA_ERROR


> **Description:** 出站数据错误


**Syntax:** `OUTBOUND_DATA_ERROR`


**Example:**


```apache
SecRule OUTBOUND_DATA_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### URLENCODED_ERROR


> **Description:** URL 编码错误


**Syntax:** `URLENCODED_ERROR`


**Example:**


```apache
SecRule URLENCODED_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### STATUS_LINE


> **Description:** HTTP 状态行


**Syntax:** `STATUS_LINE`


**Example:**


```apache
SecRule STATUS_LINE "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### HIGHEST_SEVERITY


> **Description:** 最高严重级别


**Syntax:** `HIGHEST_SEVERITY`


**Example:**


```apache
SecRule HIGHEST_SEVERITY "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### MSC_PCRE_LIMITS_EXCEEDED


> **Description:** PCRE 限制超出标志


**Syntax:** `MSC_PCRE_LIMITS_EXCEEDED`


**Example:**


```apache
SecRule MSC_PCRE_LIMITS_EXCEEDED "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


### 其他变量


| Name | Description |
|---|---|
| [`ENV`](#env) | 环境变量集合 |
| [`GEO`](#geo) | 地理位置信息 |
| [`RULE`](#rule) | 当前规则信息 |
| [`SESSIONID`](#sessionid) | 会话 ID |
| [`USERID`](#userid) | 用户 ID |
| [`WEBAPPID`](#webappid) | Web 应用 ID |
| [`XML`](#xml) | XML 数据 |
| [`MODSEC_BUILD`](#modsec_build) | ModSecurity 构建版本 |


#### ENV


> **Description:** 环境变量集合


**Syntax:** `ENV`


**Example:**


```apache
SecRule ENV "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### GEO


> **Description:** 地理位置信息


**Syntax:** `GEO`


**Example:**


```apache
SecRule GEO "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### RULE


> **Description:** 当前规则信息


**Syntax:** `RULE`


RULE 变量提供对当前执行规则元数据的访问。可以通过子键访问具体属性，如 RULE.id（规则ID）、RULE.msg（规则消息）、RULE.severity（严重级别）等。主要用于在日志记录和动态消息中引用规则自身的信息。


**Example:**


```apache
# 在日志中引用规则信息
SecRule ARGS "@detectSQLi" \
    "id:1047,phase:2,deny,msg:'规则 %{RULE.id} 触发: SQL 注入检测',\
    severity:CRITICAL,tag:'attack-sqli'"
```


#### SESSIONID


> **Description:** 会话 ID


**Syntax:** `SESSIONID`


**Example:**


```apache
SecRule SESSIONID "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### USERID


> **Description:** 用户 ID


**Syntax:** `USERID`


**Example:**


```apache
SecRule USERID "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### WEBAPPID


> **Description:** Web 应用 ID


**Syntax:** `WEBAPPID`


**Example:**


```apache
SecRule WEBAPPID "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### XML


> **Description:** XML 数据


**Syntax:** `XML:/xpath/expression`


XML 变量用于通过 XPath 表达式访问 XML 请求体中的特定节点。需要先通过 ctl:requestBodyProcessor=XML 启用 XML 解析器。支持完整的 XPath 语法，包括属性访问、文本节点提取等。


**Example:**


```apache
# 启用 XML 解析
SecRule REQUEST_HEADERS:Content-Type "@contains xml" \
    "id:1048,phase:1,pass,nolog,ctl:requestBodyProcessor=XML"

# 使用 XPath 检查 XML 节点
SecRule XML:/root/user/name/text() "@detectSQLi" \
    "id:1049,phase:2,deny,msg:'XML 节点检测到 SQL 注入'"
```


#### MODSEC_BUILD


> **Description:** ModSecurity 构建版本


**Syntax:** `MODSEC_BUILD`


**Example:**


```apache
SecRule MODSEC_BUILD "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


### 文件上传变量


| Name | Description |
|---|---|
| [`FILES`](#files) | 上传文件内容 |
| [`FILES_NAMES`](#files_names) | 上传文件名称 |
| [`FILES_SIZES`](#files_sizes) | 上传文件大小 |
| [`FILES_TMPNAMES`](#files_tmpnames) | 上传文件临时名称 |
| [`FILES_TMP_CONTENT`](#files_tmp_content) | 上传文件临时内容 |
| [`FILES_COMBINED_SIZE`](#files_combined_size) | 所有上传文件的总大小 |


#### FILES


> **Description:** 上传文件内容


**Syntax:** `FILES | FILES:fieldname`


FILES 包含通过 multipart/form-data 上传的文件的原始文件名。可通过 FILES:fieldname 访问特定表单字段中上传的文件。此变量是文件上传安全检查的核心，可用于验证文件扩展名、文件名格式等。


**Example:**


```apache
# 检查上传文件的危险扩展名
SecRule FILES "@rx \.(php|jsp|asp|exe|sh|bat)$" \
    "id:1050,phase:2,deny,status:403,msg:'禁止上传此类型文件'"
```


#### FILES_NAMES


> **Description:** 上传文件名称


**Syntax:** `FILES_NAMES`


FILES_NAMES 包含所有上传文件对应的表单字段名称。可用于检测表单字段名中的异常内容，或验证上传文件是否通过预期的表单字段提交。


**Example:**


```apache
# 确保文件仅通过 'avatar' 字段上传
SecRule FILES_NAMES "!@streq avatar" \
    "id:1051,phase:2,deny,msg:'非法的文件上传字段'"
```


#### FILES_SIZES


> **Description:** 上传文件大小


**Syntax:** `FILES_SIZES`


**Example:**


```apache
SecRule FILES_SIZES "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### FILES_TMPNAMES


> **Description:** 上传文件临时名称


**Syntax:** `FILES_TMPNAMES`


**Example:**


```apache
SecRule FILES_TMPNAMES "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### FILES_TMP_CONTENT


> **Description:** 上传文件临时内容


**Syntax:** `FILES_TMP_CONTENT`


**Example:**


```apache
SecRule FILES_TMP_CONTENT "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### FILES_COMBINED_SIZE


> **Description:** 所有上传文件的总大小


**Syntax:** `FILES_COMBINED_SIZE`


**Example:**


```apache
SecRule FILES_COMBINED_SIZE "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


### 特殊变量


| Name | Description |
|---|---|
| [`TX`](#tx) | 事务变量集合 (临时变量) |
| [`GLOBAL`](#global) | 全局变量集合 |
| [`IP`](#ip) | IP 地址集合 |
| [`SESSION`](#session) | 会话变量集合 |
| [`USER`](#user) | 用户变量集合 |
| [`RESOURCE`](#resource) | 资源变量集合 |
| [`MATCHED_VAR`](#matched_var) | 最后匹配的变量值 |
| [`MATCHED_VAR_NAME`](#matched_var_name) | 最后匹配的变量名 |
| [`MATCHED_VARS`](#matched_vars) | 所有匹配的变量值 |
| [`MATCHED_VARS_NAMES`](#matched_vars_names) | 所有匹配的变量名 |
| [`UNIQUE_ID`](#unique_id) | 唯一请求标识符 |
| [`DURATION`](#duration) | 事务处理时长 |


#### TX


> **Description:** 事务变量集合 (临时变量)


**Syntax:** `TX:key`


TX (Transaction) 是最常用的集合变量，用于在单个事务（请求-响应周期）内存储和传递数据。TX 变量在事务结束后自动销毁。通过 setvar 动作设置，通过 TX:key 访问。TX:0 到 TX:9 为特殊变量，自动存储 @rx 操作符的正则捕获组结果。此外，TX 变量常用于实现异常评分模式（如 tx.anomaly_score）。


**Example:**


```apache
# 设置异常评分
SecRule ARGS "@detectSQLi" \
    "id:1052,phase:2,pass,setvar:tx.anomaly_score=+5,msg:'SQL 注入加分'"

# 在评分累计后进行阻断判断
SecRule TX:anomaly_score "@ge 10" \
    "id:1053,phase:2,deny,msg:'异常评分超过阈值: %{TX.anomaly_score}'"

# 使用正则捕获组
SecRule REQUEST_URI "@rx ^/user/(\d+)" \
    "id:1054,phase:1,pass,setvar:tx.user_id=%{TX.1}"
```


#### GLOBAL


> **Description:** 全局变量集合


**Syntax:** `GLOBAL:key`


GLOBAL 集合用于存储跨事务的全局持久化数据。与 TX 不同，GLOBAL 变量在多个请求之间保持，可用于实现全局计数器、全局状态标志等。需要通过 initcol 动作初始化后才能使用。


**Example:**


```apache
# 初始化全局集合
SecAction "id:1055,phase:1,pass,nolog,initcol:GLOBAL=global"

# 全局请求计数
SecRule REQUEST_URI "@rx ." \
    "id:1056,phase:1,pass,nolog,setvar:GLOBAL.request_count=+1"
```


#### IP


> **Description:** IP 地址集合


**Syntax:** `IP:key`


IP 集合用于存储与客户端 IP 地址关联的持久化数据。常用于实现 IP 级别的访问频率限制、IP 信誉评分等。需要通过 initcol:IP=%{REMOTE_ADDR} 初始化。IP 集合中的数据在同一客户端 IP 的多次请求间保持。


**Example:**


```apache
# 初始化 IP 集合
SecAction "id:1057,phase:1,pass,nolog,initcol:IP=%{REMOTE_ADDR}"

# IP 级别的请求频率限制
SecRule IP:request_count "@gt 100" \
    "id:1058,phase:1,deny,msg:'IP 请求频率超限'"
SecRule REQUEST_URI "@rx ." \
    "id:1059,phase:1,pass,nolog,setvar:IP.request_count=+1,\
    expirevar:IP.request_count=60"
```


#### SESSION


> **Description:** 会话变量集合


**Syntax:** `SESSION:key`


SESSION 集合用于存储与用户会话关联的持久化数据。需要先通过 setsid 动作设置会话标识符，然后通过 initcol:SESSION=%{SESSIONID} 初始化。可用于跟踪会话级别的行为、实现会话级访问控制等。


**Example:**


```apache
# 设置会话 ID 并初始化
SecRule REQUEST_COOKIES:PHPSESSID "@rx (.+)" \
    "id:1060,phase:1,pass,nolog,setsid:%{TX.1},initcol:SESSION=%{SESSIONID}"
```


#### USER


> **Description:** 用户变量集合


**Syntax:** `USER:key`


USER 集合用于存储与认证用户关联的持久化数据。需要先通过 setuid 动作设置用户标识符并初始化。可用于跟踪特定用户的行为模式、实现用户级别的访问控制。


**Example:**


```apache
# 设置用户 ID
SecRule ARGS_POST:username "@rx (.+)" \
    "id:1061,phase:2,pass,nolog,setuid:%{TX.1}"
```


#### RESOURCE


> **Description:** 资源变量集合


**Syntax:** `RESOURCE:key`


RESOURCE 集合用于存储与特定资源（如 URL 路径）关联的持久化数据。可用于跟踪对特定资源的访问模式，如特定 API 端点的调用频率。


**Example:**


```apache
# 初始化资源集合
SecAction "id:1062,phase:1,pass,nolog,initcol:RESOURCE=%{REQUEST_FILENAME}"
```


#### MATCHED_VAR


> **Description:** 最后匹配的变量值


**Syntax:** `MATCHED_VAR`


MATCHED_VAR 包含规则中最后一个匹配成功的变量的值。当规则检查多个变量（如 ARGS）并找到匹配时，此变量保存触发匹配的具体值。常用于日志记录和调试，了解具体什么内容触发了规则。


**Example:**


```apache
# 在日志中记录匹配的内容
SecRule ARGS "@detectSQLi" \
    "id:1063,phase:2,deny,msg:'SQL 注入: %{MATCHED_VAR}'"
```


#### MATCHED_VAR_NAME


> **Description:** 最后匹配的变量名


**Syntax:** `MATCHED_VAR_NAME`


MATCHED_VAR_NAME 包含最后一个匹配成功的变量的完整名称。例如，如果 ARGS:username 触发了规则，MATCHED_VAR_NAME 的值为 "ARGS:username"。与 MATCHED_VAR 配合使用可以完整记录触发规则的位置和内容。


**Example:**


```apache
# 记录匹配的变量名和值
SecRule ARGS "@detectXSS" \
    "id:1064,phase:2,deny,msg:'XSS 在 %{MATCHED_VAR_NAME}: %{MATCHED_VAR}'"
```


#### MATCHED_VARS


> **Description:** 所有匹配的变量值


**Syntax:** `MATCHED_VARS`


MATCHED_VARS 是一个集合变量，包含当前规则所有匹配成功的变量值。与 MATCHED_VAR 不同，当规则匹配多个值时（如多个参数都包含恶意内容），MATCHED_VARS 包含所有这些值。可用于链式规则中进一步检查所有匹配内容。


**Example:**


```apache
# 检查所有匹配值的长度
SecRule ARGS "@rx script" \
    "id:1065,phase:2,pass,setvar:tx.script_found=1,chain"
SecRule MATCHED_VARS "@gt 100" "t:length,deny,msg:'检测到长脚本内容'"
```


#### MATCHED_VARS_NAMES


> **Description:** 所有匹配的变量名


**Syntax:** `MATCHED_VARS_NAMES`


MATCHED_VARS_NAMES 是一个集合变量，包含当前规则所有匹配成功的变量名称。与 MATCHED_VAR_NAME 不同，当规则匹配多个变量时，此变量包含所有匹配变量的名称。


**Example:**


```apache
# 记录所有匹配的变量名
SecRule ARGS|REQUEST_HEADERS "@detectSQLi" \
    "id:1066,phase:2,deny,msg:'SQL 注入在: %{MATCHED_VARS_NAMES}'"
```


#### UNIQUE_ID


> **Description:** 唯一请求标识符


**Syntax:** `UNIQUE_ID`


UNIQUE_ID 包含 WGE 为每个请求生成的唯一标识符。此 ID 在日志中用于关联同一请求的多条记录，便于事后分析和调试。可在 msg 动作中引用以便追踪特定请求。


**Example:**


```apache
# 在日志中记录请求 ID
SecRule ARGS "@detectSQLi" \
    "id:1067,phase:2,deny,msg:'[%{UNIQUE_ID}] SQL 注入攻击'"
```


#### DURATION


> **Description:** 事务处理时长


**Syntax:** `DURATION`


**Example:**


```apache
SecRule DURATION "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


### 时间变量


| Name | Description |
|---|---|
| [`TIME`](#time) | 当前时间 (格式化字符串) |
| [`TIME_EPOCH`](#time_epoch) | Unix 时间戳 |
| [`TIME_YEAR`](#time_year) | 年份 |
| [`TIME_MON`](#time_mon) | 月份 |
| [`TIME_DAY`](#time_day) | 日期 |
| [`TIME_HOUR`](#time_hour) | 小时 |
| [`TIME_MIN`](#time_min) | 分钟 |
| [`TIME_SEC`](#time_sec) | 秒 |
| [`TIME_WDAY`](#time_wday) | 星期几 |


#### TIME


> **Description:** 当前时间 (格式化字符串)


**Syntax:** `TIME`


**Example:**


```apache
SecRule TIME "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### TIME_EPOCH


> **Description:** Unix 时间戳


**Syntax:** `TIME_EPOCH`


**Example:**


```apache
SecRule TIME_EPOCH "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### TIME_YEAR


> **Description:** 年份


**Syntax:** `TIME_YEAR`


**Example:**


```apache
SecRule TIME_YEAR "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### TIME_MON


> **Description:** 月份


**Syntax:** `TIME_MON`


**Example:**


```apache
SecRule TIME_MON "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### TIME_DAY


> **Description:** 日期


**Syntax:** `TIME_DAY`


**Example:**


```apache
SecRule TIME_DAY "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### TIME_HOUR


> **Description:** 小时


**Syntax:** `TIME_HOUR`


**Example:**


```apache
SecRule TIME_HOUR "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### TIME_MIN


> **Description:** 分钟


**Syntax:** `TIME_MIN`


**Example:**


```apache
SecRule TIME_MIN "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### TIME_SEC


> **Description:** 秒


**Syntax:** `TIME_SEC`


**Example:**


```apache
SecRule TIME_SEC "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented


#### TIME_WDAY


> **Description:** 星期几


**Syntax:** `TIME_WDAY`


**Example:**


```apache
SecRule TIME_WDAY "@rx value" "id:1001,deny,msg:'Test'"
```


**Implementation:** Not yet implemented

<script>
(function(){var a="time_sec",done=false;function jump(){if(done)return;var c=document.querySelector('#R-body-inner')||document.documentElement,t=null,els=document.querySelectorAll('[id="'+a+'"]');for(var i=0;i<els.length;i++){if(els[i].tagName==='H3'||els[i].tagName==='H4'){t=els[i];break}}if(t&&c){done=true;c.scrollTop=c.scrollTop+t.getBoundingClientRect().top-c.getBoundingClientRect().top-20}};jump();document.addEventListener('DOMContentLoaded',jump);requestAnimationFrame(jump)})();
</script>
