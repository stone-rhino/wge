+++
title = "RESPONSE_PROTOCOL"
weight = 28
+++

**描述:** 响应协议


**语法:** `RESPONSE_PROTOCOL`


**是否实现:** 否

**示例:**


```apache
SecRule RESPONSE_PROTOCOL "@rx value" "id:1001,deny,msg:'Test'"
```
