+++
title = "TIME"
weight = 93
+++

**描述:** 当前时间 (格式化字符串)


**语法:** `TIME`


**是否实现:** 否

**示例:**


```apache
SecRule TIME "@rx value" "id:1001,deny,msg:'Test'"
```
