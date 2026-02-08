+++
title = "SESSIONID"
weight = 70
+++

**描述:** 会话 ID


**语法:** `SESSIONID`


**是否实现:** 否

**示例:**


```apache
SecRule SESSIONID "@rx value" "id:1001,deny,msg:'Test'"
```
