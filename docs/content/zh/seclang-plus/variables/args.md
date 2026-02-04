+++
title = "ARGS"
weight = 1
+++

**描述:** 所有请求参数 (GET 和 POST)


**语法:** `ARGS | ARGS:Key`


**示例:**


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
