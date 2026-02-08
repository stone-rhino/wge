+++
title = "TIME_SEC"
weight = 100
+++

**描述:** 秒


**语法:** `TIME_SEC`


**是否实现:** 否

**示例:**


```apache
SecRule TIME_SEC "@rx value" "id:1001,deny,msg:'Test'"
```
