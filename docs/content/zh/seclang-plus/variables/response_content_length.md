+++
title = "RESPONSE_CONTENT_LENGTH"
weight = 29
+++

**描述:** 响应内容长度

**语法:** `RESPONSE_CONTENT_LENGTH`

此变量会返回响应头中的content-length，其等价于变量`RESPONSE_HEADERS:content-length`。

**示例:**

```apache
SecRule RESPONSE_CONTENT_LENGTH "@rx value" "id:1001,deny,msg:'Test'"
```
