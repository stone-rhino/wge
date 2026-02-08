+++
title = "TIME_WDAY"
weight = 101
+++

**描述:** 星期几


**语法:** `TIME_WDAY`


**是否实现:** 否

**示例:**


```apache
SecRule TIME_WDAY "@rx value" "id:1001,deny,msg:'Test'"
```
