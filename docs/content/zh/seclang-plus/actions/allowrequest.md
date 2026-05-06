+++
title = "allow:request"
weight = 14
+++

**描述:** 允许整个请求通过

**语法:** `allow:request`

**区分大小写:** 是

allow:request 动作允许整个请求跳过所有剩余规则。适用于白名单已知安全的请求或受信来源。一旦触发，该请求在任何阶段都不再执行后续规则。

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,allow:request"
```
