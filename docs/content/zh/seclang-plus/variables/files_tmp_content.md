+++
title = "FILES_TMP_CONTENT"
weight = 79
+++

**描述:** 上传文件临时内容


**语法:** `FILES_TMP_CONTENT`


**示例:**


```apache
SecRule FILES_TMP_CONTENT "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
