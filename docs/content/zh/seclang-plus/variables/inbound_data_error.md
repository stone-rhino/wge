+++
title = "INBOUND_DATA_ERROR"
weight = 61
+++

**描述:** 请求体长度超限错误

**语法:** `INBOUND_DATA_ERROR`

当请求体大小高于SecRequestBodyLimit指令配置的设置时，此变量将被设置为1。

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule INBOUND_DATA_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```
