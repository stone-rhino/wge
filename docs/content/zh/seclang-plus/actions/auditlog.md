+++
title = "auditlog"
weight = 20
+++

**描述:** 记录到审计日志

**语法:** `auditlog`

**区分大小写:** 是

此action虽然能够正常解析和设置相关标志，但是其并没有实际上的作用。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,auditlog,status:403"
```