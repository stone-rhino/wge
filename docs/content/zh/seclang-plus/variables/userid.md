+++
title = "USERID"
weight = 71
+++

**描述:** 用户 ID


**语法:** `USERID`


**是否实现:** 否

**示例:**


```apache
SecRule USERID "@rx value" "id:1001,deny,msg:'Test'"
```
