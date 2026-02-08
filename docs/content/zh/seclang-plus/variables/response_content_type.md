+++
title = "RESPONSE_CONTENT_TYPE"
weight = 30
+++

**描述:** 响应内容类型


**语法:** `RESPONSE_CONTENT_TYPE`


**是否实现:** 否

**示例:**


```apache
SecRule RESPONSE_CONTENT_TYPE "@rx value" "id:1001,deny,msg:'Test'"
```
