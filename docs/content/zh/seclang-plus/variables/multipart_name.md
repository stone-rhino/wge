+++
title = "MULTIPART_NAME"
weight = 41
+++

**描述:** Multipart 字段名

**语法:** `MULTIPART_NAME`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule MULTIPART_NAME "@rx value" "id:1001,deny,msg:'Test'"
```
