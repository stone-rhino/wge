+++
title = "TIME_DAY"
weight = 97
+++

**描述:** 日期

**语法:** `TIME_DAY`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule TIME_DAY "@rx value" "id:1001,deny,msg:'Test'"
```
