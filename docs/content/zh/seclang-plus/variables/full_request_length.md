+++
title = "FULL_REQUEST_LENGTH"
weight = 23
+++

**描述:** 完整请求的长度


**语法:** `FULL_REQUEST_LENGTH`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule FULL_REQUEST_LENGTH "@gt 10485760" "id:1001,deny,msg:'Test'"
```
