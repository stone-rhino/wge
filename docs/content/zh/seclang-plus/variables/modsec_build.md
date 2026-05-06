+++
title = "MODSEC_BUILD"
weight = 74
+++

**描述:** ModSecurity 构建版本

**语法:** `MODSEC_BUILD`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule MODSEC_BUILD "@rx value" "id:1001,deny,msg:'Test'"
```
