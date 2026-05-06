+++
title = "TIME_MIN"
weight = 99
+++

**描述:** 当前分钟值

**语法:** `TIME_MIN`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule TIME_MIN "@rx value" "id:1001,deny,msg:'Test'"
```
