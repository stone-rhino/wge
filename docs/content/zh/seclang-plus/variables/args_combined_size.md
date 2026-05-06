+++
title = "ARGS_COMBINED_SIZE"
weight = 7
+++

**描述:** 所有参数的总大小

**语法:** `ARGS_COMBINED_SIZE`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule ARGS_COMBINED_SIZE "@eq 10" "id:1001,deny,msg:'Test'"
```
