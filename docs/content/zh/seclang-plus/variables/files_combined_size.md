+++
title = "FILES_COMBINED_SIZE"
weight = 80
+++

**描述:** 所有上传文件的总大小


**语法:** `FILES_COMBINED_SIZE`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule FILES_COMBINED_SIZE "@rx value" "id:1001,deny,msg:'Test'"
```
