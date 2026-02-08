+++
title = "STATUS_LINE"
weight = 64
+++

**描述:** HTTP 状态行


**语法:** `STATUS_LINE`


**是否实现:** 否

**示例:**


```apache
SecRule STATUS_LINE "@rx value" "id:1001,deny,msg:'Test'"
```
