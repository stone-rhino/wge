+++
title = "TIME_SEC"
weight = 100
+++

**描述:** 当前秒

**语法:** `TIME_SEC`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule TIME_SEC "@rx value" "id:1001,deny,msg:'Test'"
```
