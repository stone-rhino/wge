+++
title = "TIME_EPOCH"
weight = 94
+++

**描述:** Unix 时间戳


**语法:** `TIME_EPOCH`


**示例:**


```apache
SecRule TIME_EPOCH "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
