+++
title = "FULL_REQUEST"
weight = 22
+++

**描述:** 完整的请求数据

**语法:** `FULL_REQUEST`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule FULL_REQUEST "@rx value" "id:1001,deny,msg:'Test'"
```
