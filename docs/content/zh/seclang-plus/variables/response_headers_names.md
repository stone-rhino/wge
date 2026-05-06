+++
title = "RESPONSE_HEADERS_NAMES"
weight = 26
+++

**描述:** 响应头名称

**语法:** `RESPONSE_HEADERS_NAMES | RESPONSE_HEADERS_NAMES:Key`

此变量引用响应头的名称，与REQUEST_HEADERS_NAMES请求头的方式相同。

**示例:**

```apache
SecRule RESPONSE_HEADERS_NAMES:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```
