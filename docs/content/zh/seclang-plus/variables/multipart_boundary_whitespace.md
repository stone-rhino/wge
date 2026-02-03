+++
title = "MULTIPART_BOUNDARY_WHITESPACE"
weight = 47
+++

**描述:** Multipart 边界空白错误


**语法:** `MULTIPART_BOUNDARY_WHITESPACE`


**示例:**


```apache
SecRule MULTIPART_BOUNDARY_WHITESPACE "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
