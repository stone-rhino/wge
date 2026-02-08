+++
title = "MULTIPART_LF_LINE"
weight = 51
+++

**描述:** Multipart LF 行


**语法:** `MULTIPART_LF_LINE`


**是否实现:** 否

**示例:**


```apache
SecRule MULTIPART_LF_LINE "@rx value" "id:1001,deny,msg:'Test'"
```
