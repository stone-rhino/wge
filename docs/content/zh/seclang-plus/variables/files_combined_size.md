+++
title = "FILES_COMBINED_SIZE"
weight = 80
+++

**描述:** 所有上传文件的总大小


**语法:** `FILES_COMBINED_SIZE`


**示例:**


```apache
SecRule FILES_COMBINED_SIZE "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
