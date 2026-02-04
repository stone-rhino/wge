+++
title = "MULTIPART_INVALID_HEADER_FOLDING"
weight = 55
+++

**描述:** Multipart 无效头折叠


**语法:** `MULTIPART_INVALID_HEADER_FOLDING`


**示例:**


```apache
SecRule MULTIPART_INVALID_HEADER_FOLDING "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
