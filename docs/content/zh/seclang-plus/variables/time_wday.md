+++
title = "TIME_WDAY"
weight = 101
+++

**描述:** 当前工作日值

**语法:** `TIME_WDAY`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule TIME_WDAY "@rx value" "id:1001,deny,msg:'Test'"
```
