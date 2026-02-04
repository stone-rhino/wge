+++
title = "MULTIPART_INVALID_QUOTING"
weight = 53
+++

**描述:** Multipart 无效引号


**语法:** `MULTIPART_INVALID_QUOTING`


**示例:**


```apache
SecRule MULTIPART_INVALID_QUOTING "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
