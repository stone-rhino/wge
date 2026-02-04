+++
title = "MULTIPART_FILE_LIMIT_EXCEEDED"
weight = 56
+++

**描述:** Multipart 文件数量超限


**语法:** `MULTIPART_FILE_LIMIT_EXCEEDED`


**示例:**


```apache
SecRule MULTIPART_FILE_LIMIT_EXCEEDED "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
