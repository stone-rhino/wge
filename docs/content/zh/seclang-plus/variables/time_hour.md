+++
title = "TIME_HOUR"
weight = 98
+++

**描述:** 小时


**语法:** `TIME_HOUR`


**是否实现:** 否

**示例:**


```apache
SecRule TIME_HOUR "@rx value" "id:1001,deny,msg:'Test'"
```
