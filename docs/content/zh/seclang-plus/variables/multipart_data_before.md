+++
title = "MULTIPART_DATA_BEFORE"
weight = 48
+++

**描述:** Multipart 边界前的数据


**语法:** `MULTIPART_DATA_BEFORE`


**是否实现:** 否

**示例:**


```apache
SecRule MULTIPART_DATA_BEFORE "@rx value" "id:1001,deny,msg:'Test'"
```
