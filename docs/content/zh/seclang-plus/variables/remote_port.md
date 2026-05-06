+++
title = "REMOTE_PORT"
weight = 33
+++

**描述:** 客户端端口

**语法:** `REMOTE_PORT`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule REMOTE_PORT "@rx value" "id:1001,deny,msg:'Test'"
```
