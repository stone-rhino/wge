+++
title = "RESPONSE_CONTENT_TYPE"
weight = 30
+++

**描述:** 响应内容类型

**语法:** `RESPONSE_CONTENT_TYPE`

此变量会返回响应头中的content-type，其等价于变量`RESPONSE_HEADERS:content-type`。

**示例:**

```apache
SecRule RESPONSE_CONTENT_TYPE "@rx value" "id:1001,deny,msg:'Test'"
```
