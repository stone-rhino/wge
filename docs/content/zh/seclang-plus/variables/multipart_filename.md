+++
title = "MULTIPART_FILENAME"
weight = 40
+++

**描述:** Multipart 文件名

**语法:** `MULTIPART_FILENAME`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule MULTIPART_FILENAME "@rx value" "id:1001,deny,msg:'Test'"
```
