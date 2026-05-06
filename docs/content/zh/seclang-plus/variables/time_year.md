+++
title = "TIME_YEAR"
weight = 95
+++

**描述:** 当前年份

**语法:** `TIME_YEAR`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule TIME_YEAR "@rx value" "id:1001,deny,msg:'Test'"
```
