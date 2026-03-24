+++
title = "WEBAPPID"
weight = 72
+++

**描述:** Web 应用 ID

**语法:** `WEBAPPID`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule WEBAPPID "@rx value" "id:1001,deny,msg:'Test'"
```
