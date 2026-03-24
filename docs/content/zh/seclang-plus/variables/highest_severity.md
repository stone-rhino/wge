+++
title = "HIGHEST_SEVERITY"
weight = 65
+++

**描述:** 最高严重级别


**语法:** `HIGHEST_SEVERITY`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule HIGHEST_SEVERITY "@rx value" "id:1001,deny,msg:'Test'"
```
