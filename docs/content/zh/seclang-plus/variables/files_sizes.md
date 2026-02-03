+++
title = "FILES_SIZES"
weight = 77
+++

**描述:** 上传文件大小


**语法:** `FILES_SIZES`


**示例:**


```apache
SecRule FILES_SIZES "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
