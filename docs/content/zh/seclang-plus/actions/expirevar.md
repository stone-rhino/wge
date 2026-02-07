+++
title = "expirevar"
weight = 26
+++

**描述:** 设置变量在指定时间（秒为单位）后过期。

**语法:** `expirevar:variable=INT`

**区分大小写:** 是

**是否实现:** 否

此指令并未在WGE中实现，无法使用。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,expirevar:session.suspicious=3600,status:403"
```