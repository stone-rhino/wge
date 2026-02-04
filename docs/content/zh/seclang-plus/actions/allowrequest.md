+++
title = "allow:request"
weight = 14
+++

**描述:** 允许整个请求通过

**信息:** Original Example: `allow:request`

**语法:** `allow:request`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,allow:request"
```


**参数类型:** `string`


此操作对 SecRule 指令和 SecAction 指令均为必需。目前虽非 SecRuleScript 指令强制要求，但强烈建议使用。
