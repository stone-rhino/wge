+++
title = "USERID"
weight = 71
+++

**描述:** 用户 ID

**语法:** `USERID`

此变量包含使用setuid设置的值。

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule USERID "@rx value" "id:1001,deny,msg:'Test'"
```
