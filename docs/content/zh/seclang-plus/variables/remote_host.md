+++
title = "REMOTE_HOST"
weight = 32
+++

**描述:** 客户端主机名

**语法:** `REMOTE_HOST`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule REMOTE_HOST "@rx value" "id:1001,deny,msg:'Test'"
```
