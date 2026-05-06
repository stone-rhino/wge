+++
title = "noauditlog"
weight = 21
+++

**描述:** 不记录到审计日志

**语法:** `noauditlog`

**区分大小写:** 是

与 auditlog 一样，其在WGE中并不存在实际的意义。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,noauditlog,status:403"
```