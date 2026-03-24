+++
title = "FILES_SIZES"
weight = 77
+++

**描述:** 上传文件大小


**语法:** `FILES_SIZES`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**


```apache
SecRule FILES_SIZES "@rx value" "id:1001,deny,msg:'Test'"
```
