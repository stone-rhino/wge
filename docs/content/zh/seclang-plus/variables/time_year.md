+++
title = "TIME_YEAR"
weight = 95
+++

**描述:** 年份


**语法:** `TIME_YEAR`


**是否实现:** 否

**示例:**


```apache
SecRule TIME_YEAR "@rx value" "id:1001,deny,msg:'Test'"
```
