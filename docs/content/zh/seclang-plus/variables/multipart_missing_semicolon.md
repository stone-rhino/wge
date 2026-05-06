+++
title = "MULTIPART_MISSING_SEMICOLON"
weight = 52
+++

**描述:** Multipart 缺少分号

**语法:** `MULTIPART_MISSING_SEMICOLON`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule MULTIPART_MISSING_SEMICOLON "@rx value" "id:1001,deny,msg:'Test'"
```
