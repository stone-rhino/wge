+++
title = "allow:phase"
weight = 13
+++

**描述:** 允许请求通过当前阶段

**信息:** Original Example: `allow:phase`

**语法:** `allow:phase`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,allow:phase"
```


**参数类型:** `string`


该操作支持更精细的控制。可选参数如下： 若需放行响应，请在 RESPONSE_HEADERS 阶段添加规则并单独使用 allow：
