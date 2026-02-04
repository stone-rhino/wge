+++
title = "URLENCODED_ERROR"
weight = 63
+++

**描述:** URL 编码错误


**语法:** `URLENCODED_ERROR`


**示例:**


```apache
SecRule URLENCODED_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
