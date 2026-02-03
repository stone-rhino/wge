+++
title = "MULTIPART_FILENAME"
weight = 40
+++

**描述:** Multipart 文件名


**语法:** `MULTIPART_FILENAME`


**示例:**


```apache
SecRule MULTIPART_FILENAME "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
