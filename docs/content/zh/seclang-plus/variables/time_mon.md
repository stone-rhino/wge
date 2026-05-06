+++
title = "TIME_MON"
weight = 96
+++

**描述:** 当前月份值

**语法:** `TIME_MON`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule TIME_MON "@rx value" "id:1001,deny,msg:'Test'"
```
