+++
title = "MULTIPART_INVALID_PART"
weight = 54
+++

**描述:** Multipart 无效部分


**语法:** `MULTIPART_INVALID_PART`


**是否实现:** 否

**示例:**


```apache
SecRule MULTIPART_INVALID_PART "@rx value" "id:1001,deny,msg:'Test'"
```
