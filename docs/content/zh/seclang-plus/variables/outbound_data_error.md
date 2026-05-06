+++
title = "OUTBOUND_DATA_ERROR"
weight = 62
+++

**描述:** 响应体长度超限错误

**语法:** `OUTBOUND_DATA_ERROR`

当响应体大小高于SecResponseBodyLimit指令配置的设置时，此变量将被设置为1。

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule OUTBOUND_DATA_ERROR "@rx value" "id:1001,deny,msg:'Test'"
```
