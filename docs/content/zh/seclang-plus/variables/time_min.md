+++
title = "TIME_MIN"
weight = 99
+++

**描述:** 分钟


**语法:** `TIME_MIN`


**是否实现:** 否

**示例:**


```apache
SecRule TIME_MIN "@rx value" "id:1001,deny,msg:'Test'"
```
