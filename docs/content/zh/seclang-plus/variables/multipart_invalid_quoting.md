+++
title = "MULTIPART_INVALID_QUOTING"
weight = 53
+++

**描述:** Multipart 无效引号

**语法:** `MULTIPART_INVALID_QUOTING`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule MULTIPART_INVALID_QUOTING "@rx value" "id:1001,deny,msg:'Test'"
```
