+++
title = "allow"
weight = 12
+++

**描述:** 允许请求通过

**语法:** `allow | allow:phase | allow:request`

**区分大小写:** 是

`allow`：跳过后续所有规则的执行。

`allow:phase`：跳过当前阶段后续所有规则的执行。

`allow:request`：跳过请求阶段所有规则的执行。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,allow,status:403"
```