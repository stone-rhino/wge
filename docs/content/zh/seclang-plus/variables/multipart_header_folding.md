+++
title = "MULTIPART_HEADER_FOLDING"
weight = 50
+++

**描述:** Multipart 头折叠


**语法:** `MULTIPART_HEADER_FOLDING`


**是否实现:** 否

**示例:**


```apache
SecRule MULTIPART_HEADER_FOLDING "@rx value" "id:1001,deny,msg:'Test'"
```
