+++
title = "FILES_TMPNAMES"
weight = 78
+++

**描述:** 上传文件临时名称


**语法:** `FILES_TMPNAMES`


**是否实现:** 否

**示例:**


```apache
SecRule FILES_TMPNAMES "@rx value" "id:1001,deny,msg:'Test'"
```
