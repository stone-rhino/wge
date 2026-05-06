+++
title = "RESPONSE_PROTOCOL"
weight = 28
+++

**描述:** 响应协议

**语法:** `RESPONSE_PROTOCOL`

此变量包含HTTP响应协议信息

**示例:**

```apache
SecRule RESPONSE_PROTOCOL "@rx value" "id:1001,deny,msg:'Test'"
```
