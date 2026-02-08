+++
title = "TIME_EPOCH"
weight = 94
+++

**描述:** Unix 时间戳


**语法:** `TIME_EPOCH`


**是否实现:** 否

**示例:**


```apache
SecRule TIME_EPOCH "@rx value" "id:1001,deny,msg:'Test'"
```
