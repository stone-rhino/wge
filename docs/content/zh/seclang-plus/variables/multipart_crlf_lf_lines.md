+++
title = "MULTIPART_CRLF_LF_LINES"
weight = 43
+++

**描述:** Multipart CRLF/LF 行数


**语法:** `MULTIPART_CRLF_LF_LINES`


**示例:**


```apache
SecRule MULTIPART_CRLF_LF_LINES "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
