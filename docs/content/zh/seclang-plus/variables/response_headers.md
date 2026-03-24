+++
title = "RESPONSE_HEADERS"
weight = 25
+++

**描述:** 响应头

**语法:** `RESPONSE_HEADERS | RESPONSE_HEADERS:Key`

此变量引用响应头的值，与REQUEST_HEADERS请求头的方式相同。

**示例:**

```apache
SecRule RESPONSE_HEADERS:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```
