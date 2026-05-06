+++
title = "TIME_HOUR"
weight = 98
+++

**描述:** 当前小时值

**语法:** `TIME_HOUR`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule TIME_HOUR "@rx value" "id:1001,deny,msg:'Test'"
```
